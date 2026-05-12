#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

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
static uint64_t total_axi_reads = 0;
static uint64_t total_axi_writes = 0;

void clk_tick() {
    dut->clk = 1;
    dut->eval();
    if (tfp) tfp->dump(sim_time++);

    dut->clk = 0;
    dut->eval();
    if (tfp) tfp->dump(sim_time++);
}

void reset_dut() {
    dut->rst_n = 0;
    for (int i = 0; i < 5; i++) clk_tick();
    dut->rst_n = 1;
    clk_tick();
}

// =============================================
// AXI Transaction Functions
// =============================================

static const uint32_t MAX_AXI_CYCLES = 10000;

uint32_t axi_read(uint32_t addr) {
    dut->cmd_valid = 1;
    dut->cmd_write = 0;
    dut->cmd_addr  = addr;

    uint32_t timeout = 0;
    while (!dut->cmd_ready && timeout < MAX_AXI_CYCLES) {
        clk_tick();
        timeout++;
    }
    clk_tick();
    dut->cmd_valid = 0;

    timeout = 0;
    while (!dut->cmd_done && timeout < MAX_AXI_CYCLES) {
        clk_tick();
        timeout++;
    }

    uint32_t data = dut->cmd_rdata;
    clk_tick();

    total_axi_reads++;
    std::cout << "[AXI RD] addr=0x" << std::hex << std::setw(8) << std::setfill('0') << addr
              << " data=0x" << std::setw(8) << data << std::dec << std::endl;

    if (timeout >= MAX_AXI_CYCLES) {
        std::cerr << "[ERROR] AXI read timeout at addr=0x" << std::hex << addr << std::dec << std::endl;
    }

    return data;
}

void axi_write(uint32_t addr, uint32_t data, uint8_t strb) {
    dut->cmd_valid = 1;
    dut->cmd_write = 1;
    dut->cmd_addr  = addr;
    dut->cmd_wdata = data;
    dut->cmd_wstrb = strb;

    uint32_t timeout = 0;
    while (!dut->cmd_ready && timeout < MAX_AXI_CYCLES) {
        clk_tick();
        timeout++;
    }
    clk_tick();
    dut->cmd_valid = 0;

    timeout = 0;
    while (!dut->cmd_done && timeout < MAX_AXI_CYCLES) {
        clk_tick();
        timeout++;
    }
    clk_tick();

    total_axi_writes++;
    std::cout << "[AXI WR] addr=0x" << std::hex << std::setw(8) << std::setfill('0') << addr
              << " data=0x" << std::setw(8) << data
              << " strb=0x" << (int)strb << std::dec << std::endl;

    if (timeout >= MAX_AXI_CYCLES) {
        std::cerr << "[ERROR] AXI write timeout at addr=0x" << std::hex << addr << std::dec << std::endl;
    }
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
    uint64_t size;

    zenith_io_device_t(uint64_t b, uint64_t s) : base(b), size(s) {}

    bool load(reg_t addr, size_t len, uint8_t* bytes) override {
        uint32_t offset = (uint32_t)(addr - base);
        uint32_t word_addr = offset >> 2;
        uint32_t data = axi_read(word_addr);
        memcpy(bytes, &data, std::min(len, sizeof(data)));
        return true;
    }

    bool store(reg_t addr, size_t len, const uint8_t* bytes) override {
        uint32_t offset = (uint32_t)(addr - base);
        uint32_t word_addr = offset >> 2;
        uint32_t data = 0;
        memcpy(&data, bytes, std::min(len, sizeof(data)));

        uint8_t strb = 0xF;
        if (len == 1)      strb = 1 << (offset & 0x3);
        else if (len == 2) strb = 3 << (offset & 0x2);

        axi_write(word_addr, data, strb);
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
    cfg.isa = "rv32im_zicsr";
    cfg.priv = "m";
    cfg.misaligned = false;

    std::vector<mem_cfg_t> mem_layout = {
        {0x80000000, 0x40000}  // 256KB starting at 0x80000000
    };

    auto io_dev = std::make_shared<zenith_io_device_t>(io_base, io_size);
    auto dbg_uart = std::make_shared<debug_uart_t>();
    auto test_res = std::make_shared<test_result_t>();

    std::vector<std::pair<reg_t, abstract_device_t*>> plugin_devices;
    plugin_devices.push_back({io_base, io_dev.get()});
    plugin_devices.push_back({debug_uart_t::BASE, dbg_uart.get()});
    plugin_devices.push_back({test_result_t::BASE, test_res.get()});

    sim_t spike(&cfg, "",
                mem_layout,
                plugin_devices,
                {},                // debug_module_config
                fw_path.c_str(),
                false,             // halted
                nullptr,           // dtm
                false,             // socket
                nullptr);          // cmd_file

    // 6. Run Spike
    std::cout << "[VP] Starting simulation with firmware: " << fw_path << std::endl;
    std::cout << "[VP] MMIO range: 0x" << std::hex
              << io_base << " - 0x" << (io_base + io_size - 1)
              << std::dec << std::endl;

    spike.run();

    // 7. Cleanup
    if (tfp) {
        tfp->close();
        delete tfp;
    }
    delete dut;

    std::cout << "\n[VP] Simulation completed." << std::endl;
    std::cout << "[VP] Total AXI reads:  " << total_axi_reads << std::endl;
    std::cout << "[VP] Total AXI writes: " << total_axi_writes << std::endl;
    std::cout << "[VP] Sim time:         " << sim_time << " ticks" << std::endl;

    if (enable_trace) {
        std::cout << "[VP] Waveform: out/waveform.fst" << std::endl;
    }

    return (test_res->result == 1) ? 0 : 1;
}
