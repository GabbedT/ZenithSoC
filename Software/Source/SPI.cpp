#ifndef SPI_CPP
#define SPI_CPP

#include "../Library/Driver/SPI.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <stdint.h>

/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

SPI::SPI(uint32_t spiNumber) : 
    /* Initialize the base addres considering that each device has 4 registers */
    spiBaseAddress ( (uint32_t *) (SPI_BASE + (spiNumber * 4)) ),

    /* Initialize register addresses based on the base address */
    status      ( (struct spiStatus_s *) (spiNumber) ),
    bufferTX    ( (uint8_t *) (spiNumber + 1)        ),
    bufferRX    ( (uint8_t *) (spiNumber + 2)        ),
    event       ( (uint32_t *) (spiNumber + 3)       ),
    slaveSelect ( (uint32_t *) (spiNumber + 4)       ) {

    /* Disable operations */
    status->interruptEnable = false;

    /* Set operation mode */
    status->bitOrder = MSB_FIRST;
    setMode(MODE0);
    setFrequency(1'000'000); 
};


SPI::~SPI() {
    /* Disable operations */
    status->interruptEnable = false;

    /* Set operation mode */
    status->bitOrder = MSB_FIRST;
    setMode(MODE0);
    setFrequency(1'000'000); 
};


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

SPI& SPI::init(uint32_t clkFreq, spiMode_e spiMode, bitOrder_e bitOrder) {
    status->bitOrder = bitOrder;

    setMode(spiMode);
    setFrequency(clkFreq); 

    return *this;
};


SPI& SPI::setFrequency(uint32_t clkFreq) {
    status->clockDivider = (SYSTEM_FREQUENCY / clkFreq) - 1;

    return *this;
};


SPI& SPI::setBitOrder(bitOrder_e bitOrder) {
    status->bitOrder = bitOrder;

    return *this;
};


SPI& SPI::setMode(spiMode_e spiMode) {
    /* CPHA = MODE[0] */
    status->cpha = (spiMode & 1);

    /* CPOL = MODE[1] */
    status->cpol = (spiMode & 2) >> 1;

    return *this;
};


SPI& SPI::enableInterrupt(bool enable) {
    status->interruptEnable = enable;

    return *this;
};


volatile struct SPI::spiStatus_s* SPI::getStatus() {
    return status;
};



/*****************************************************************/
/*                         COMMUNICATION                         */
/*****************************************************************/

template<typename Type> Type SPI::transfer(Type data) {
    /* Store the parameter bytes */
    uint8_t txBytes[sizeof(Type)]; 
    extractBytes(data, txBytes);

    for (int i = 0; i < sizeof(Type); ++i) {
        /* Wait until the TX buffer become not full */
        while (status->fullTX) {  }

        /* Write to the TX buffer */
        *bufferTX = txBytes[i];
    }

    /* Wait untile the transaction ends */
    while (status->idle) {  }

    uint8_t rxBytes[sizeof(Type)]; Type dataRX; 

    /* Read the received data and compress it */
    unloadBufferRX(rxBytes, sizeof(Type));
    compressBytes(&dataRX, rxBytes);

    return *dataRX;
};


SPI& SPI::transferStream(uint8_t* txBuf, uint8_t* rxBuf, uint32_t size) {
    /* Write to TX buffer the entire array */
    loadBufferTX(txBuf, size);

    /* Wait untile the transaction ends */
    while (status->idle) {  }

    /* Copy the received data into the array */
    unloadBufferRX(rxBuf, size);

    return *this;
}


/***********************************************************/
/*                         UTILITY                         */
/***********************************************************/

template<typename T> void SPI::extractBytes(T data, uint8_t* bytes) {
    for (int i = 0; i < sizeof(T); ++i) {
        /* Extract the bytes from the N bit parameter:
         *
         * 1: Shift the data by a byte multiplied by the iteration number
         *    to bring the i-th byte at the bit position 7:0.
         * 2: Mask the remaining data except for the first byte to extract 
         *    the i-th byte. */
        bytes[i] = (uint8_t) ((data >> (i * 8)) & 0xFF);
    }
};


template<typename T> void compressBytes(T* data, uint8_t* bytes) {
    /* Initialize the data value */
    *data = 0;

    for (int i = 0; i < sizeof(T); ++i) {
        /* Compress the bytes from the array:
         * 
         * 1: Shift the byte into it's position based on the iteration number.
         * 2: OR the shifted byte with the parameter. */
        *data |= bytes[i] << (i * 8);
    }
};


void SPI::unloadBufferRX (uint8_t* data, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        /* Store byte */
        data[i] = *bufferRX;
    }
};


void SPI::loadBufferTX(uint8_t* data, uint32_t size) {
    for (int i = 0; i < size; ++i) {
        /* Wait until the TX buffer not full */
        while (status->fullTX) {  }

        /* Write to the TX buffer */
        *bufferTX = data[i];
    }
};

#endif 