#include "platform.h"

/*
 * PRNG register map (from prng.sv):
 *   0 = LFSR_LOW  (R/W: read low 32 bits / write seed low)
 *   1 = LFSR_HIGH (R/W: read high 32 bits / write seed high)
 *
 * The LFSR shifts every clock cycle when not being written.
 */

#define PRNG_LOW   REG32(PRNG_BASE + 0x00)
#define PRNG_HIGH  REG32(PRNG_BASE + 0x04)

void main(void) {
    vp_println("[PRNG] Test start");

    /* Seed the LFSR */
    PRNG_LOW  = 0xDEADBEEF;
    PRNG_HIGH = 0x12345678;

    vp_println("[PRNG] Seed written");

    /* Read first random value */
    uint32_t val1 = PRNG_LOW;
    vp_print("[PRNG] Value 1: ");
    vp_print_hex(val1);
    vp_putchar('\n');

    /* Let it shift a few cycles */
    volatile int delay;
    for (delay = 0; delay < 100; delay++);

    /* Read second random value */
    uint32_t val2 = PRNG_LOW;
    vp_print("[PRNG] Value 2: ");
    vp_print_hex(val2);
    vp_putchar('\n');

    /* Values should differ (LFSR is shifting) */
    if (val1 != val2 && val1 != 0 && val2 != 0) {
        vp_println("[PRNG] PASS - values differ");
        TEST_PASS();
    } else {
        vp_println("[PRNG] FAIL - values identical or zero");
        TEST_FAIL();
    }
}
