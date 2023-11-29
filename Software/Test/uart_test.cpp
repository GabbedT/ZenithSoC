#include "../Library/Driver/UART.h"

extern "C" void uart_test() {
    /* Create UART device */
    UART uart(0);

    /* Read registers to assert they are initialized */
    struct UART::uartStatus_s uartStatus = uart.getStatus();
    UART::uartEvent_e uartEvent = uart.getEvent();

    /* Setup communication */
    uart.init(115200, true, UART::EVEN, UART::STOP1, UART::BIT8)
        .setFlowControl(false);

    /* Check modifications */
    uartStatus = uart.getStatus();

    /* Send a string */
    uint8_t str[] = {"Test UART device!"};

    uart.loadBufferTX(str, sizeof(str))
        .sendByte('\n');

    /* Put in wait to receive a byte */
    while (uart.getStatus().emptyRX) {  }

    /* Send back the received data */
    uart.sendByte(uart.receiveByte());

    return;
}