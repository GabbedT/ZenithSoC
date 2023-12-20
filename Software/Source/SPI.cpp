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
    status      ( (struct spiStatus_s *) (spiBaseAddress) ),
    bufferTX    ( (uint8_t *) (spiBaseAddress + 1)        ),
    bufferRX    ( (uint8_t *) (spiBaseAddress + 2)        ),
    event       ( (uint32_t *) (spiBaseAddress + 3)       ),
    slaveSelect ( (uint32_t *) (spiBaseAddress + 4)       ) {

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


SPI& SPI::connect(uint32_t slaveIndex) {
    /* Wait until a previous transaction ends */
    while (!status->idle) {  }

    /* Clear slaves */
    *slaveSelect &= 0;

    /* Set new slave */
    *slaveSelect |= 1 << slaveIndex;

    return *this;
}


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


inline bool SPI::isIdle() {
    return status->idle;
};


/*****************************************************************/
/*                         COMMUNICATION                         */
/*****************************************************************/


SPI& SPI::transferStream(uint8_t* txBuf, uint8_t* rxBuf, uint32_t size) {
    /* Write to TX buffer the entire array */
    loadBufferTX(txBuf, size);

    /* Wait until the transaction ends */
    while (!status->idle) {  }

    /* Copy the received data into the array */
    unloadBufferRX(rxBuf, size);

    return *this;
}


/***********************************************************/
/*                         UTILITY                         */
/***********************************************************/

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