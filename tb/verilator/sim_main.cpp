// ============================================================================
//   1. Build the Verilated model (zenith_tb_top: ZenithSoC + internal DDR/ROM).
//   2. Load a firmware ELF (and optional boot ELF) into the DUT DDR/ROM through
//      the DPI export functions defined in the wrapper.
//   3. Drive clock and reset, then let the core run freely.
//   4. Print an execution trace: PC | disasm | rd<=value | mem access.
//   5. Stop on a `tohost` write, on +max_cycles, or on Ctrl-C.
//
// The trace disassembler reuses Spike's disassembler_t (libriscv), exactly like
// the cosim flow. The ISA string is injected at build time via -DCOSIM_ISA.
// ============================================================================

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <deque>
#include <csignal>

#include "Vzenith_tb_top.h"
#include "Vzenith_tb_top__Dpi.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include "svdpi.h"

#include "riscv/isa_parser.h"
#include "riscv/disasm.h"

#include "elf_loader.h"      // reused from cosim/sim (added to the include path)

#ifndef COSIM_ISA
#define COSIM_ISA "rv32im_zicsr"
#endif

// -----------------------------------------------------------------------------
//      MEMORY MAP (apogeo_memory_map.svh)
// -----------------------------------------------------------------------------
static constexpr uint32_t BOOT_END  = 0x00004000u;   // 16 KiB boot ROM
static constexpr uint32_t USER_BASE = 0x80000000u;   // DDR / user region

static constexpr uint64_t HALF_PERIOD_NS = 5;        // 10 ns -> 100 MHz model

// -----------------------------------------------------------------------------
//      RETIRED-INSTRUCTION EVENT (filled by the DPI trace callback)
// -----------------------------------------------------------------------------
struct TraceEvent {
    bool     is_exception;
    uint32_t pc;
    uint32_t info;
    uint32_t rd;
    uint32_t rd_value;
    bool     is_store;
    bool     is_load;
    uint32_t mem_addr;
    uint32_t mem_data;
    uint32_t mem_width;
};

static std::deque<TraceEvent> g_events;

// DPI import: called by the wrapper on every retired instruction.
extern "C" void zenith_trace_commit(uint32_t is_exception,
                                    uint32_t pc,
                                    uint32_t info,
                                    uint32_t rd,
                                    uint32_t rd_value,
                                    uint32_t is_store,
                                    uint32_t is_load,
                                    uint32_t mem_addr,
                                    uint32_t mem_data,
                                    uint32_t mem_width) {

    g_events.push_back(TraceEvent{
        is_exception != 0,
        pc,
        info,
        rd,
        rd_value,
        is_store != 0,
        is_load != 0,
        mem_addr,
        mem_data,
        mem_width
    });
}

// ============================================================================
//      SIMULATION DRIVER
// ============================================================================

class Sim {
public:
    Sim(bool trace_wave,
        bool trace_print,
        uint64_t max_cycles)
        : enable_wave_(trace_wave),
          enable_print_(trace_print),
          max_cycles_(max_cycles),
          isa_(COSIM_ISA, "MSU"),
          dis_(&isa_) {

        dut_ = new Vzenith_tb_top;

        if (enable_wave_) {
            Verilated::traceEverOn(true);
            tfp_ = new VerilatedFstC;
            dut_->trace(tfp_, 99);
            tfp_->open("out/zenith.fst");
        }

        // One initial eval so that SystemVerilog `initial` blocks run before we
        // preload memory. memory_bank.SV zeroes its array and runs $readmemh in
        // an initial block; in Verilator that fires on the first eval().
        // If we preloaded the ROM before this, it would be wiped. (The DDR model
        // has no initial block, so it is unaffected either way.)
        dut_->clk = 0;
        dut_->rst_n = 0;
        dut_->eval();

        // Resolve the DPI scope of the wrapper (where the preload/peek
        // functions live) so svSetScope() targets the right module.
        top_scope_ = svGetScopeFromName("TOP.zenith_tb_top");
        if (!top_scope_)
            top_scope_ = svGetScopeFromName("zenith_tb_top");
    }

