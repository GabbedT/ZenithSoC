#ifndef SERIAL_IO_H
#define SERIAL_IO_H

#include <inttypes.h>
#include <stdarg.h>
#include <type_traits>

#include "Driver/UART.h"

#include "mmio.h"
#include "platform.h"


class Serial_IO {

public: 

    enum base_e {DEC, BIN, HEX};


private:

    /* Pointers to UART registers */
    static volatile struct UART::uartCtrlStatus_s* volatile status;
    static volatile uint8_t* bufferTX;
    static volatile uint8_t* bufferRX;


public: 

    /**
     * @brief Set the speed of the UART device.
     * 
     * @param baudRate Baud rate for UART.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */ 
    static void init(uint32_t baudRate = 115200, bool parityEnable = true, UART::parityMode_e parityMode = UART::EVEN, 
                     UART::stopBits_e stopBits = UART::STOP1, UART::dataLenght_e dataBits = UART::BIT8);


/****************************************************************/
/*                      OUTPUT METHODS                          */
/****************************************************************/

    /**
     * @brief Display a single character. Write one time to the UART buffer.
     * 
     * @param character Ascii character.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void write(char character); 


    /**
     * @brief Write an array of bytes to the UART buffer until it reaches
     * the null byte.
     * 
     * @param str String containing 1 null character.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void write(const char* str); 


    /**
     * @brief Write an array of bytes to the UART buffer.
     * 
     * @param str String to write.
     * @param size The size of the string.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void write(const char* str, uint32_t size);


    /**
     * @brief Write a number in a specific base system.
     * 
     * @param num Number (8, 16, 32, 64, 128 bits) to display.
     * @param base Number base system.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    template<typename Type> static void write(Type number, base_e base) {
        switch (base) {
            case DEC:
                if (std::is_signed<Type>::value) {
                    writeD(number, true);
                } else {
                    writeD(number, false);
                }
            break;

            case BIN:
                writeB(number);
            break;

            case HEX:
                writeH(number);
            break;
            
            default:
            break;
        }
    };


    /**
     * @brief Write a floating point number with a specified number of
     * digits.
     * 
     * @param num Number to display.
     * @param digits Digits after the dot.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void write(float num, uint32_t digits);


    /**
     * @brief Print a formatted string.
     * 
     * @param format The string with the format characters.
     * @param args Argument list
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void vprintf(const char *format, va_list args);


    /**
     * @brief Print a formatted string.
     * 
     * @param format The string with the format characters.
     * @param args Argument list
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void printf(const char *format, ...);


    /**
     * @brief Print a formatted string. Use %d for decimal, %h or %x for hexadecimal,
     * %b for binary, %.Nf for float where N is the precision, %c for characters.
     * 
     * @param format The string with the format characters.
     * @param ... The data to print corresponding to the format character.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    static void println(const char *format, ...);


    /**
     * @brief Print the decimal value of the number passed as parameter.
     * 
     * @tparam Type From 8 bit to 128 bit data type.
     * 
     * @param number Number to print.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    template<typename Type> static void writeD(Type number, bool isSigned) {
        char buffer[40];

        /* To account for overflows during negations */
        uint64_t number64 = number;

        if (isSigned & (number < 0)) {
            /* If negative the number is in two complement */
            number64 = -number64;

            /* Print a minus */
            write('-');
        } 


        uint32_t size = 0;

        do {
            /* Extract the first digit and convert it to character */
            char digit = (char) ((number64 % 10) + '0');

            /* Shift (in decimal base) to the right by 1 */
            number64 /= 10;

            /* Save the digit into the string and increment the index */
            buffer[size++] = digit;
        } while (number64 != 0);

        /* Reverse the array */
        for (int i = 0; i < size / 2; ++i) {
            uint32_t swap = buffer[size - 1 - i];

            buffer[size - 1 - i] = buffer[i];
            buffer[i] = swap;
        }

        write(buffer, size);

        /* Print EOS character */
        write('\0');
    };


    /**
     * @brief Print the hexadecimal value of the number passed as parameter.
     * 
     * @tparam Type From 8 bit to 128 bit data type.
     * 
     * @param number Number to print.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    template<typename Type> static void writeH(Type number) {
        uint8_t nibble[2];
        char str[sizeof(Type) * 2];
        
        /* Scan each byte */
        for (int i = 0; i < sizeof(Type) * 2; i += 2) {
            /* Take first and second nibble of the current byte */
            nibble[1] = (number & 0xF0) >> 4;
            nibble[0] = number & 0x0F;

            for (int j = 0; j < 2; ++j) {
                if (nibble[j] < 10) {
                    /* 0 - 9 */
                    str[i + j] = '0' + nibble[j];
                } else {
                    /* A - F */
                    str[i + j] = 'A' + nibble[j] - 10;
                }
            }

            number >>= 8;
        }

        /* Reverse the array */
        for (int i = 0; i < sizeof(Type); ++i) {
            char swap = str[(sizeof(Type) * 2) - 1 - i];

            str[(sizeof(Type) * 2) - 1 - i] = str[i];
            str[i] = swap;
        }

        write(str, sizeof(Type) * 2);
        write('\0');
    };


    /**
     * @brief Print the binary value of the number passed as parameter.
     * 
     * @tparam Type From 8 bit to 128 bit data type.
     * 
     * @param number Number to print.
     * 
     * @return The Serial_IO object itself to chain the function call.
     */
    template<typename Type> static void writeB(Type number) {
        char str[sizeof(Type) * 8];

        for (int i = 0; i < sizeof(Type) * 8; ++i) {
            bool lsb = number & 1;
            str[((sizeof(Type) * 8 ) - 1) - i] = lsb ? '1' : '0';

            number >>= 1;
        }

        write(str, sizeof(Type) * 8);
        write('\0');
    };


/****************************************************************/
/*                       INPUT METHODS                          */
/****************************************************************/

    /**
     * @brief Read a string from serial input
     * 
     * @param str Pointer to string buffer
     * @param size Size of the buffer
     * 
     * @return The end string index to the buffer
     */
    static uint32_t readString(char *str, uint32_t size);

    /**
     * @brief Read a string from serial input and convert it into an integer
     * 
     * @param base Base of the input string
     * @param error Pointer to a boolean that saves the status of the conversion
     * 
     * @return The converted integer
     */
    static uint32_t readNumber(base_e base, bool *error);

    /**
     * @brief Read a char from RX buffer
     * 
     * @return The char read
     */
    static char readChar();
};

#endif 