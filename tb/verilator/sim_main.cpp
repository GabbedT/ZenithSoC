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
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

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

static volatile std::sig_atomic_t g_stop_requested = 0;
static volatile std::sig_atomic_t g_stop_signal = 0;


// -----------------------------------------------------------------------------
//      SD CARD BACKING STORE
// -----------------------------------------------------------------------------

static std::vector<uint8_t> g_sd_bytes;

static bool has_hex_extension(const std::string& path) {
    std::string lower = path;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return lower.size() >= 4 && lower.substr(lower.size() - 4) == ".hex";
}

static bool load_sd_hex(const std::string& path, std::vector<uint8_t>& data) {
    std::ifstream input(path);
    if (!input.is_open())
        return false;

    uint64_t cursor = 0;
    std::string line;

    while (std::getline(input, line)) {
        const auto comment = line.find_first_of("#;");
        if (comment != std::string::npos)
            line.erase(comment);

        const auto cpp_comment = line.find("//");
        if (cpp_comment != std::string::npos)
            line.erase(cpp_comment);

        std::istringstream tokens(line);
        std::string token;

        while (tokens >> token) {
            if (!token.empty() && token.front() == '@') {
                try {
                    cursor = std::stoull(token.substr(1), nullptr, 16);
                } catch (...) {
                    return false;
                }
                continue;
            }

            if (token.rfind("0x", 0) == 0 || token.rfind("0X", 0) == 0)
                token.erase(0, 2);

            token.erase(std::remove(token.begin(), token.end(), '_'), token.end());
            if (!token.empty() && token.back() == ',')
                token.pop_back();

            if (token.empty())
                continue;

            if (token.size() % 2 != 0)
                token.insert(token.begin(), '0');

            for (size_t pos = 0; pos < token.size(); pos += 2) {
                uint8_t value;
                try {
                    value = static_cast<uint8_t>(
                        std::stoul(token.substr(pos, 2), nullptr, 16));
                } catch (...) {
                    return false;
                }

                if (cursor >= data.size())
                    data.resize(cursor + 1, 0xFF);
                data[cursor++] = value;
            }
        }
    }

    return true;
}

static bool load_sd_image(const std::string& path, uint32_t block) {
    std::vector<uint8_t> image;

    if (has_hex_extension(path)) {
        if (!load_sd_hex(path, image))
            return false;
    } else {
        std::ifstream input(path, std::ios::binary);
        if (!input.is_open())
            return false;

        image.assign(std::istreambuf_iterator<char>(input),
                     std::istreambuf_iterator<char>());
    }

    const uint64_t offset = static_cast<uint64_t>(block) * 512;
    if (offset + image.size() > UINT32_MAX)
        return false;

    g_sd_bytes.assign(offset + image.size(), 0xFF);
    std::copy(image.begin(), image.end(), g_sd_bytes.begin() + offset);

    std::cout << "[ZTB] SD image loaded: " << path
              << " bytes=" << image.size()
              << " block=0x" << std::hex << block
              << " byte_offset=0x" << offset << std::dec << "\n";
    return true;
}

extern "C" uint32_t zenith_sd_read_word(uint32_t byte_addr) {
    uint32_t data = 0xFFFFFFFFu;

    for (uint32_t lane = 0; lane < 4; lane++) {
        const uint64_t address = static_cast<uint64_t>(byte_addr) + lane;
        if (address < g_sd_bytes.size()) {
            data &= ~(0xFFu << (lane * 8));
            data |= static_cast<uint32_t>(g_sd_bytes[address]) << (lane * 8);
        }
    }

    // The VP card PHY serializes each 32-bit Wishbone word MSB-first, while
    // the Zenith SD controller exposes received words in little-endian CPU
    // order. Swap here so a byte-for-byte disk image reaches DDR unchanged.
    return ((data & 0x000000FFu) << 24)
         | ((data & 0x0000FF00u) << 8)
         | ((data & 0x00FF0000u) >> 8)
         | ((data & 0xFF000000u) >> 24);
}

extern "C" void zenith_sd_write_word(uint32_t byte_addr,
                                      uint32_t data,
                                      uint32_t strobe) {
    const uint64_t end = static_cast<uint64_t>(byte_addr) + 4;
    if (end > UINT32_MAX)
        return;
    if (end > g_sd_bytes.size())
        g_sd_bytes.resize(end, 0xFF);

    const uint32_t disk_data = ((data & 0x000000FFu) << 24)
                             | ((data & 0x0000FF00u) << 8)
                             | ((data & 0x00FF0000u) >> 8)
                             | ((data & 0xFF000000u) >> 24);
    const uint32_t disk_strobe = ((strobe & 0x1u) << 3)
                               | ((strobe & 0x2u) << 1)
                               | ((strobe & 0x4u) >> 1)
                               | ((strobe & 0x8u) >> 3);

    for (uint32_t lane = 0; lane < 4; lane++) {
        if (disk_strobe & (1u << lane))
            g_sd_bytes[byte_addr + lane] =
                (disk_data >> (lane * 8)) & 0xFF;
    }
}


