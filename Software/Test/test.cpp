#include "../Library/Driver/UART.h"
#include "../Library/Driver/GPIO.h"
#include "../Library/Driver/Timer.h"
#include "../Library/Driver/SPI.h"

#include <inttypes.h>

void print(const char *str, UART& device);

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
    print("===================== TEST START =====================\n", uart);
    print("Send 10 characters!\n", uart);

    /* Receive buffer */
    uint8_t uartResponse[10];
    // uart.unloadBufferRX(uartResponse, sizeof(uartResponse));

    /* Send back the received data */
    print("Received: ", uart);
    print((const char *) uartResponse, uart);

    uart.sendByte('\n');


/*
 * GPIO TEST
 */

    /* Create GPIO device */
    GPIO gpio(0);

    /* GPIO[3:0] = OUTPUT, GPIO[7:4] = INPUT */
    gpio.init(0x00, 0xF0, 0x00, 0xFF);

    for (int i = 0; i < 4; ++i) {
        /* Set output ON */
        print("Inside first for loop!\n", uart);
        gpio.setPinValue(i, true);
    }

    /* Scan through all the input pins from 4-th to 7-th */
    for (int i = 4; i < 8; ++i) {
        /* Wait until input pin gets triggered */
        // print("Waiting for pin to be high...\n", uart);
        // while (!gpio.getPinValue(i)) { }
        
        print("Pin high!\n", uart);
        gpio.setPinValue(i - 4, false);
    }


/*
 * TIMER TEST
 */

    Timer timer(0);

    timer.init(100, Timer::ONE_SHOT)
         .setInterrupt(false)
         .start();

    for (int i = 0; i < 4; ++i) {
        gpio.setPinValue(i, true);
        
        print("Waiting for timer to halt...\n", uart);
        while (!timer.getConfiguration()->halted) {  }
        print("Halted!\n", uart);

        timer.restart();
        print("Restarted!\n", uart);

        gpio.setPinValue(i, false);
    }


/*
 * SPI TEST
 */

    SPI spi(0);

    spi.init(1'000'000, SPI::MODE0, SPI::MSB_FIRST)
       .enableInterrupt(false)
       .connect(0);

    /* Send 64 bit */
    print("Sending 64 bits...\n", uart);
    uint64_t data64 = spi.transfer((uint64_t) 0xF0F0F0F0F0F0F0F0);

    /* Send 32 bit */
    print("Sending 32 bits...\n", uart);
    uint32_t data32 = spi.transfer((uint32_t) 0xF0F0F0F0);

    /* Send 16 bit */
    print("Sending 16 bits...\n", uart);
    uint16_t data16 = spi.transfer((uint16_t) 0xF0F0);

    /* Send 8 bit */
    print("Sending 8 bits...\n", uart);
    uint8_t data8 = spi.transfer((uint8_t) 0xF0);

    /* Send array of bytes */
    uint8_t byteArrayTX[] = "SPI Transfer...\n";
    uint8_t byteArrayRX[17];
    spi.transferStream(byteArrayTX, byteArrayRX, 17);
    

    print("===================== TEST DONE =====================\n\n\n", uart);
    
    return;
}


void print(const char *str, UART& device) {
    unsigned int size; 

    for (size = 0; str[size] != '\0'; ++size) { }

    device.loadBufferTX((uint8_t *) str, size);

    return;
}