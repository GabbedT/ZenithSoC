#include "platform.h"

#define MY_REG_0  REG32(UART_BASE + 0x00)

void main(void) {
    vp_println("[FW] Template test start");

    MY_REG_0 = 0xCAFEBABE;
    uint32_t val = MY_REG_0;

    vp_print("[FW] Read back: ");
    vp_print_hex(val);
    vp_putchar('\n');

    if (val == 0xCAFEBABE) {
        vp_println("[FW] PASS");
        TEST_PASS();
    } else {
        vp_println("[FW] FAIL");
        TEST_FAIL();
    }
}