// -----------------------------------------------------------------------------
//      UART
// -----------------------------------------------------------------------------

static std::ofstream g_uart_file;
static std::ofstream g_trace_file;

static void uart_capture_open(const std::string& dir) {
    std::string path = dir + "/stdout.txt";

    g_uart_file.open(path, std::ios::out | std::ios::trunc);

    if (!g_uart_file.is_open()) {
        std::cerr << "[ZTB] WARN: cannot open " << path << " for UART capture\n";
    }
}


extern "C" void zenith_uart_tx_byte(uint32_t data) {
    char c = static_cast<char> (data & 0xFF);

    std::cout << c << std::flush;

    if (g_uart_file.is_open()) {
        g_uart_file << c << std::flush;
    }
}

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
        uint64_t trace_start,
        uint64_t max_cycles)
        : enable_wave_(trace_wave),
          enable_print_(trace_print),
          trace_start_(trace_start),
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

        while (!finished_ && !g_stop_requested) {
            tick();

            if (tohost_addr_ && tohost_hit_) {
                uint32_t exit_code = tohost_value_ >> 1;

                std::cout << "[ZTB] tohost write (value=0x"
                        << std::hex << tohost_value_
                        << ", exit_code=" << std::dec << exit_code
                        << ") -> stop\n";

                if (exit_code == 0) {
                    std::cout << "[ZTB] PASS\n";
                    return 0;
                } else {
                    std::cout << "[ZTB] FAIL\n";
                    return 1;
                }
            }

            if (max_cycles_ && cycles_ >= max_cycles_) {
                std::cout << "[ZTB] reached max_cycles="
                        << std::dec
                        << max_cycles_
                        << " -> stop\n";

                if (!recent_events_.empty()) {
                    std::cout << "[ZTB] last retire: cycle=" << last_retire_cycle_
                              << " pc=0x" << std::hex
                              << recent_events_.back().pc << std::dec << "\n";

                    for (const TraceEvent& event : recent_events_)
                        print_event(event);
                }
                return 1;
            }
        }

        if (g_stop_requested) {
            std::cout << "\n[ZTB] signal "
                    << g_stop_signal
                    << " -> graceful stop\n";
            return 128 + g_stop_signal;
        }

        return 0;
    }

    void set_tohost(uint32_t a) { tohost_addr_ = a; }

    void verify_ddr_image(const ElfImage& img) {
        size_t mismatches = 0;

        for (const auto& [addr, expected] : img.words) {
            if (addr < USER_BASE)
                continue;

            const uint32_t actual = peek_insn(addr);
            if (actual != expected) {
                if (mismatches < 8) {
                    std::cout << "[ZTB] DDR mismatch @0x" << std::hex << addr
                              << " expected=0x" << expected
                              << " actual=0x" << actual << std::dec << "\n";
                }
                mismatches++;
            }
        }

        std::cout << "[ZTB] SD-to-DDR verification: "
                  << (mismatches == 0 ? "MATCH" : "MISMATCH")
                  << " (" << mismatches << " differing ELF words)\n";
    }

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

            recent_events_.push_back(e);
            if (recent_events_.size() > 32)
                recent_events_.pop_front();
            last_retire_cycle_ = cycles_;

            if (e.is_store &&
                tohost_addr_ &&
                e.mem_addr == tohost_addr_) {

                tohost_hit_ = true;
                tohost_value_ = e.mem_data;
                finished_ = true;
            }

            if (enable_print_ && cycles_ >= trace_start_)
                print_event(e);
        }
    }

    void print_event(const TraceEvent& e) {
        std::ostream& os = g_trace_file.is_open() ? g_trace_file : std::cout;

        os << std::hex << std::setfill('0');
        os << "0x" << std::setw(8) << e.pc << " : ";
        

        if (e.is_exception) {
            os << "<exception vec="
               << std::dec
               << e.info
               << ">\n";
            os << std::setfill(' ');
            return;
        }

        // Disassemble the word fetched at PC (DDR region only).
        uint32_t word = peek_insn(e.pc);
        insn_t insn(word);

        os << std::setfill(' ')
           << std::left
           << std::setw(28)
           << dis_.disassemble(insn)
           << std::right;

        if (e.rd != 0) {
            os << " x"
               << std::dec
               << std::setfill('0')
               << std::setw(2)
               << e.rd
               << " <= 0x"
               << std::hex
               << std::setfill('0')
               << std::setw(8)
               << e.rd_value;
        }

        if (e.is_store || e.is_load) {
            static const char* w[] = {"b", "h", "w"};

            const char* ws =
                (e.mem_width <= 2)
                    ? w[e.mem_width]
                    : "?";

            os << " | "
               << (e.is_store ? "ST" : "LD")
               << "."
               << ws
               << " @0x"
               << std::hex
               << std::setfill('0')
               << std::setw(8)
               << e.mem_addr;

            if (e.is_store) {
                os << " data 0x"
                   << std::setfill('0')
                   << std::setw(8)
                   << e.mem_data;
            }
        }

        os << std::setfill(' ')
           << std::dec
           << "\n";
    }

    Vzenith_tb_top* dut_ = nullptr;
    VerilatedFstC* tfp_ = nullptr;
    svScope top_scope_ = nullptr;

    bool enable_wave_;
    bool enable_print_;

    uint64_t trace_start_;
    uint64_t max_cycles_;
    uint64_t cycles_ = 0;
    uint64_t sim_time_ = 0;

    uint32_t tohost_addr_ = 0;
    bool tohost_hit_ = false;
    uint32_t tohost_value_ = 0;
    bool finished_ = false;

    std::deque<TraceEvent> recent_events_;
    uint64_t last_retire_cycle_ = 0;

    isa_parser_t isa_;
    disassembler_t dis_;
};

