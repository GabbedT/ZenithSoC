#ifndef VP_PLATFORM_H
#define VP_PLATFORM_H

#include <stdint.h>

/* ============================================================
 *  VP Memory Map 
 * ============================================================ */

#define VP_TICK_ADDR 0x30000000u
#define VP_TICK (*(volatile uint32_t *)VP_TICK_ADDR)


/* ============================================================
 *  Virtual Platform Helpers
 * ============================================================ */

/* Debug UART: prints to host console via Spike */
#define VP_STDOUT (*(volatile char *)0x10000000)

/* Test result register */
#define TEST_RESULT   (*(volatile uint32_t *)0x20000000)
#define TEST_PASS()   do { TEST_RESULT = 1; } while(0)
#define TEST_FAIL()   do { TEST_RESULT = 0; } while(0)


/* Print single char */
static inline void vp_putchar(char c) {
    VP_STDOUT = c;
}

/* Print string */
static inline void vp_print(const char *s) {
    while (*s) VP_STDOUT = *s++;
}

/* Print hex value */
static inline void vp_print_hex(uint32_t val) {
    const char hex[] = "0123456789abcdef";
    vp_print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vp_putchar(hex[(val >> i) & 0xF]);
    }
}

/* Print string with newline */
static inline void vp_println(const char *s) {
    vp_print(s);
    vp_putchar('\n');
}

/* Print hex number with newline */
static inline void vp_println_hex(uint32_t val) {
    vp_print_hex(val);
    vp_putchar('\n');
}


/* Delay the simulation of N cycles (N is 10ns) */
static inline void vp_delay_cycles(uint32_t cycles)
{
    VP_TICK = cycles;
}

#endif
