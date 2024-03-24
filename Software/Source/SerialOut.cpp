#ifndef SERIAL_OUT_CPP
#define SERIAL_OUT_CPP

#include "../Library/SerialOut.h"

#include <inttypes.h>


/****************************************************************/
/*                             DATA                             */
/****************************************************************/

volatile struct UART::uartCtrlStatus_s* volatile SerialOut::status;
volatile uint8_t* SerialOut::bufferTX;


/****************************************************************/
/*                         INITIALIZER                          */
/****************************************************************/

void SerialOut::init(uint32_t baudRate, bool parityEnable, UART::parityMode_e parityMode, UART::stopBits_e stopBits, UART::dataLenght_e dataBits) {
    status = (volatile UART::uartCtrlStatus_s *) (UART_BASE + (DEBUG_UART * 4));
    bufferTX = (uint8_t *) (status + 1);
        
    status->parityEnable = parityEnable;
    status->dataBits = dataBits;
    status->stopBits = stopBits;
    status->parityMode = parityMode;

    /* Set baud rate */
    status->clockDivider = (SYSTEM_FREQUENCY / (baudRate * 16)) - 1;

    /* Disable data reception and enable transmission */
    status->enableRX = false;
    status->enableTX = true;
};


/****************************************************************/
/*                        SIMPLE WRITE                          */
/****************************************************************/

void SerialOut::write(char character) {
    /* Wait until the TX buffer become not full */
    while (status->fullTX) {  }

    /* Write to the TX buffer */
    *bufferTX = (uint8_t) character;
}; 


void SerialOut::write(const char* str) {
    uint32_t size; 

    /* Compute string size */
    for (size = 0; str[size] != '\0'; ++size) {  }

    write(str, size);
};


void SerialOut::write(const char* str, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        /* Wait until the TX buffer not full */
        while (status->fullTX) {  }

        /* Write to the TX buffer */
        *bufferTX = str[i];
    }
};


/****************************************************************/
/*                      FORMATTED WRITE                         */
/****************************************************************/

void SerialOut::vprintf(const char *format, va_list args) {
    for (int i = 0; format[i] != '\0'; ++i) {
        if (format[i] == '%') {
            ++i;

            switch (format[i]) {
                /* Decimal */
                case 'd': {
                    int32_t value = va_arg(args, int32_t);

                    writeD(value, true);
                    break;
                }

                /* Decimal 64 bit */
                case 'l': {
                    int64_t value = va_arg(args, int64_t);

                    writeD(value, true);
                    break;
                }

                /* Unsigned decimal */
                case 'u': {
                    uint32_t value = va_arg(args, uint32_t);

                    writeD(value, false);
                    break;
                }

                /* Hexadecimal */
                case 'x': {
                    i += 1;

                    switch (format[i]) {
                        /* Write byte */
                        case 'b': {
                            uint8_t value = va_arg(args, uint32_t);

                            writeH(value);
                            break;
                        }

                        /* Write halfword */
                        case 'h': {
                            uint16_t value = va_arg(args, uint32_t);

                            writeH(value);
                            break;
                        }

                        /* Write double word */
                        case 'd': {
                            uint64_t value = va_arg(args, uint64_t);

                            writeH(value);
                            break;
                        }

                        /* Write word */
                        default: {
                            uint32_t value = va_arg(args, int);

                            writeH(value);

                            i -= 1;
                            break;
                        }
                    }

                    break;
                }

                /* Binary */
                case 'b': {
                    i += 1;

                    switch (format[i]) {
                        /* Write byte */
                        case 'b': {
                            uint8_t value = va_arg(args, uint32_t);

                            writeB(value);
                            break;
                        }

                        /* Write halfword */
                        case 'h': {
                            uint16_t value = va_arg(args, uint32_t);

                            writeB(value);
                            break;
                        }

                        /* Write double word */
                        case 'd': {
                            uint64_t value = va_arg(args, uint64_t);

                            writeB(value);
                            break;
                        }

                        /* Write word */
                        default: {
                            uint32_t value = va_arg(args, int);

                            writeB(value);
                                
                            i -= 1;
                            break;
                        }
                    }

                    break;
                }
                    
                default: 
                    write("\n\nFORMAT ERROR!\n\n");
                break;
            }
        } else {
            write(format[i]);
        }
    }

    va_end(args);
};


void SerialOut::println(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    write('\n');
    write('\0');
};


void SerialOut::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    write('\0');
};

#endif 