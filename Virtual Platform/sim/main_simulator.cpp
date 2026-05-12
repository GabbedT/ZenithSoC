#include <iostream>
#include <cstdint>
#include <memory>

#include "Vtb_top.h"
#include "verilated.h"
#include "verilated_fst_c.h"

/* Verilator Model */
static Vtb_top *dut = nullptr;
static VerilatedFstC *tfp = nullptr;

static uint64_t simTime = 0;

static const uint64_t CLK_PERIOD = 10; /* 100 MHz */


/* Run clock */
void clk_tick() {
    /* Rising Edge */
    dut->clk_i = 1;
    dut->eval();

    if (tfp) {
        tfp->dump(simTime++);
    }


    /* Falling Edge */
    dut->clk_i = 0;
    dut->eval();

    if (tfp) {
        tfp->dump(simTime++);
    }
};

/* Reset system */
void reset_dut() {
    dut->rst_n_i = 0;

    for (int i = 0; i < 10 ++i) {
        clk_tick();
    }

    dut->rst_n_i = 1;
    clk_tick();
};


uint32_t io_read(uint32_t address) {
    dut->read_start_i = 1;
    dut->read_invalid_i = 0;
    dut->read_address_i = address;
    clk_tick();

    dut->read_start_i = 0;
    while (!dut->read_done_o) clk_tick(); 

    uint32_t data = dut->read_data_o;
    clk_tick();

    std::cout << "[AXI READ] Address = 0x" << std::hex << address << " Data: 0x" << data << std::dec << std::endl;

    return data;
};


void io_write(uint32_t address, uint32_t data, uint8_t strobe) {
    dut->write_start_i = 1;
    dut->write_address_i = address;
    dut->write_data_i = data;
    dut->write_strobe_i = strobe;
    clk_tick();

    dut->write_start_i = 0;
    while (!dut->write_done_o) clk_tick(); 

    clk_tick();
    std::cout << "[AXI WRITE] Address = 0x" << std::hex << address << " Data: 0x" << data << std::dec << std::endl;
};