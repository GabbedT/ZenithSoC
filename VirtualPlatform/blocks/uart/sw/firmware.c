#include "platform.h"

#include <stdbool.h>

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

#define UART_CONTROL    REG32(UART_BASE + 0x00)
#define UART_TX_DATA    REG32(UART_BASE + 0x04)
#define UART_RX_DATA    REG32(UART_BASE + 0x08)
#define UART_EVENT      REG32(UART_BASE + 0x0C)

#include <stdint.h>
#include <stdbool.h>

typedef union {
    uint32_t raw;

    struct {
        /* Buffer status */
        unsigned int emptyRX : 1;
        unsigned int fullRX : 1;
        unsigned int emptyTX : 1;
        unsigned int fullTX : 1;

        /* Enable interrupt for each event */
        unsigned int interruptEnable : 5;

        /* Operation enable */
        unsigned int enableRX : 1;
        unsigned int enableTX : 1;

        /* Communication config */
        unsigned int parityEnable : 1;
        unsigned int parityMode : 1;
        unsigned int stopBits : 1;
        unsigned int dataBits : 2;

        /* Enable CTS - RTS flow control */
        unsigned int flowControl : 1;

        /* Clock divider */
        unsigned int clockDivider : 15;
    } bits;
} uartCtrlStatus_t;


void main(void) {
    uartCtrlStatus_t ctrl; 

    vp_println("[UART] Test start");

    /* Configure: 8N1, baud divider for loopback testing */
    /* Set a reasonable baud divider (system_clk / (16 * baud)) */
    ctrl.raw = UART_CONTROL;

    ctrl.bits.enableRX = true;
    ctrl.bits.enableTX = true;
    ctrl.bits.parityEnable = true;

    UART_CONTROL = ctrl.raw;


    vp_print_hex(UART_CONTROL);

    vp_println("[UART] Config written");

    /* Write a test byte to TX */
    UART_TX_DATA = 0x55;

    vp_println("[UART] TX byte sent (0x55)");

    /* In loopback, the TX output feeds back to RX.
     * Wait some cycles for transmission to complete.
     * In the VP, each UART bit-time takes divider*16 clock cycles. */

    vp_delay_cycles(50000);

    /* Read status to check if data received */
    uint32_t status = UART_CONTROL;
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