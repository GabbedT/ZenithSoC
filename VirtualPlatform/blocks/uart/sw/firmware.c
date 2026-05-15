#include "platform.h"

/*
 * UART register map (from uart_pkg.sv):
 *   0x00 = TX_DATA / CONFIG
 *   0x04 = RX_DATA / STATUS
 *   0x08 = BAUD_DIVIDER
 *   0x0C = INTERRUPT_ENABLE
 *
 * Word-aligned register indices (address >> 2):
 *   0 = TX_DATA
 *   1 = RX_BUFFER
 *   2 = CONFIG (baud, data length, parity, etc.)
 *   3 = STATUS
 */

#define UART_TX_DATA    REG32(UART_BASE + 0x00)
#define UART_RX_DATA    REG32(UART_BASE + 0x04)
#define UART_CONFIG     REG32(UART_BASE + 0x08)
#define UART_STATUS     REG32(UART_BASE + 0x0C)

void main(void) {
    vp_println("[UART] Test start");

    /* Configure: 8N1, baud divider for loopback testing */
    /* Set a reasonable baud divider (system_clk / (16 * baud)) */
    UART_CONFIG = (1 << 15) | (1 << 14) | 54;  /* TX enable | RX enable | divider */

    vp_println("[UART] Config written");

    /* Write a test byte to TX */
    UART_TX_DATA = 0x55;

    vp_println("[UART] TX byte sent (0x55)");

    /* In loopback, the TX output feeds back to RX.
     * Wait some cycles for transmission to complete.
     * In the VP, each UART bit-time takes divider*16 clock cycles. */

    volatile int delay;
    for (delay = 0; delay < 50000; delay++);

    /* Read status to check if data received */
    uint32_t status = UART_STATUS;
    vp_print("[UART] Status: ");
    vp_print_hex(status);
    vp_putchar('\n');

    /* Read received byte */
    uint32_t rx = UART_RX_DATA;
    vp_print("[UART] RX byte: ");
    vp_print_hex(rx);
    vp_putchar('\n');

    if ((rx & 0xFF) == 0x55) {
        vp_println("[UART] Loopback PASS");
        TEST_PASS();
    } else {
        vp_println("[UART] Loopback FAIL");
        TEST_FAIL();
    }
}
