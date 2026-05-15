#include "platform.h"

/*
 * Timer register map (from timer.sv):
 *   0 = COMPARE_LOW
 *   1 = COMPARE_HIGH
 *   2 = TIMER_VALUE_LOW
 *   3 = TIMER_VALUE_HIGH
 *   4 = CONFIGURATION  (bit0=enable, bit1=one_shot, bit2=int_enable)
 */

#define TIMER_COMPARE_LO  REG32(TIMER_BASE + 0x00)
#define TIMER_COMPARE_HI  REG32(TIMER_BASE + 0x04)
#define TIMER_VALUE_LO    REG32(TIMER_BASE + 0x08)
#define TIMER_VALUE_HI    REG32(TIMER_BASE + 0x0C)
#define TIMER_CONFIG      REG32(TIMER_BASE + 0x10)

void main(void) {
    vp_println("[TIMER] Test start");

    /* Set compare value to something small */
    TIMER_COMPARE_LO = 100;
    TIMER_COMPARE_HI = 0;

    vp_println("[TIMER] Compare set to 100");

    /* Read back compare to verify write */
    uint32_t cmp = TIMER_COMPARE_LO;
    vp_print("[TIMER] Compare readback: ");
    vp_print_hex(cmp);
    vp_putchar('\n');

    /* Enable timer (bit 0) */
    TIMER_CONFIG = 0x01;

    vp_println("[TIMER] Timer enabled");

    /* Wait some cycles for the timer to count */
    volatile int delay;
    for (delay = 0; delay < 500; delay++);

    /* Read current timer value */
    uint32_t val = TIMER_VALUE_LO;
    vp_print("[TIMER] Timer value: ");
    vp_print_hex(val);
    vp_putchar('\n');

    /* Timer should have counted past 0 */
    if (val > 0 && cmp == 100) {
        vp_println("[TIMER] PASS");
        TEST_PASS();
    } else {
        vp_println("[TIMER] FAIL");
        TEST_FAIL();
    }
}