// ============================================================================
//      SIGNALS
// ============================================================================

static Sim* g_sim = nullptr;

static void signal_handler(int s) {
    g_stop_requested = 1;
    g_stop_signal = s;
}

// ============================================================================
//      MAIN
// ============================================================================
int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    std::string fw_path, boot_path, sd_path;
    uint32_t sd_block = 0x2000;
    bool enable_wave  = false;
    bool enable_print = true;
    uint64_t trace_start = 0;
    uint64_t max_cycles = 0;   // 0 = unlimited

    for (int i = 1; i < argc; i++) {
        std::string a(argv[i]);

        if (a.rfind("+firmware=", 0) == 0)
            fw_path = a.substr(10);
        else if (a.rfind("+boot=", 0) == 0)
            boot_path = a.substr(6);
        else if (a.rfind("+sd=", 0) == 0)
            sd_path = a.substr(4);
        else if (a.rfind("+sd_block=", 0) == 0)
            sd_block = std::stoul(a.substr(10), nullptr, 0);
        else if (a == "+wave")
            enable_wave = true;
        else if (a == "+notrace")
            enable_print = false;
        else if (a.rfind("+trace_start=", 0) == 0)
            trace_start = std::stoull(a.substr(13));
        else if (a.rfind("+max_cycles=", 0) == 0)
            max_cycles = std::stoull(a.substr(12));
    }

    if (fw_path.empty() && sd_path.empty()) {
        std::cerr << "[ZTB] usage: " << argv[0]
                  << " +firmware=fw.elf [+boot=boot.elf] [+wave] [+notrace]"
                  << " [+sd=image.bin|hex] [+sd_block=N] [+max_cycles=N]\n";
        return 2;
    }

    ElfImage img;
    if (!fw_path.empty() && !load_elf(fw_path, img)) {
        std::cerr << "[ZTB] cannot load firmware ELF: "
                  << fw_path
                  << "\n";
        return 2;
    }

    if (!sd_path.empty() && !load_sd_image(sd_path, sd_block)) {
        std::cerr << "[ZTB] cannot load SD image: " << sd_path << "\n";
        return 2;
    }

    uart_capture_open("out");
    g_trace_file.open("out/trace.txt", std::ios::out | std::ios::trunc);

    g_sim = new Sim(enable_wave, enable_print, trace_start, max_cycles);
    if (!g_sim->scope()) {
        std::cerr << "[ZTB] FATAL: DPI scope zenith_tb_top not found\n";
        return 4;
    }

    // In SD-boot mode the application ELF is metadata only (entry/tohost and
    // disassembly). Leaving DDR untouched ensures the bootloader really copies
    // the application from the card before executing it.
    if (sd_path.empty() && !fw_path.empty())
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
              << " firmware=" << (fw_path.empty() ? "<none>" : fw_path)
              << "\n";

    int rc = g_sim->run(img.tohost);

    // Dump performance counters at end of simulation
    svSetScope(g_sim->scope());
    zenith_perf_dump();

    if (!sd_path.empty() && !fw_path.empty())
        g_sim->verify_ddr_image(img);

    delete g_sim;
    g_sim = nullptr;

    if (g_trace_file.is_open()) {
        g_trace_file.flush();
        g_trace_file.close();
    }

    if (g_uart_file.is_open()) {
        g_uart_file.flush();
        g_uart_file.close();
    }

    return rc;
}
