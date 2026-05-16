#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <optional>

#include "Vtb_top.h"
#include "verilated.h"
#include "verilated_fst_c.h"

#include "riscv/abstract_device.h"
#include "riscv/cfg.h"
#include "riscv/sim.h"
#include "riscv/mmu.h"
#include "riscv/processor.h"

// =============================================
// Verilator Model
// =============================================

static Vtb_top *dut = nullptr;
static VerilatedFstC *tfp = nullptr;
static uint64_t sim_time = 0;

static constexpr uint64_t HALF_PERIOD_PS = 5000;

void clk_tick() {
    dut->clk = 1;
    dut->eval();
    if (tfp) tfp->dump(sim_time);
    sim_time += HALF_PERIOD_PS;

    dut->clk = 0;
    dut->eval();
    if (tfp) tfp->dump(sim_time);
    sim_time += HALF_PERIOD_PS;
}

void reset_dut() {
    dut->rst_n = 0;
    for (int i = 0; i < 5; i++) clk_tick();
    dut->rst_n = 1;
    clk_tick();
}

uint32_t axi_read(uint32_t address) {
    dut->read_i = 1;
    dut->read_address_i = address >> 2;

    clk_tick();
    dut->read_i = 0;

    while (!dut->read_done_o) {
        clk_tick();
    }

    uint32_t data = dut->read_data_o;
    clk_tick();

    std::cout << "[AXI RD] Address: 0x" << std::hex << std::setw(8) << std::setfill('0') << address
              << " Data: 0x" << std::setw(8) << data << std::dec << std::endl;

    return data;
}

void axi_write(uint32_t address, uint32_t data, uint8_t strb) {
    dut->write_i = 1;
    dut->write_address_i = address >> 2;
    dut->write_data_i = data;
    dut->write_strobe_i = strb;

    clk_tick();
    dut->write_i = 0;

    while (!dut->write_done_o) {
        clk_tick();
    }

    std::cout << "[AXI WR] Address: 0x" << std::hex << std::setw(8) << std::setfill('0') << address
              << " Data: 0x" << std::setw(8) << data
              << " Strobe: 0x" << (int) strb << std::dec << std::endl;
}

// =============================================
// ZenithSoC MMIO Device for Spike
// =============================================

// Block-specific MMIO device. BASE and SIZE are set per block via
// command-line args (+io_base=0xNNNN +io_size=0xNNNN).
// Addresses reaching the DUT are word-aligned register indices (offset >> 2).
class zenith_io_device_t : public abstract_device_t {
public:
    uint64_t base;
    uint64_t dev_size;

    zenith_io_device_t(uint64_t b, uint64_t s)
        : base(b), dev_size(s) {}

    reg_t size() override {
        return dev_size;
    }

    bool load(reg_t addr, size_t len, uint8_t* bytes) override {
        uint32_t offset = static_cast<uint32_t>(addr);

        uint32_t data = axi_read(offset);

        // Shift data for byte/halfword reads
        data >>= 8 * (offset & 0x3);

        memcpy(bytes, &data, std::min(len, sizeof(data)));

        return true;
    }

    bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
        uint32_t offset = static_cast<uint32_t>(addr);

        uint32_t data = 0;
        memcpy(&data, bytes, std::min(len, sizeof(data)));

        uint8_t strb = 0xF;

        if (len == 1) {
            uint32_t byte_pos = offset & 0x3;
            strb = 1 << byte_pos;
            data <<= 8 * byte_pos;
        } else if (len == 2) {
            uint32_t half_pos = offset & 0x2;
            strb = 3 << half_pos;
            data <<= 8 * half_pos;
        }

        axi_write(offset, data, strb);

        return true;
    }
};


class vp_tick_t : public abstract_device_t {
public:
    static const uint64_t BASE = 0x30000000;
    static const uint64_t SIZE = 0x1000;

    reg_t size() override {
        return SIZE;
    }

    bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
        uint32_t cycles = 0;
        memcpy(&cycles, bytes, std::min(len, sizeof(cycles)));

        std::cout << "[VP TICK] advancing " << cycles << " cycles" << std::endl;

        for (uint32_t i = 0; i < cycles; i++) {
            clk_tick();
        }

        return true;
    }

    bool load(reg_t addr, size_t len, uint8_t* bytes) override {
        memset(bytes, 0, len);
        return true;
    }
};


// Debug UART: prints characters to host console
class debug_uart_t : public abstract_device_t {
public:
    static const uint64_t BASE = 0x10000000;
    static const uint64_t SIZE = 0x00001000;

    bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
        for (size_t i = 0; i < len; i++) {
            putchar(bytes[i]);
        }

