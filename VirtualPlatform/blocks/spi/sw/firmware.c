#include "platform.h"

/*
 * SPI register map (from spi_pkg.sv):
 *   0 = TX_DATA
 *   1 = RX_DATA
 *   2 = CONFIG (divider, CPOL, CPHA, bit order)
 *   3 = SLAVE_SELECT
 *   4 = STATUS
 */

#define SPI_TX_DATA      REG32(SPI_BASE + 0x00)
#define SPI_RX_DATA      REG32(SPI_BASE + 0x04)
#define SPI_CONFIG        REG32(SPI_BASE + 0x08)
#define SPI_SLAVE_SELECT  REG32(SPI_BASE + 0x0C)
#define SPI_STATUS        REG32(SPI_BASE + 0x10)

void main(void) {
    vp_println("[SPI] Test start");

    /* Configure: divider=10, CPOL=0, CPHA=0, MSB first */
    SPI_CONFIG = 10;

    /* Select slave 0 */
    SPI_SLAVE_SELECT = 0x01;

    vp_println("[SPI] Config written");

    /* Send test byte (loopback: MOSI->MISO) */
    SPI_TX_DATA = 0xAB;

    vp_println("[SPI] TX byte sent (0xAB)");

    /* Wait for transmission */
    volatile int delay;
    for (delay = 0; delay < 10000; delay++);

    /* Read status */
    uint32_t status = SPI_STATUS;
    vp_print("[SPI] Status: ");
    vp_print_hex(status);
    vp_putchar('\n');

    /* Read received data */
    uint32_t rx = SPI_RX_DATA;
    vp_print("[SPI] RX byte: ");
    vp_print_hex(rx);
    vp_putchar('\n');

    if ((rx & 0xFF) == 0xAB) {
        vp_println("[SPI] Loopback PASS");
        TEST_PASS();
    } else {
        vp_println("[SPI] Loopback FAIL (expected 0xAB)");
        TEST_FAIL();
    }
}
