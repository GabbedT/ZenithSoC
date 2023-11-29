#ifndef UART_H
#define UART_H

#include <stdint.h>

#include "../mmio.h"

class UART {

public:

    /* Number of bits in an UART frame */
    enum dataLenght_e { BIT5, BIT6, BIT7, BIT8 };

    /* Number of stop bits in an UART frame */
    enum stopBits_e { STOP1, STOP2 };

    /* Type of parity computed */
    enum parityMode_e { EVEN, ODD };


    /* Interrupt event */
    enum uartEvent_e {
        /* Data received */
        DATA_RX = 1 << 1,

        /* Data transmitted */
        DATA_TX = 1 << 2,

        /* RX buffer full */
        RX_FULL = 1 << 3,

        /* RX buffer full */
        TX_EMPTY = 1 << 4,

        /* RX buffer full */
        PARITY_ERROR = 1 << 5
    };


    /* Status register fields */
    struct uartStatus_s {
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

        /* Clock divider to generate the right baud rate */
        unsigned int clockDivider : 15;
    };


private: 

    /* Base memory address */
    uint32_t* const uartBaseAddress;

    /* UART current status */
    struct uartStatus_s* volatile const status;

    /* RX & TX Buffers */
    uint8_t* volatile const bufferRX;
    uint8_t* volatile const bufferTX;

    /* UART Interrupt event */
    uint32_t* volatile const event;


public: 

    /**
     * @brief Construct a new UART::UART object, disable all functions, the user will
     * later enable certain fuctions to have better control over the device. Set 8N1 
     * configuration with 9600 baudrate
     * 
     * @param uartNumber A number that ranges from 0 to UART_NUMBER - 1, it specifies the system UART.
     */
    UART(uint32_t uartNumber = 0);

    /* Deconstructor */
    ~UART();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    /**
     * @brief Enable / Disable transmission
     * 
     * @param enable 
     * @return UART& to chain the function call
     */
    UART& setModeTX(bool enable);

    /**
     * @brief Enable / Disable reception
     * 
     * @param enable 
     * @return UART& to chain the function call.
     */
    UART& setModeRX(bool enable);

    /**
     * @brief Set communication settings.
     * 
     * @param baudRate UART baudrate for transmission / reception.
     * @param parityEnable Enable transmission and reception of the parity bit.
     * @param parityMode Select the parity computation mode.
     * @param stopBits Select the number of stop bits transmitted / received.
     * @param dataBits Select the number of data bits transmitted / received.
     * @return UART& to chain the function call.
     */
    UART& init(uint32_t baudRate, bool parityEnable, parityMode_e parityMode, stopBits_e stopBits, dataLenght_e dataBits);

    /**
     * @brief Change flow control (CTS/RTS) settings.
     * 
     * @param enable Boolean value to enable / disable flow control.
     * @return UART& to chain the function call.
     */
    UART& setFlowControl(bool enable);

    UART& setInterrupt(uartEvent_e source);

    UART& disableInterrupt(uartEvent_e source);

    /**
     * @brief Set UART baud rate.
     * 
     * @param baudRate Baud rate value.
     * @return UART& to chain the function call.
     */
    UART& setBaudRate(uint32_t baudRate);

    /**
     * @brief Return the value of UART status register.
     * 
     * @return Register status value.
     */
    struct uartStatus_s getStatus();

    /**
     * @brief Return the value of UART event register.
     * 
     * @return Register event value.
     */
    uartEvent_e getEvent();


/*****************************************************************/
/*                         COMMUNICATION                         */
/*****************************************************************/

    /**
     * @brief Sends a byte of data over UART by waiting for the TX buffer to empty and then writing
     * the data to the buffer.
     * 
     * @param data Byte to send.
     * @return The UART object itself to chain the function call.
     */
    UART& sendByte(uint8_t data);

    /**
     * @brief Load several bytes of data into the TX buffer of a UART object. It waits for the buffer to empty before
     * writing each byte.
     * 
     * @param data Pointer to the start of the data packet.
     * @param size Size of the data packet to send.
     * @return The UART object itself to chain the function call.
     */
    UART& loadBufferTX(uint8_t* data, uint32_t size);

    /**
     * @brief Wait until there is data in the receive buffer and then returns the received byte.
     * 
     * @return The byte stored inside the RX buffer.
     */
    uint8_t receiveByte();

    /**
     * @brief Wait until there is data in the receive buffer and then returns the received byte.
     * Warning! The function is blocking, so it won't return until the buffer is filled!
     * 
     * @return The byte stored inside the RX buffer.
     */
    UART& unloadBufferRX (uint8_t* data, uint32_t size);
};

#endif 