        fflush(stdout);

        return true;
    }

    reg_t size() override {
        return 0x1000;
    }

    bool load(reg_t addr, size_t len, uint8_t* bytes) override {
        memset(bytes, 0, len);

        return true;
    }
};

// Test result register: firmware writes here to signal pass/fail
class test_result_t : public abstract_device_t {
public:
    static const uint64_t BASE = 0x20000000;
    static const uint64_t SIZE = 0x00001000;
    int result = -1;

    bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
        uint32_t val = 0;
        memcpy(&val, bytes, std::min(len, sizeof(val)));
        result = val;
        std::cout << "[TEST] Result = " << val << (val ? " PASS" : " FAIL") << std::endl;
        return true;
    }

    reg_t size() override {
        return 0x1000;
    }

    bool load(reg_t addr, size_t len, uint8_t* bytes) override {
        memset(bytes, 0, len);
        return true;
    }
};

// =============================================
// Main
// =============================================

int main(int argc, char **argv) {
    Verilated::commandArgs(argc, argv);

    // 1. Create Verilator model
    dut = new Vtb_top;

    // 2. Enable waveform tracing (pass +notrace to disable)
    bool enable_trace = true;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "+notrace") enable_trace = false;
    }

    if (enable_trace) {
        Verilated::traceEverOn(true);
        tfp = new VerilatedFstC;
        dut->trace(tfp, 99);
        tfp->open("out/waveform.fst");
    }

    // 3. Reset
    reset_dut();

    // 4. Parse command-line arguments
    std::string fw_path = "out/firmware.elf";
    uint64_t io_base = 0x00004000;  // default: UART base
    uint64_t io_size = 0x00002000;  // default: one device interleave
    
    for (int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (arg.find("+firmware=") == 0)
            fw_path = arg.substr(10);
        else if (arg.find("+io_base=") == 0)
            io_base = std::stoull(arg.substr(9), nullptr, 0);
        else if (arg.find("+io_size=") == 0)
            io_size = std::stoull(arg.substr(9), nullptr, 0);
    }

    // 5. Configure Spike
    cfg_t cfg;
    cfg.isa = "rv32im_zfinx_zba_zbs_zicsr";
    cfg.priv = "m";


    std::vector<std::pair<reg_t, abstract_mem_t*>> mems;

    reg_t mem_base = 0x80000000;
    reg_t mem_size = 128 * 1024 * 1024;

    mems.push_back(std::make_pair(mem_base, new mem_t(mem_size)));


    auto io_dev = std::make_shared<zenith_io_device_t>(io_base, io_size);
    auto dbg_uart = std::make_shared<debug_uart_t>();
    auto test_res = std::make_shared<test_result_t>();
    auto vp_tick = std::make_shared<vp_tick_t>();


    debug_module_config_t dm_config;

    std::vector<std::pair<const device_factory_t*, std::vector<std::string>>> plugin_device_factories;

    std::vector<std::string> htif_args;
    htif_args.push_back(fw_path);


    sim_t spike(
        &cfg,
        false,
        mems,
        plugin_device_factories,
        false,
        htif_args,
        dm_config,
        nullptr,
        false,
        nullptr,
        false,
        nullptr,
        std::nullopt
    );

    const_cast<bus_t&>(spike.get_bus()).add_device(io_base, io_dev.get());
    const_cast<bus_t&>(spike.get_bus()).add_device(debug_uart_t::BASE, dbg_uart.get());
    const_cast<bus_t&>(spike.get_bus()).add_device(test_result_t::BASE, test_res.get());
    const_cast<bus_t&>(spike.get_bus()).add_device(vp_tick_t::BASE, vp_tick.get());

    // 6. Run Spike
    std::cout << "[VP] Starting simulation with firmware: " << fw_path << std::endl;
    std::cout << "[VP] MMIO range: 0x" << std::hex
              << io_base << " - 0x" << (io_base + io_size - 1)
              << std::dec << std::endl;

    std::cout << "[VP] Forcing PC to firmware entry 0x80000000" << std::endl;
    spike.get_core(0)->get_state()->pc = 0x80000000;

    std::cout << "[VP] Entering Spike run..." << std::endl;
    spike.run();

    std::cout << "[VP] Spike terminated!" << std::endl;

    // 7. Cleanup
    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete dut;

    std::cout << "\n[VP] Simulation completed." << std::endl;
    std::cout << "[VP] Sim time:         " << sim_time << " ticks" << std::endl;

    if (enable_trace) {
        std::cout << "[VP] Waveform: out/waveform.fst" << std::endl;
    }

    return (test_res->result == 1) ? 0 : 1;
}
