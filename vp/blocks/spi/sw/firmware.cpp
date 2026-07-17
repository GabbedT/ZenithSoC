#include "platform.h"
#include "SPI.h"

#include <stdint.h>
#include <stdbool.h>

extern "C" int main(void) {
    SPI spi(0);
    SPI::spiError_e spiError = SPI::NO_ERROR;

    /* Exit flag */
    bool error = false;

    
    /* Test frequency configuration and error reporting. */
    spiError = SPI::NO_ERROR;
    spi.setFrequency(0, &spiError);
    if (spiError != SPI::ILLEGAL_CLOCK) {
        vp_println("[ERROR] \'ILLEGAL_CLOCK\' error not generated");
        TEST_FAIL();
    }

    spiError = SPI::NO_ERROR;
    spi.setFrequency(50000001, &spiError);
    if (spiError != SPI::ILLEGAL_CLOCK) {
        vp_println("[ERROR] \'ILLEGAL_CLOCK\' error not generated");
        TEST_FAIL();
    }


    /* Test an invalid slave index before testing each available slave. */
    spiError = SPI::NO_ERROR;
    spi.connect(4, &spiError);
    if (spiError != SPI::INDEX_OUT_OF_RANGE) {
        vp_println("[ERROR] \'INDEX_OUT_OF_RANGE\' error not generated");
        TEST_FAIL();
    }


    /* Test every slave with every SPI mode and both bit orders. */
    for (int i = 0; i < 4; ++i) {               /* Slave */
        for (int j = 0; j < 4; ++j) {           /* Mode  */
            for (int k = 0; k < 2; ++k) {       /* Bit order */
                SPI::bitOrder_e bitOrder;

                switch (k) {
                    case 0: bitOrder = SPI::MSB_FIRST;
                        break;

                    case 1: bitOrder = SPI::LSB_FIRST;
                        break;

                    default: bitOrder = SPI::MSB_FIRST;
                        break;
                }

                spiError = SPI::NO_ERROR;
                spi.init(1000000, (SPI::spiMode_e) j, bitOrder, &spiError)
                   .connect(i, &spiError);

                while ((spi.status->cpol != ((j >> 1) & 1)) ||
                       (spi.status->cpha != (j & 1)) ||
                       (spi.status->bitOrder != bitOrder)) { }
                
                if (spiError != SPI::NO_ERROR) {
                    vp_println("[ERROR] Initialization or Connection error");
                    TEST_FAIL();
                }

                /* Check if config was actually written */
                error = (spi.status->cpol != ((j >> 1) & 1)) ||
                        (spi.status->cpha != (j & 1)) ||
                        (spi.status->bitOrder != bitOrder);

                if (error) {
                    vp_println("[ERROR] Configuration not applied");
                    TEST_FAIL();
                }

                /* Transmit configuration, receive slave ID */
                uint8_t txByte = 0x80 | (i << 4) | (j << 1) | k;
                uint8_t rxByte = spi.exchange<uint8_t>(txByte);

                if (rxByte != (uint8_t) i) {
                    vp_print("[ERROR] Slave ");
                    vp_print_hex(i);
                    vp_print(", mode ");
                    vp_print_hex(j);
                    vp_print(", bit order ");
                    vp_print_hex(k);
                    vp_print(" returned ");
                    vp_println_hex(rxByte);
                    TEST_FAIL();
                }

                /* Stream test, RX[0] is slave ID. Every following
                 * byte is the previous byte sent on MOSI. */
                uint8_t txBuffer[5] = {
                    (uint8_t) (0x10 | i),
                    (uint8_t) (0x20 | j),
                    (uint8_t) (0x30 | k),
                    0xA5,
                    0x5A
                };
                uint8_t rxBuffer[5] = {0};

                spi.exchangeStream(txBuffer, rxBuffer, sizeof(txBuffer));
                
                /* Check slave ID */
                if (rxBuffer[0] != (uint8_t) i) {
                    vp_println("[ERROR] Slave ID mismatch on stream test");
                    TEST_FAIL();
                }
                
                /* Check that TX data is equal to RX data (except Slave ID) */
                for (int l = 1; l < sizeof(txBuffer); ++l) {
                    if (rxBuffer[l] != txBuffer[l - 1]) {
                        vp_println("[ERROR] RX data differs from TX data");
                        TEST_FAIL();
                    }
                }
            }
        }
    }

    spiError = SPI::NO_ERROR;
    uint8_t received[2] = {0};

    /* Non-blocking transfer and retrieve test. */
    spi.setMode(SPI::MODE1)
       .setBitOrder(SPI::MSB_FIRST)
       .connect(3, &spiError);

    if (spiError != SPI::NO_ERROR) {
        vp_println("[ERROR] Connection to Slave 3 test failed");
        TEST_FAIL();
    }

    /* Send data, don't wait for reception */
    spi.transfer<uint16_t>(0xB64D);

    /* Do stuff */
    while (!spi.isIdle()) { }

    /* Retrieve bytes */
    spi.retrieve(received, sizeof(received), &spiError);

    if (spiError != SPI::NO_ERROR) {
        vp_println("[ERROR] Retrieve error");
        TEST_FAIL();
    }

    if ((received[0] != 3) || (received[1] != 0x4D)) {
        vp_println("[ERROR] Retrieved bytes mismatch");
        TEST_FAIL();
    }


    /* Read an empty buffer to check error */
    spi.retrieve(received, 1, &spiError);

    if (spiError != SPI::RECEIVE_SIZE_ERROR) {
        vp_println("[ERROR] \'RECEIVE_SIZE_ERROR\' error not generated");
        TEST_FAIL();
    }

    TEST_PASS();
    return 0;
}
