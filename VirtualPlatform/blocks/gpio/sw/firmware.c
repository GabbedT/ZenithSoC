#include "platform.h"

/*
 * GPIO register map (from gpio.sv):
 *   0 = PIN_VALUE  (R/W: set output value / read pin)
 *   1 = DIRECTION  (W: 0=output, 1=input)
 *   2 = INT_ENABLE
 *   3 = INT_STATUS
 *
 * Data is 8 bits wide (bits [7:0] of the 32-bit word).
 */

#define GPIO_PIN_VALUE  REG32(GPIO_BASE + 0x00)
#define GPIO_DIRECTION  REG32(GPIO_BASE + 0x04)
#define GPIO_INT_ENABLE REG32(GPIO_BASE + 0x08)
#define GPIO_INT_STATUS REG32(GPIO_BASE + 0x0C)

void main(void) {
    vp_println("[GPIO] Test start");

    /* Set all pins as output (direction = 0) */
    GPIO_DIRECTION = 0x00;

    /* Write a pattern */
    GPIO_PIN_VALUE = 0xA5;

    vp_println("[GPIO] Wrote 0xA5 to output pins");

    /* Read back */
    uint32_t val = GPIO_PIN_VALUE;
    vp_print("[GPIO] Read back: ");
    vp_print_hex(val);
    vp_putchar('\n');

    if ((val & 0xFF) == 0xA5) {
        vp_println("[GPIO] PASS");
        TEST_PASS();
    } else {
        vp_println("[GPIO] FAIL");
        TEST_FAIL();
    }
}
