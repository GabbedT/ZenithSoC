#ifndef SERIAL_IO_CPP
#define SERIAL_IO_CPP

#include "../Library/Serial_IO.h"

#include <inttypes.h>


/****************************************************************/
/*                             DATA                             */
/****************************************************************/

volatile struct UART::uartCtrlStatus_s* volatile Serial_IO::status;
volatile uint8_t* Serial_IO::bufferTX;
volatile uint8_t* Serial_IO::bufferRX;


/****************************************************************/
/*                         INITIALIZER                          */
/****************************************************************/

void Serial_IO::init(uint32_t baudRate, bool parityEnable, UART::parityMode_e parityMode, UART::stopBits_e stopBits, UART::dataLenght_e dataBits) {
    status = (volatile UART::uartCtrlStatus_s *) (UART_BASE + (DEBUG_UART * 4));
    bufferTX = (uint8_t *) (status + 1);
    bufferRX = (uint8_t *) (status + 2);
        
    status->parityEnable = parityEnable;
    status->dataBits = dataBits;
    status->stopBits = stopBits;
    status->parityMode = parityMode;

    /* Set baud rate */
    status->clockDivider = (SYSTEM_FREQUENCY / (baudRate * 16)) - 1;

    /* Disable data reception and enable transmission */
    status->enableRX = true;
    status->enableTX = true;
};


/****************************************************************/
/*                        SIMPLE WRITE                          */
/****************************************************************/

void Serial_IO::write(char character) {
    /* Wait until the TX buffer become not full */
    while (status->fullTX) {  }

    /* Write to the TX buffer */
    *bufferTX = (uint8_t) character;
}; 


void Serial_IO::write(const char* str) {
    uint32_t size; 

    /* Compute string size */
    for (size = 0; str[size] != '\0'; ++size) {  }

    write(str, size);
};


void Serial_IO::write(const char* str, uint32_t size) {
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

void Serial_IO::vprintf(const char *format, va_list args) {
    uint32_t precision = 6;

    for (uint32_t i = 0; format[i] != '\0'; ++i) {
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
                            uint8_t value = static_cast<uint8_t>(va_arg(args, uint32_t));

                            writeH(value);
                            break;
                        }

                        /* Write halfword */
                        case 'h': {
                            uint16_t value = static_cast<uint16_t>(va_arg(args, uint32_t));

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
                            uint8_t value = static_cast<uint8_t>(va_arg(args, uint32_t));

                            writeB(value);
                            break;
                        }

                        /* Write halfword */
                        case 'h': {
                            uint16_t value = static_cast<uint16_t>(va_arg(args, uint32_t));

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

                /* Floating point precision */
                case '.': {
                    i += 1;

                    /* Extract precision */
                    precision = 0;

                    while (format[i] >= '0' && format[i] <= '9') {
                        precision = precision * 10 + (format[i] - '0');
                        i += 1;
                    }
                    
                    if (format[i] == 'f') {
                        /* Get the value passed as argument */
                        float value = static_cast<float>(va_arg(args, double));
                        
                        /* Print minus sign */
                        if (value < 0) {
                            write('-');
                            value = -value;
                        }
                        
                        /* Extract integer and fractional part */
                        int integerPart = static_cast<int>(value);
                        float fractionalPart = value - integerPart;

                        writeD(integerPart, false);
                        write('.');

                        for (int i = 0; i < precision; ++i) {
                            /* Bring the most significant digit of the fractional part into the integer part
                            * EX: 0.15 => 1.50 */
                            fractionalPart *= 10;

                            /* Extract and print */
                            int digit = static_cast<int>(fractionalPart);
                            write('0' + digit);

                            /* Remove the integer part and repeat */
                            fractionalPart -= digit;
                        }

                        break;
                    } else {
                        write("[PRINT] FORMAT ERROR!\n");

                        break;
                    }
                }
                
                    
                default: 
                    write("[PRINT] FORMAT ERROR!\n");
                break;
            }
        } else {
            write(format[i]);
        }
    }

    va_end(args);
};

void Serial_IO::println(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    write('\n');
    write('\0');
};


void Serial_IO::printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    vprintf(format, args);

    va_end(args);

    write('\0');
};


/****************************************************************/
/*                       INPUT METHODS                          */
/****************************************************************/

uint32_t Serial_IO::readString(char *str, uint32_t size) {
    uint32_t count = 0;

    /* Size - 1 to accomodate the last character "\0" */
    while (count < size - 1) {
        /* Wait until RX buffer is not empty */
        while (status->emptyRX);

        /* Read RX buffer */
        char received = (char)(*bufferRX);
        
        if (received == '\n' || received == '\0') {
            /* Exit if the user press ENTER */
            break;
        } else {
            /* Save the character received */
            str[count++] = received;
        }
    }

    /* End Of String terminator */
    str[count] = '\0';

    return count;
};


uint32_t Serial_IO::readNumber(Serial_IO::base_e base, bool *error) {
    /* Create a buffer with the maximum number of digit plus the terminator */
    char digit[33]; 

    uint32_t index = readString(digit, 33);
    uint32_t number = 0;

    /* Take account for the sign in the decimal case */
    bool isNegative = false;

    /* Iterate on the read string */
    for (uint32_t i = 0; i < index; ++i) {
        switch (base) {
            case DEC: {
                if (digit[i] == '-') {
                    isNegative = true;

                    /* Next iteration */
                    continue;
                }
                
                if (digit[i] >= '0' && digit[i] <= '9') {
                    /* Each iteration it shifts the most significant digit on the left, then
                     * it adds the new character as the least significand digit */
                    number = number * 10 + (digit[i] - '0');
                } else {
                    /* Error: illegal character */
                    *error = true;
                }

                break;
            }

            case BIN: {
                if (digit[i] == '0' || digit[i] == '1') {
                    /* Simply pack the binary number */
                    number = (number << 1) | (digit[i] - '0');
                } else {
                    /* Error: illegal character */
                    *error = true;
                }

                break;
            }

            case HEX: {
                uint32_t converter; 

                /* Simply pack the binary number */
                if (digit[i] >= '0' && digit[i] <= '9') {
                    converter = '0';
                } else if (digit[i] >= 'A' && digit[i] <= 'F') {
                    converter = 'A' + 10;
                } else if (digit[i] >= 'a' && digit[i] <= 'f') {
                    converter = 'a' + 10;
                } else {
                    /* Error: illegal character */
                    *error = true;
                }

                number = (number << 4) | (digit[i] - number);

                break;
            }

            default:
            break;
        }
    }

    return isNegative ? -number : number;
};


char Serial_IO::readChar() {
    /* Wait until RX buffer is not empty */
    while (status->emptyRX);

    /* Read RX buffer */
    return (char)(*bufferRX);
};


#endif 