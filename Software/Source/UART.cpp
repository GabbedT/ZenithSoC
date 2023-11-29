#ifndef UART_CPP
#define UART_CPP

#include "../Library/Driver/UART.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <stdint.h>

/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

/**
 * @brief Construct a new UART::UART object, disable all functions, the user will
 * later enable certain fuctions to have better control over the device. Set 8N1 
 * configuration with 9600 baudrate
 * 
 * @param uartNumber A number that ranges from 0 to UART_NUMBER - 1, it specifies the system UART.
 */
UART::UART(uint32_t uartNumber) : 
    /* Initialize the base addres considering that each device has 4 registers */
    uartBaseAddress ( (uint32_t *) (UART_BASE + (uartNumber * 4)) ),

    /* Initialize register addresses based on the base address */
    status          ( (struct uartStatus_s *) (uartBaseAddress)     ),
    bufferTX        ( (uint8_t *) (uartBaseAddress + 1)             ),
    bufferRX        ( (uint8_t *) (uartBaseAddress + 2)             ),
    event           ( (uint32_t * volatile) (uartBaseAddress + 3)   ) {

    
    /* Disable all functions */
    status->flowControl = false;
    status->parityEnable = false;
    status->enableTX = false;
    status->enableRX = false;
    status->interruptEnable = 0;

    /* Set a standard 8N1 */
    status->dataBits = BIT8;
    status->stopBits = STOP1;
    status->parityMode = EVEN;

    UART::setBaudRate(9600);
};


UART::~UART() {
    /* Disable all functions */
    status->flowControl = false;
    status->parityEnable = false;
    status->enableTX = false;
    status->enableRX = false;
    status->interruptEnable = 0;

    /* Set a standard 8N1 */
    status->dataBits = BIT8;
    status->stopBits = STOP1;
    status->parityMode = EVEN;

    UART::setBaudRate(9600);
};


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

/**
 * @brief Enable / Disable transmission.
 * 
 * @param enable Boolean value to enable / disable data transmission.
 * @return The UART object itself to chain the function call.
 */
UART& UART::setModeTX(bool enable) {
    status->enableTX = enable;

    return *this;
};


/**
 * @brief Enable / Disable reception.
 * 
 * @param enable Boolean value to enable / disable data reception.
 * @return The UART object itself to chain the function call.
 */
UART& UART::setModeRX(bool enable) {
    status->enableRX = enable;

    return *this;
};


/**
 * @brief Set communication settings.
 * 
 * @param baudRate UART baudrate for transmission / reception.
 * @param parityEnable Enable transmission and reception of the parity bit.
 * @param parityMode Select the parity computation mode.
 * @param stopBits Select the number of stop bits transmitted / received.
 * @param dataBits Select the number of data bits transmitted / received.
 * @return The UART object itself to chain the function call.
 */
UART& UART::init(uint32_t baudRate, bool parityEnable, parityMode_e parityMode, stopBits_e stopBits, dataLenght_e dataBits) {
    status->parityEnable = parityEnable;
    status->dataBits = dataBits;
    status->stopBits = stopBits;
    status->parityMode = parityMode;

    UART::setBaudRate(baudRate);

    UART::setModeTX(true);
    UART::setModeRX(true);

    return *this;
};



/**
 * @brief Change flow control (CTS/RTS) settings.
 * 
 * @param enable Boolean value to enable / disable flow control.
 * @return The UART object itself to chain the function call.
 */
UART& UART::setFlowControl(bool enable) {
    status->flowControl = enable;

    return *this;
};



/**
 * @brief Set UART baud rate.
 * 
 * @param baudRate Baud rate value.
 * @return The UART object itself to chain the function call.
 */
UART& UART::setBaudRate(uint32_t baudRate) {
    status->clockDivider = (SYSTEM_FREQUENCY / (baudRate * 16)) - 1;

    return *this;
};



/**
 * @brief Enable the interrupt source to generate an interrupt.
 * 
 * @return The UART object itself to chain the function call.
 */
UART& UART::setInterrupt(uartEvent_e interrupt) {
    status->interruptEnable |= interrupt;

    return *this;
};


/**
 * @brief Disable the interrupt source to generate an interrupt.
 * 
 * @return The UART object itself to chain the function call.
 */
UART& UART::disableInterrupt(uartEvent_e interrupt) {
    status->interruptEnable &= ~interrupt;

    return *this;
};


/**
 * @brief Return the value of UART status register.
 * 
 * @return Register status value.
 */
struct UART::uartStatus_s UART::getStatus() {
    return *status;
};


/**
 * @brief Return the value of UART event register.
 * 
 * @return Register event value.
 */
UART::uartEvent_e UART::getEvent() {
    return (uartEvent_e) *event;
};



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
UART& UART::sendByte(uint8_t data) {
    /* Wait until the TX buffer become not full */
    while (status->fullTX) {  }

    /* Write to the TX buffer */
    *bufferTX = data;

    return *this;
};


/**
 * @brief Load several bytes of data into the TX buffer of a UART object. It waits for the buffer to empty before
 * writing each byte.
 * 
 * @param data Pointer to the start of the data packet.
 * @param size Size of the data packet to send.
 * @return The UART object itself to chain the function call.
 */
UART& UART::loadBufferTX(uint8_t* data, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        /* Wait until the TX buffer not full */
        while (status->fullTX) {  }

        /* Write to the TX buffer */
        *bufferTX = data[i];
    }

    return *this;
};



/**
 * @brief Wait until there is data in the receive buffer and then returns the received byte.
 * 
 * @return The byte stored inside the RX buffer.
 */
uint8_t UART::receiveByte() {
    /* Wait until the RX buffer has data stored */
    while (status->emptyRX) {  }

    return *bufferRX;
};



/**
 * @brief Wait until there is data in the receive buffer and then returns the received byte.
 * Warning! The function is blocking, so it won't return until the buffer is filled!
 * 
 * @return The byte stored inside the RX buffer.
 */
UART& UART::unloadBufferRX (uint8_t* data, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        /* Wait until the RX buffer has data stored */
        while (status->emptyRX) {  }

        /* Store byte */
        data[i] = *bufferRX;
    }

    return *this;
}

#endif 