    ~Sim() {
        if (tfp_) {
            tfp_->close();
            delete tfp_;
        }
        if (dut_)
            delete dut_;
    }

    svScope scope() const { return top_scope_; }

    // --- Firmware loading ---------------------------------------------------
    // User words (>= USER_BASE) go to the DDR model (relative addressing);
    // boot words (< BOOT_END) go to the ROM banks.
    void preload_image(const ElfImage& img) {
        svSetScope(top_scope_);
        for (const auto& [addr, data] : img.words) {
            if (addr >= USER_BASE)
                zenith_ddr_preload_word(addr - USER_BASE, data);
            else if (addr < BOOT_END)
                zenith_rom_preload_word(addr, data);
        }
    }

    void preload_boot(const ElfImage& boot) {
        svSetScope(top_scope_);
        for (const auto& [addr, data] : boot.words)
            if (addr < BOOT_END)
                zenith_rom_preload_word(addr, data);
    }

    // Fetch a 32-bit word as the core would see it, for disassembly.
    uint32_t peek_insn(uint32_t pc) {
        svSetScope(top_scope_);
        if (pc >= USER_BASE)
            return zenith_ddr_peek_word(pc - USER_BASE);
        if (pc < BOOT_END)
            return zenith_rom_peek_word(pc);
        return 0;
    }


    // --- Clock / reset ------------------------------------------------------
    void tick() {
        dut_->clk = 1;
        dut_->eval();
        dump();

        Verilated::timeInc(HALF_PERIOD_NS);
        sim_time_ += HALF_PERIOD_NS;

        dut_->clk = 0;
        dut_->eval();
        dump();

        Verilated::timeInc(HALF_PERIOD_NS);
        sim_time_ += HALF_PERIOD_NS;

        cycles_++;
        drain_trace();
    }

    void reset() {
        dut_->rst_n = 0;
        for (int i = 0; i < 8; i++)
            tick();

        dut_->rst_n = 1;
        tick();
    }

    // --- Main run loop ------------------------------------------------------
    int run(uint32_t tohost_addr) {
        std::cout << "[ZTB] start (max_cycles="
                  << (max_cycles_
                          ? std::to_string(max_cycles_)
                          : std::string("inf"))
                  << ")\n";

        reset();

        while (!finished_) {
            tick();

            if (tohost_addr_ && tohost_hit_) {
                std::cout << "[ZTB] tohost write (value=0x"
                          << std::hex
                          << tohost_value_
                          << std::dec
                          << ") -> stop\n";

                return (tohost_value_ == 1) ? 0 : 1;
            }

            if (max_cycles_ && cycles_ >= max_cycles_) {
                std::cout << "[ZTB] reached max_cycles="
                          << std::dec
                          << max_cycles_
                          << " -> stop\n";
                return 0;
            }
        }

        return 0;
    }

    void set_tohost(uint32_t a) { tohost_addr_ = a; }

    // --- Future manual-control seams (not implemented yet) -----------------
    // void pause();
    // void resume();
    // void poke_mem(uint32_t addr, uint32_t data);
    // void force_irq(uint8_t vector);

private:
    void dump() {
        if (tfp_)
            tfp_->dump(sim_time_);
    }

    // Pop retired events, print them, and watch for the tohost store.
    void drain_trace() {
        while (!g_events.empty()) {
            TraceEvent e = g_events.front();
            g_events.pop_front();

            if (e.is_store &&
                tohost_addr_ &&
                e.mem_addr == tohost_addr_) {

                tohost_hit_ = true;
                tohost_value_ = e.mem_data;
                finished_ = true;
            }

            if (enable_print_)
                print_event(e);
        }
    }

