#include "Vfpu_test_top.h"
#include "verilated.h"
extern "C" {
#include "softfloat.h"
}
#include <array>
#include <cstdint>
#include <cstdio>
#include <random>

static uint32_t reference(unsigned op, unsigned mode, uint32_t a, uint32_t b)
{
    softfloat_roundingMode = mode;
    softfloat_detectTininess = softfloat_tininess_afterRounding;
    softfloat_exceptionFlags = 0;
    uint32_t result;
    switch (op) {
    case 0: result = f32_add({a}, {b}).v; break;
    case 1: result = f32_sub({a}, {b}).v; break;
    case 2: result = f32_mul({a}, {b}).v; break;
    case 3: return uint32_t(f32_to_i32({a}, mode, true));
    case 4: return uint32_t(f32_to_ui32({a}, mode, true));
    case 5: result = i32_to_f32(int32_t(a)).v; break;
    default: result = ui32_to_f32(a).v; break;
    }
    // RISC-V returns the canonical NaN for arithmetic operations.
    if ((result & 0x7fffffff) > 0x7f800000) result = 0x7fc00000;
    return result;
}

int main(int argc, char **argv)
{
    Verilated::commandArgs(argc, argv);
    Vfpu_test_top dut;
    auto tick = [&] {
        dut.clk_i = 0; dut.eval();
        dut.clk_i = 1; dut.eval();
    };
    dut.rst_n_i = 0;
    dut.valid_i = dut.stall_i = dut.flush_i = 0;
    tick();
    dut.rst_n_i = 1;
    std::mt19937 rng(0x5eed);
    const std::array<uint32_t, 24> edges = {
        0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000,
        0x3effffff, 0x3f000000, 0x3f000001, 0xbf000000,
        0x3f800000, 0xbf800000, 0x3fc00000, 0xbfc00000,
        0x4effffff, 0x4f000000, 0xcf000000, 0xcf000001,
        0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000,
        0x7fc00000, 0x7f800001
    };
    unsigned compared = 0, flushed = 0;
    for (unsigned test = 0; test < 100000; ++test) {
        unsigned op = rng() % 7, mode = rng() % 5;
        uint32_t a = (test % 3) ? rng() : edges[rng() % edges.size()];
        uint32_t b = (test % 3) ? rng() : edges[rng() % edges.size()];
        uint32_t expected = reference(op, mode, a, b);
        dut.operand_a_i = a; dut.operand_b_i = b;
        dut.operation_i = op; dut.rounding_i = mode;
        dut.valid_i = 1; dut.stall_i = 0;
        tick();
        dut.valid_i = 0;

        if (test % 31 == 0) {
            dut.flush_i = 1;
            tick();
            dut.flush_i = 0;
            for (unsigned cycle = 0; cycle < 8; ++cycle) {
                tick();
                if (dut.valid_o) {
                    std::fprintf(stderr, "FPU retired a flushed operation\n");
                    return 1;
                }
            }
            ++flushed;
            continue;
        }

        bool completed = false;
        for (unsigned cycle = 0; cycle < 64; ++cycle) {
            dut.stall_i = (rng() % 4 == 0);
            tick();
            if (dut.valid_o && !dut.stall_i) {
                if (dut.result_o != expected) {
                    std::fprintf(stderr, "FPU op=%u rm=%u a=%08x b=%08x: got=%08x expected=%08x\n",
                                 op, mode, a, b, dut.result_o, expected);
                    return 1;
                }
                completed = true;
                ++compared;
                break;
            }
        }
        if (!completed) {
            std::fprintf(stderr, "FPU operation timed out\n");
            return 1;
        }
    }
    dut.final();
    std::printf("FPU PASS: %u SoftFloat result comparisons, %u flushes, randomized stalls\n",
                compared, flushed);
}
