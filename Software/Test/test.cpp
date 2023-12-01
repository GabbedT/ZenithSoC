#include "../Library/Driver/UART.h"
#include "../Library/Driver/GPIO.h"
#include "../Library/Driver/Timer.h"

extern "C" void test() {

/*
 * UART TEST
 */

    /* Create UART device */
    UART uart(0);

    /* Setup communication */
    uart.init(115200, true, UART::EVEN, UART::STOP1, UART::BIT8)
        .setFlowControl(false);

    /* Send a string */
    uint8_t uartPresentation[] = {"Test UART device!\n"};
    uart.loadBufferTX(uartPresentation, sizeof(uartPresentation));

    uint8_t uartRequest[] = {"Send 10 characters!\n"};
    uart.loadBufferTX(uartRequest, sizeof(uartRequest));

    /* Receive buffer */
    uint8_t uartResponse[10];
    uart.unloadBufferRX(uartResponse, sizeof(uartResponse));

    /* Send back the received data */
    uint8_t uartConfirm[] = {"Received: "};
    uart.loadBufferTX(uartConfirm, sizeof(uartConfirm));
    uart.loadBufferTX(uartResponse, sizeof(uartResponse));


/*
 * GPIO TEST
 */

    /* Create GPIO device */
    GPIO gpio(0);

    /* GPIO[3:0] = OUTPUT, GPIO[7:4] = INPUT */
    gpio.init(0x00, 0xF0, 0x00, 0xFF);

    for (int i = 0; i < 4; ++i) {
        /* Set output ON */
        gpio.setPinValue(i, true);
    }

    /* Scan through all the input pins from 4-th to 7-th */
    for (int i = 4; i < 8; ++i) {
        /* Wait until input pin gets triggered */
        while (!gpio.getPinValue(i)) { }

        gpio.setPinValue(i - 4, false);
    }


/*
 * TIMER TEST
 */

    Timer timer(0);

    timer.init(5 * 100, Timer::ONE_SHOT).setInterrupt(false).start();

    for (int i = 0; i < 4; ++i) {
        gpio.setPinValue(i, true);
        
        while (!timer.getConfiguration().halted) {  }
        timer.restart();

        gpio.setPinValue(i, false);
    }

    return;
}