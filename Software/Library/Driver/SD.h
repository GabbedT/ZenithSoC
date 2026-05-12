#ifndef SD_H
#define SD_H

#include <inttypes.h>

#include "../mmio.h"
#include "../platform.h"

class SD {

public: 

    /* Max number of elements in a 32/8 bit buffer */
    const uint32_t MAX_32_BIT_BLOCK = 128;
    const uint32_t MAX_8_BIT_BLOCK = 512;

    /* Response length */
    enum responseType_e { R48, R136 };
    
    /* Physical bus width */
    enum busWidth_e { BUS_NARROW, BUS_WIDE };
    
    /* SD Clock speed */
    enum clockSpeed_e { CLK_400KHZ, CLK_25MHZ };


    /* Configuration Register bitmap */
    struct sdControl_s {
        /* Enable interrupt sources */
        unsigned int interruptEnable : 9;

        /* Set to send the command with the relative argument and number, 
         * this bit resets by itself */
        unsigned int sendCommand : 1;

        /* Set the bus width (4 vs 1 bit), a sequence of commands must 
         * be issued before changing this configuration */
        unsigned int busWidth : 1;

        /* Set the clock speed, a sequence of commands must 
         * be issued before changing this configuration */
        unsigned int clockSpeed : 1;

        /* Flush TX Buffer after a fault */
        unsigned int flushTX : 1;

        /* Enable the FSMs */
        unsigned int enableSD : 1;

        /* Send a physical reset signal of 1ms to the SD card */
        unsigned int resetCard : 1;

        unsigned int padding : 18;
    };

    struct sdStatus_s {
        /* Data FSM status */
        unsigned int dataTimeout : 1;
        unsigned int dataCRC_Error : 1;
        unsigned int dataIdle : 1;
        
        /* CMD FSM status */
        unsigned int cmdResponseType : 1; 
        unsigned int cmdTimeout : 1;
        unsigned int cmdCRC_Error : 1;
        unsigned int cmdIdle : 1;

        /* Response buffer status */
        unsigned int respBufferFull : 1;
        unsigned int respBufferEmpty : 1;
        
        /* RX Buffer status */
        unsigned int rxBufferFull : 1;
        unsigned int rxBufferEmpty : 1;

        /* TX Buffer status */
        unsigned int txBufferFull : 1;
        unsigned int txBufferEmpty : 1;

        /* Data token received after data operation */
        unsigned int dataToken : 8;

        /* Card is inserted in the slot */
        unsigned int cardDetected : 1;

        unsigned int padding : 10;
    };

    struct sdInterruptStatus_s {
        /* Data FSM status */
        unsigned int dataTimeout : 1;
        unsigned int dataCRC_Error : 1;
        unsigned int dataDone : 1;
        
        /* CMD FSM status */
        unsigned int cmdTimeout : 1;
        unsigned int cmdCRC_Error : 1;
        unsigned int cmdDone : 1;

        /* RX Buffer status */
        unsigned int rxBufferFull : 1;

        /* TX Buffer status */
        unsigned int txBufferEmpty : 1;

        /* Card is inserted in the slot */
        unsigned int cardDetected : 1;

        unsigned int padding : 23;
    };


public:

    /* Relative Card Address */
    uint16_t cardRCA = 0;

    /* */
    uint32_t cardOCR = 0;

    uint64_t cardSCR = 0;

    uint64_t cardCSD[2] = {0};
    uint64_t cardCID[2] = {0};

    /* Base memory address */
    uint32_t* const sdBaseAddress;

    /* Configuration register */
    volatile struct sdControl_s* volatile const control;

    /* SD Status register */
    volatile struct sdStatus_s* volatile const status;

    /* Command info registers */
    volatile uint32_t* const commandNumber;
    volatile uint32_t* const commandArgument;

    /* Interrupt event register*/
    volatile struct sdInterruptStatus_s* volatile const event;

    /* Command Response buffer */
    volatile uint8_t* const responseBuffer;

    /* TX and RX buffers */
    volatile uint32_t* const rxBuffer;
    volatile uint32_t* const txBuffer;


/*****************************************************************/
/*                          CONSTRUCTOR                          */
/*****************************************************************/

    SD();

    ~SD();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    SD& init(clockSpeed_e speed, busWidth_e width, uint8_t* cmd8_response, bool& timeout, bool& crcError, bool& isHighCapacity);

    SD& reset();

    SD& setClockSpeed(clockSpeed_e speed);

    SD& setBusWidth(busWidth_e width);

    SD& setInterruptEnable(bool enable, uint32_t position);


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/

    inline uint8_t getDataError();

    inline uint8_t getCommandError();

    inline bool isCardInserted();


/*****************************************************************/
/*                         DATA TRANSFER                         */
/*****************************************************************/

    SD& sendCommand(uint32_t commandNumber, uint32_t argument);

    SD& sendCommand(uint32_t commandNumber, uint32_t argument, uint8_t* responseBuffer, bool& timeout);

    /* Get all the bytes from the response, from the byte that has been received first (MSB) (which is placed on the first position of the buffer)
     * to the last */
    SD& readResponse(uint8_t* responseBuffer, bool& timeout, bool& crcError);

    SD& readBlock(uint32_t blockAddress, uint32_t* blockRead, uint8_t* responseBuffer, bool& timeout, bool& crcError);

    SD& writeBlock(uint32_t blockAddress, uint32_t* blockRead, uint8_t* responseBuffer, uint8_t& responseToken, bool& timeout, bool& crcError);

    SD& readBurst(uint32_t baseAddress, uint32_t burstLength, uint32_t* burstRead, uint8_t* responseBuffer, bool& timeout, bool& crcError);

    SD& writeBurst(uint32_t baseAddress, uint32_t burstLength, uint32_t* burstWrite, uint8_t* responseBuffer, uint8_t* tokenBuffer, bool& timeout, bool& crcError);

    SD& flushResponseBuffer();

    SD& flushDataBuffer();

/*****************************************************************/
/*                           CARD INFO                           */
/*****************************************************************/

    SD& readCID(uint8_t* cidBuffer, bool& timeout, bool& crcError);    // 16 bytes

    SD& readCSD(uint8_t* csdBuffer, bool& timeout, bool& crcError);    // 16 bytes  

    SD& readSCR(uint8_t* scrBuffer);    // 8 bytes

    SD& readOCR(uint8_t* ocrBuffer, bool& timeout, bool& crcError);    // 4 bytes

};

#endif 