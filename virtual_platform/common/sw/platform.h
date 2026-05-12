#ifndef VP_PLATFORM_H
#define VP_PLATFORM_H

#include <stdint.h>

/* ============================================================
 *  ZenithSoC Memory Map (matching real hardware)
 *  Addresses from ZenithSoC/Software/Library/mmio.h
 * ============================================================ */

#define MMIO_START        0x00004000
#define DEVICE_INTERLEAVE (1 << 13)

/* Peripheral base addresses */
#define UART_BASE     (MMIO_START)
#define TIMER_BASE    (UART_BASE     + DEVICE_INTERLEAVE)
#define GPIO_BASE     (TIMER_BASE    + DEVICE_INTERLEAVE)
#define SPI_BASE      (GPIO_BASE     + DEVICE_INTERLEAVE)
#define ETHERNET_BASE (SPI_BASE      + DEVICE_INTERLEAVE)
#define PRNG_BASE     (ETHERNET_BASE + DEVICE_INTERLEAVE)
#define APU_BASE      (PRNG_BASE     + DEVICE_INTERLEAVE)
#define NC_MEMORY_BASE (APU_BASE     + DEVICE_INTERLEAVE + (1 << 10))
#define SD_BASE       (NC_MEMORY_BASE + DEVICE_INTERLEAVE)

/* Platform constants */
#define SYSTEM_FREQUENCY 100000000
#define CLOCK_PERIOD     10

/* ============================================================
 *  Virtual Platform Helpers
 * ============================================================ */

/* Debug UART: prints to host console via Spike */
#define DEBUG_UART_TX (*(volatile char *)0x10000000)

/* Test result register */
#define TEST_RESULT   (*(volatile uint32_t *)0x20000000)
#define TEST_PASS()   do { TEST_RESULT = 1; } while(0)
#define TEST_FAIL()   do { TEST_RESULT = 0; } while(0)

/* MMIO access helpers */
#define REG32(addr)   (*(volatile uint32_t *)(addr))
#define REG16(addr)   (*(volatile uint16_t *)(addr))
#define REG8(addr)    (*(volatile uint8_t  *)(addr))

/* Debug print to host console */
static inline void vp_putchar(char c) {
    DEBUG_UART_TX = c;
}

static inline void vp_print(const char *s) {
    while (*s) DEBUG_UART_TX = *s++;
}

static inline void vp_print_hex(uint32_t val) {
    const char hex[] = "0123456789abcdef";
    vp_print("0x");
    for (int i = 28; i >= 0; i -= 4) {
        vp_putchar(hex[(val >> i) & 0xF]);
    }
}

static inline void vp_println(const char *s) {
    vp_print(s);
    vp_putchar('\n');
}

#endif