    void print_event(const TraceEvent& e) {
        std::cout << std::hex << std::setfill('0');
        std::cout << "0x" << std::setw(8) << e.pc << " : ";

        if (e.is_exception) {
            std::cout << "<exception vec="
                      << std::dec
                      << e.info
                      << ">\n";
            std::cout << std::setfill(' ');
            return;
        }

        // Disassemble the word fetched at PC (DDR region only).
        uint32_t word = peek_insn(e.pc);
        insn_t insn(word);

        std::cout << std::setfill(' ')
                  << std::left
                  << std::setw(28)
                  << dis_.disassemble(insn)
                  << std::right;

        if (e.rd != 0) {
            std::cout << " x"
                      << std::dec
                      << std::setw(2)
                      << e.rd
                      << " <= 0x"
                      << std::hex
                      << std::setw(8)
                      << e.rd_value;
        }

        if (e.is_store || e.is_load) {
            static const char* w[] = {"b", "h", "w"};

            const char* ws =
                (e.mem_width <= 2)
                    ? w[e.mem_width]
                    : "?";

            std::cout << " | "
                      << (e.is_store ? "ST" : "LD")
                      << "."
                      << ws
                      << " @0x"
                      << std::hex
                      << std::setw(8)
                      << e.mem_addr;

            if (e.is_store) {
                std::cout << " data 0x"
                          << std::setw(8)
                          << e.mem_data;
            }
        }

        std::cout << std::setfill(' ')
                  << std::dec
                  << "\n";
    }

    Vzenith_tb_top* dut_ = nullptr;
    VerilatedFstC* tfp_ = nullptr;
    svScope top_scope_ = nullptr;

    bool enable_wave_;
    bool enable_print_;

    uint64_t max_cycles_;
    uint64_t cycles_ = 0;
    uint64_t sim_time_ = 0;

    uint32_t tohost_addr_ = 0;
    bool tohost_hit_ = false;
    uint32_t tohost_value_ = 0;
    bool finished_ = false;

    isa_parser_t isa_;
    disassembler_t dis_;
};

// ============================================================================
//      SIGNALS
// ============================================================================

static Sim* g_sim = nullptr;

static void signal_handler(int s) {
    std::cout << "\n[ZTB] signal "
              << s
              << " -> exit\n";

    if (g_sim)
        delete g_sim;

    std::exit(128 + s);
}

// ============================================================================
//      MAIN
// ============================================================================
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::string fw_path, boot_path;
    bool enable_wave  = false;
    bool enable_print = true;
    uint64_t max_cycles = 0;   // 0 = unlimited

    for (int i = 1; i < argc; i++) {
        std::string a(argv[i]);

        if (a.rfind("+firmware=", 0) == 0)
            fw_path = a.substr(10);
        else if (a.rfind("+boot=", 0) == 0)
            boot_path = a.substr(6);
        else if (a == "+wave")
            enable_wave = true;
        else if (a == "+notrace")
            enable_print = false;
        else if (a.rfind("+max_cycles=", 0) == 0)
            max_cycles = std::stoull(a.substr(12));
    }

    if (fw_path.empty()) {
        std::cerr << "[ZTB] usage: " << argv[0]
                  << " +firmware=fw.elf [+boot=boot.elf] [+wave] [+notrace]"
                  << " [+max_cycles=N]\n";
        return 2;
    }

    ElfImage img;
    if (!load_elf(fw_path, img)) {
        std::cerr << "[ZTB] cannot load firmware ELF: "
                  << fw_path
                  << "\n";
        return 2;
    }

    g_sim = new Sim(enable_wave, enable_print, max_cycles);
    if (!g_sim->scope()) {
        std::cerr << "[ZTB] FATAL: DPI scope zenith_tb_top not found\n";
        return 4;
    }

    g_sim->preload_image(img);
    g_sim->set_tohost(img.tohost);

    ElfImage boot;
    if (!boot_path.empty() && load_elf(boot_path, boot)) {
        g_sim->preload_boot(boot);
        std::cout << "[ZTB] boot stub loaded from "
                  << boot_path
                  << "\n";
    } else if (!boot_path.empty()) {
        std::cout << "[ZTB] WARN: cannot load boot ELF "
                  << boot_path
                  << "\n";
    } else {
        std::cout << "[ZTB] no boot ELF: core starts from ROM[0]=0\n";
    }

    std::cout << "[ZTB] ISA=" << COSIM_ISA
              << " entry=0x" << std::hex << img.entry
              << " tohost=0x" << img.tohost << std::dec
              << " firmware=" << fw_path
              << "\n";

    int rc = g_sim->run(img.tohost);

    delete g_sim;
    g_sim = nullptr;

    return rc;
}