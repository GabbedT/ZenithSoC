#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

#include "UART.h"

extern "C" int main(void) {
    UART uart(0);

    /* Exit flag */
    bool error = false;

    uart.init();

    for (int i = 0; i < 4; ++i) {           /* Data bits */
        /* Data bits loop, build data config string to transmit, 
         * config and mask to confront the received string*/
        const uint8_t* dataConfig;
        uint8_t mask;

        switch (i) {
            case 0: 
                dataConfig = (const uint8_t*) "[D8]"; 
                uart.setDataLength(UART::BIT8);

                mask = 0b11111111;
                break;

            case 1: 
                dataConfig = (const uint8_t*) "[D7]"; 
                uart.setDataLength(UART::BIT7);

                mask = 0b01111111;
                break;

            case 2: 
                dataConfig = (const uint8_t*) "[D6]"; 
                uart.setDataLength(UART::BIT6);

                mask = 0b00111111;
                break;

            case 3: 
                dataConfig = (const uint8_t*) "[D5]"; 
                uart.setDataLength(UART::BIT5);

                mask = 0b00011111;
                break;
            
            default:
                break;
        }

        for (int j = 0; j < 3; ++j) {       /* Parity    */
            /* Parity config loop, build parity string to transmit
             * and configure the UART */
            const uint8_t* parityConfig;

            switch (j) {
                case 0: parityConfig = (const uint8_t*) "[PE]"; uart.enableParity(true).setParityMode(UART::EVEN);
                    break;

                case 1: parityConfig = (const uint8_t*) "[PO]"; uart.enableParity(true).setParityMode(UART::ODD);
                    break;

                case 2: parityConfig = (const uint8_t*) "[NP]"; uart.enableParity(false);
                    break;

                default:
                    break;
            }
            
            for (int k = 0; k < 2; ++k) {   /* Stop Bits */
                /* Stop bits config loop, build stop bits string
                 *  to transmit and configure the UART */
                const uint8_t* stopConfig;

                switch (k) {
                    case 0: stopConfig = (const uint8_t*) "[S1]"; uart.setStopBits(UART::STOP1);
                        break;

                    case 1: stopConfig = (const uint8_t*) "[S2]"; uart.setStopBits(UART::STOP2);
                        break;

                    default:
                        break;
                }
                
                /* Transmit the three strings plus newline */
                uart.loadBufferTX((uint8_t*) dataConfig,   4);
                uart.loadBufferTX((uint8_t*) parityConfig, 4);
                uart.loadBufferTX((uint8_t*) stopConfig,   4);
                uart.sendByte('\n');
                
                uint8_t toReceive[13] = {0};

                for (int c = 0; c < 13; ++c) {
                    /* Receive each byte and confront it with the
                     * original ANDed with the mask */
                    uint8_t rxDat = uart.receiveByte();

                    if (c < 4) {
                        error = rxDat != (dataConfig[c] & mask);
                    } else if (c < 8) {
                        error = rxDat != (parityConfig[c - 4] & mask);
                    } else if (c < 12) {
                        error = rxDat != (stopConfig[c - 8] & mask);
                    }

                    if (error) {
                        TEST_FAIL();
                    }
                }
            }
        }
    }

    TEST_PASS();
}