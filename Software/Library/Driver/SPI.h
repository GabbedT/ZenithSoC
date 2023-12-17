#ifndef SPI_H
#define SPI_H

#include <inttypes.h>

#include "../mmio.h"

class SPI {

public:

    /* Bit sent first */
    enum bitOrder_e { MSB_FIRST, LSB_FIRST };

    /* SPI working mode, it's a 2 bit value that specifies CPOL and CPHA */
    enum spiMode_e { MODE0, MODE1, MODE2, MODE3 };

    /* SPI status register fields */
    typedef struct spiStatus_s {
        /* Buffer status */
        unsigned int emptyRX : 1;
        unsigned int fullRX : 1;
        unsigned int emptyTX : 1;
        unsigned int fullTX : 1;

        /* SPI is transmitting / receiving */
        unsigned int idle : 1;
        
        /* Enable interrupt for each event */
        unsigned int interruptEnable : 1;

        /* SPI configuration */
        unsigned int cpol : 1;
        unsigned int cpha : 1;
        unsigned int bitOrder : 1;

        /* Clock divider to generate the right SPI clock */
        unsigned int clockDivider : 16;

        /* Pad to 32 bits */
        unsigned int padding : 6;
    };

    

private:

    /* Base memory address */
    uint32_t* const spiBaseAddress;

    /* SPI status register */
    volatile struct spiStatus_s* const status;

    /* RX & TX Buffers */
    volatile uint8_t* const bufferRX;
    volatile uint8_t* const bufferTX;

    /* SPI Interrupt event */
    volatile uint32_t* const event;

    /* Select the slave to start communicating with */
    volatile uint32_t* const slaveSelect;


public: 

    SPI(uint32_t spiNumber = 0);

    /* Deconstructor */
    ~SPI();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    /**
     * @brief Set communication settings and enable full duplex communication.
     * 
     * @param clkFreq Frequency of SCLK pin in Hz.
     * @param spiMode Specify the values of CPOL and CPHA.
     * @param bitOrder Specify the order of the bit transmission.
     * 
     * @return SPI& to chain the function call.
     */
    SPI& init(uint32_t clkFreq, spiMode_e spiMode, bitOrder_e bitOrder);

    /**
     * @brief Set SPI pin SCLK frequency.
     * 
     * @param clkFreq Desired frequency in Hz that is less than the system clock.
     * @return SPI& to chain the function call.
     */
    SPI& setFrequency(uint32_t clkFreq);

    /**
     * @brief Set the order of transmission / reception.
     * 
     * @param bitOrder MSB or LSB first.
     * @return SPI& to chain the function call
     */
    SPI& setBitOrder(bitOrder_e bitOrder);

    /**
     * @brief Set the SPI communication mode (modify CPOL and CPHA).
     * 
     * @param spiMode MSB or LSB first.
     * @return SPI& to chain the function call
     */
    SPI& setMode(spiMode_e spiMode);

    /**
     * @brief Enable / Disable interrupt generation on transaction end.
     * 
     * @return SPI& to chain the function call.
     */
    SPI& enableInterrupt(bool enable);

    /**
     * @brief Return the value of SPI status register.
     * 
     * @return Pointer to the status register.
     */
    volatile struct spiStatus_s* getStatus();

    /**
     * @brief Check if the SPI device is idle or is engaged in a transaction.
     * 
     * @return A boolean value to express the idle status 
     */
    bool isIdle();


/*****************************************************************/
/*                         COMMUNICATION                         */
/*****************************************************************/

    /**
     * @brief Primitive type transaction: send a number of bytes based on the primitive type passed, wait for the end of the transaction 
     * and retrieve the received byte. Warning! The function is blocking, so it won't return until the transaction ends!
     * 
     * @param data TX data to transmit.
     * 
     * @return The data received.
     */
    template<typename Type> Type transfer(Type data);

    /**
     * @brief Generic transaction: send a stream of bytes, wait for the end of the transaction and retrieve the received bytes.
     * Warning! The function is blocking, so it won't return until the transaction ends!
     * 
     * @param txBuf Buffer containing data to send.
     * @param rxBuf Buffer that will contain the read data at the end of the transaction.
     * @param size Transaction size.
     * 
     * @return The SPI object itself to chain the function call.
     */
    SPI& transferStream(uint8_t* txBuf, uint8_t* rxBuf, uint32_t size);


private: 

    /**
     * @brief Extract bytes from a primitive type.
     * 
     * @param data Primitive type data.
     * @param bytes Array of bytes returned.
     */
    template<typename T> void extractBytes(T data, uint8_t* bytes);

    /**
     * @brief Compact the bytes passed as parameter into a primitiva data type.
     * 
     * @param data Primitive type data.
     * @param bytes Array of bytes that needs to be compacted.
     * @param size Array size.
     */
    template<typename T> void compressBytes(T* data, uint8_t* bytes);

    /**
     * @brief Load several bytes of data into the TX buffer of a SPI object. It waits for the buffer to empty before
     * writing each byte. Used mainly when it's needed to send a stream of bytes without receiving anything.
     * 
     * @param data Pointer to the start of the data packet.
     * @param size Size of the data packet to send.
     * 
     * @return The SPI object itself to chain the function call.
     */
    void loadBufferTX(uint8_t* data, uint32_t size);

    /**
     * @brief Wait until there is data in the receive buffer and then returns the received byte.
     * 
     * @return The byte stored inside the RX buffer.
     */
    void unloadBufferRX (uint8_t* data, uint32_t size);
};  

#endif 