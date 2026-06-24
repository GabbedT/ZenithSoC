#ifndef SD_CPP
#define SD_CPP

#include "../lib/driver/SD.h"
#include "../lib/mmio.h"
#include "../lib/platform.h"

#include <stdint.h>


SD::SD() :
    /* Initialize the base address considering that each device has 4 registers of 1 byte */
    sdBaseAddress ( (uint32_t * volatile) SD_BASE ),

    /* Initialize register addresses based on the base address */
    control         ( (struct sdControl_s *)          sdBaseAddress      ),
    status          ( (struct sdStatus_s *)          (sdBaseAddress + 1) ),
    commandNumber   ( (uint32_t *)                   (sdBaseAddress + 2) ),
    commandArgument ( (uint32_t *)                   (sdBaseAddress + 3) ),
    event           ( (struct sdInterruptStatus_s *) (sdBaseAddress + 4) ),
    responseBuffer  ( (uint8_t *)                    (sdBaseAddress + 5) ),
    txBuffer        ( (uint32_t *)                   (sdBaseAddress + 6) ),
    rxBuffer        ( (uint32_t *)                   (sdBaseAddress + 7) ) {

    /* Disable Interrupts */
    control->interruptEnable = 0;
    
    /* Set default configuration */
    control->busWidth = BUS_NARROW;
    control->clockSpeed = CLK_400KHZ;
    
    /* Disable FSMs */
    control->enableSD = 0;
};


SD::~SD() {
    /* Disable Interrupts */
    control->interruptEnable = 0;

    /* Set default configuration */
    control->busWidth = BUS_NARROW;
    control->clockSpeed = CLK_400KHZ;

    /* Disable FSMs */
    control->enableSD = 0;
};


SD& SD::init(clockSpeed_e speed, busWidth_e width, uint8_t* cmd8_response, bool& isHighCapacity, errorType_e& error) {
    uint8_t resp[17] = {0};
    error = SD::NO_ERROR;

    /* ----------- Card Preparation ----------- */

    // reset();
    // control->enableSD = true;

    // /* Need ~74 SD clock cycles before starting CMD0 */
    // volatile int sink = 0;

    // for (int i = 0; i < 10000; ++i) { 
    //     ++sink;
    // }
    

    /* CMD0: GO_IDLE_STATE -> put the card in idle mode */
    sendCommand(0, 0);
    flushResponseBuffer();


    /* CMD8: SEND_IF_COND -> check voltage range (2.7–3.6V) and SD version >= 2.0 */
    sendCommand(8, 0x1AA);
    readResponse(cmd8_response, error);

    if (error == SD::CMD_CRC_ERR) {
        return *this;
    }

    /* Reset since we don't care about timeouts */
    error = SD::NO_ERROR;

    /* ----------- Initialization Loop ----------- */

    bool cardReady = false;
    uint32_t attempts = 0;

    do {
        /* CMD55: APP_CMD */
        sendCommand(55, 0);
        flushResponseBuffer();

        /* ACMD41: SEND_OP_COND -> request operating conditions + HCS bit */
        sendCommand(41, 0x40300000);
        readResponse(resp, error);

        if (error == SD::CMD_CRC_ERR) {
            return *this;
        }
        
        /* Reset since we don't care about timeouts */
        error = SD::NO_ERROR;

        /* Busy flag is bit 39,  */
        cardReady = (resp[1] & 0x80) != 0;

        ++attempts;
    } while (!cardReady && attempts < 200);

    /* Exit init */
    if (!cardReady) {
        error = SD::CARD_ERR;
        return *this;
    }


    /* ----------- OCR Register Reading ----------- */

    /* CMD58: READ_OCR -> get the official OCR register */
    sendCommand(58, 0);
    readResponse(resp, error);

    if (error == SD::NO_ERROR) {
        /* Store OCR in class variable, pack 4 bytes into uint32_t */
        this->cardOCR = (((uint32_t) resp[1]) << 24) | (((uint32_t) resp[2]) << 16) |
                        (((uint32_t) resp[3]) << 8)  | ((uint32_t) resp[4]);

        /* CCS (Card Capacity Status) is bit 30 of OCR which tell us if the card is SDSC vs SDHC/SDXC */
        isHighCapacity = (resp[1] & 0x40) != 0;
    } else {
        /* Assume SDSC if OCR not available */
        isHighCapacity = false;

        this->cardOCR = 0;
    }

    if (error == SD::CMD_CRC_ERR) {
        return *this;
    }

    error = SD::NO_ERROR;

    flushResponseBuffer();


    /* ----------- Clock Configuration ----------- */

    /* Switch to requested clock speed */
    control->clockSpeed = speed; 


    /* ----------- CID Register Reading ----------- */

    /* CMD2 ALL_SEND_CID: Get the card identification number */
    sendCommand(2, 0);
    readResponse(resp, error);

    if (error == SD::NO_ERROR) {
        /* Store CID in class variable (15 bytes = 120 bits) */
        uint8_t* cidBytes = reinterpret_cast<uint8_t*>(this->cardCID);

        /* Copy every byte of the response except the first and the last which contain
         * command index and CRC, so byte 1 to 15 of a 128 bit response */
        for (int i = 0; i < 16; i++) {
            cidBytes[15 - i] = resp[i + 1];
        }
    } else {
        this->cardCID[0] = this->cardCID[1] = 0;
    }

    if (error == SD::CMD_CRC_ERR) {
        return *this;
    }

    error = SD::NO_ERROR;


    /* ----------- RCA Register Reading ----------- */

    /* CMD3 SEND_RELATIVE_ADDR: Assign and get RCA (Relative Card Address) */
    sendCommand(3, 0);
    readResponse(resp, error);


    if (error == SD::NO_ERROR) {
        /* Extract RCA from bits [39:24] of response */
        this->cardRCA = (((uint32_t) resp[1]) << 8) | resp[2];
    }

    if (error == SD::CMD_CRC_ERR) {
        return *this;
    }

    error = SD::NO_ERROR;

    flushResponseBuffer();


    /* ----------- CSD Register Reading ----------- */

    /* CMD9: SEND_CSD -> Get Card Specific Data */
    sendCommand(9, static_cast<uint32_t>(cardRCA) << 16);
    readResponse(resp, error);


    if (error == SD::NO_ERROR) {
        /* Store CSD in class variable (15 bytes = 120 bits) */
        uint8_t* csdBytes = reinterpret_cast<uint8_t*>(this->cardCSD);

        /* Copy every byte of the response except the first and the last which contain
         * command index and CRC, so byte 1 to 15 of a 128 bit response */
        for (int i = 0; i < 16; i++) {
            csdBytes[15 - i] = resp[i + 1];
        }
    } else {
        this->cardCSD[0] = this->cardCSD[1] = 0;
    }

    if (error == SD::CMD_CRC_ERR) {
        return *this;
    }

    error = SD::NO_ERROR;

    flushResponseBuffer();


    /* ----------- Card Selection / Transfer State ----------- */

    /* CMD7 SELECT_CARD: Move card to TRANSFER state */
    sendCommand(7, ((uint32_t) this->cardRCA) << 16);
    flushResponseBuffer();



    /* ----------- Block Length Configuration ----------- */

    /* CMD16 SET_BLOCKLEN: force 512B block length (only needed for SDSC cards) */
    if (!isHighCapacity) {
        sendCommand(16, 512);
        flushResponseBuffer();
    }



    /* ----------- Bus width configuration ----------- */

    /* CMD55: APP_CMD */
    sendCommand(55, ((uint32_t) cardRCA) << 16);
    flushResponseBuffer();

    /* ACMD6: SET_BUS_WIDTH */
    sendCommand(6, (width == BUS_WIDE) ? 0x02 : 0x00);



    /* ----------- SCR Register Reading ----------- */

    /* CMD55: APP_CMD for ACMD51 */
    sendCommand(55, ((uint32_t) cardRCA) << 16);
    flushResponseBuffer();
    
    /* ACMD51: SEND_SCR */
    sendCommand(51, 0);
    flushResponseBuffer();
    
    
    /* Read SCR as two 32-bit words directly */
    uint32_t* scrPtr = reinterpret_cast<uint32_t*>(&this->cardSCR);
    

    uint32_t timeout = 0;

    /* Store first half */
    while (status->rxBufferEmpty) { 
        if (++timeout > 100000) {
            error = SD::DAT_TIMEOUT;
            return *this;
        }
    }

    timeout = 0;
    scrPtr[1] = *rxBuffer;
        
    /* Store second half */
    while (status->rxBufferEmpty) { 
        if (++timeout > 100000) {
            error = SD::DAT_TIMEOUT;
            return *this;
       
        }
    }

    timeout = 0;
    scrPtr[0] = *rxBuffer;
        
    flushDataBuffer();


    /* ----------- Final Setup ----------- */

    /* Wait until both command and data FSMs are idle */
    while (!status->cmdIdle || !status->dataIdle) { 
        if (++timeout > 100000) {
            error = SD::DAT_TIMEOUT;
            return *this;
        }
    }

    /* Flush any leftover bytes in response/data FIFOs */
    flushResponseBuffer();
    flushDataBuffer();

    /* Update host controller configuration */
    control->busWidth = width;

    return *this;
};



SD& SD::reset() {
    /* Assert reset signal */
    control->resetCard = true;

    /* Wait to deassert automatically */
    while (control->resetCard) {  }

    return *this;
};


SD& SD::setClockSpeed(clockSpeed_e speed) {
    /* Set the clock speed */
    control->clockSpeed = speed;

    return *this;
};


SD& SD::setBusWidth(busWidth_e width, errorType_e& error) {
    /* Send CMD55: APP_CMD */
    sendCommand(55, (uint32_t) SD::cardRCA << 16);
    flushResponseBuffer();

    /* Send ACMD6 */
    sendCommand(6, (width == BUS_WIDE) ? 0x2 : 0x0);

    /* Wait for both FSMs to be idle */
    while (!status->cmdIdle || !status->dataIdle) {  }

    if (status->cmdTimeout) {
        error = SD::CMD_TIMEOUT;
        status->cmdTimeout = false;

        flushResponseBuffer(); flushDataBuffer();

        return *this;
    }

    if (status->cmdCRC_Error) {
        error = SD::CMD_CRC_ERR;
        status->cmdCRC_Error = false;

        flushResponseBuffer(); flushDataBuffer();

        return *this;
    }

    if (status->dataTimeout) {
        error = SD::DAT_TIMEOUT;
        status->cmdTimeout = false;

        flushResponseBuffer(); flushDataBuffer();

        return *this;
    }

    if (status->dataCRC_Error) {
        error = SD::DAT_CRC_ERR;
        status->cmdCRC_Error = false;

        flushResponseBuffer(); flushDataBuffer();

        return *this;
    }

    if (status->dataError) {
        error = SD::DAT_ERR;
        status->dataError = false;

        flushResponseBuffer(); flushDataBuffer();

        return *this;
    }

    /* Flush buffers to delete data */
    flushResponseBuffer(); flushDataBuffer();

    control->busWidth = width;

    return *this;
};


SD& SD::setInterruptEnable(bool enable, uint32_t position) {
    if (enable) {
        /* Set */
        control->interruptEnable |= (1 << position);
    } else {
        /* Clear*/
        control->interruptEnable &= ~(1 << position);
    }

    return *this;
};


SD& SD::sendCommand(uint32_t cmdNumber, uint32_t argument) {
    /* Wait for both FSMs to be idle */
    while (!status->cmdIdle) {  }

    /* Set the command number and argument */
    *commandNumber = cmdNumber;
    *commandArgument = argument;

    /* Send Command */
    control->sendCommand = true;

    return *this;
};


SD& SD::readResponse(uint8_t* responseBuffer, errorType_e& error) {
    /* Wait for both FSMs to be idle */
    while (!status->cmdIdle) {  }

    if (status->cmdTimeout) {
        error = SD::CMD_TIMEOUT;
        status->cmdTimeout = false;

        flushResponseBuffer();

        return *this;
    }

    if (status->cmdCRC_Error) {
        error = SD::CMD_CRC_ERR;
        status->cmdCRC_Error = false;

        flushResponseBuffer();

        return *this;
    }

    /* Pointer to array */
    unsigned int i = 0;

    while (!status->respBufferEmpty) {
        /* Read response byte */
        responseBuffer[i++] = *this->responseBuffer;

        if (i >= 16) break;
    }

    return *this;
};


SD& SD::readBlock(uint32_t blockAddress, uint32_t* blockRead, uint8_t* responseBuffer, errorType_e& error) {
    /* Issue read block command */
    sendCommand(17, blockAddress);

    /* Read response */
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        return *this;
    }


    /* Wait for data FSM to be idle */
    while (!status->dataIdle) {  }

    if (status->dataTimeout) {
        error = SD::DAT_TIMEOUT;
        status->dataTimeout = false;

        flushDataBuffer();

        return *this;
    }

    /* Pointer to array */
    unsigned int i = 0;

    while (!status->rxBufferEmpty) {
        /* Read data word */
        blockRead[i++] = *rxBuffer;

        if (i >= SD::MAX_32_BIT_BLOCK) break;
    }

    return *this;
};


SD& SD::writeBlock(uint32_t blockAddress, uint32_t* blockWrite, uint8_t* responseBuffer,  errorType_e& error) {
    /* Since CMD FSM will automatically start data FSM if it detects CMD25 fill first the buffer 
     * to avoiding reading into an empty buffer */
    int i = 0;

    while (i < MAX_32_BIT_BLOCK && !status->txBufferFull) {
        /* Write buffer */
        *txBuffer = blockWrite[i++];
    }

    
    /* Issue write block command */
    sendCommand(24, blockAddress);

    /* Read response */
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        control->flushTX = true;
        
        return *this;
    }

    /* Wait for data FSM to finish */
    while (!status->dataIdle) {  }


    if (status->dataTimeout) {
        error = SD::DAT_TIMEOUT;
        status->dataTimeout = false;

        return *this;
    }

    if (status->dataCRC_Error) {
        error = SD::DAT_CRC_ERR;
        status->dataCRC_Error = false;

        return *this;
    }

    return *this;
};


SD& SD::readBurst(uint32_t baseAddress, uint32_t burstLength, uint32_t* burstRead, uint8_t* responseBuffer,  errorType_e& error) {
    /* Issue read burst command */
    sendCommand(18, baseAddress);

    /* Read response */
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        return *this;
    }

    for (int i = 0; i < burstLength; ++i) {
        for (int j = 0; j < MAX_32_BIT_BLOCK; ++j) {
            /* Wait until buffer is not empty */
            while (status->rxBufferEmpty) {  }

            burstRead[i * MAX_32_BIT_BLOCK + j] = *rxBuffer;
        }

        /* Error Checks */
        if (status->dataCRC_Error) {
            error = SD::DAT_CRC_ERR;
            status->dataCRC_Error = false;

            break;
        }
    }

    /* Stop burst */
    sendCommand(12, 0);

    /* Flush useless data */
    flushResponseBuffer();
    flushDataBuffer();

    return *this;
};


SD& SD::writeBurst(uint32_t baseAddress, uint32_t burstLength, uint32_t* burstWrite, uint8_t* responseBuffer,  errorType_e& error) {
    /* Since CMD FSM will automatically start data FSM if it detects CMD25 fill first the buffer 
     * to avoiding reading into an empty buffer */
    for (int j = 0; j < MAX_32_BIT_BLOCK; ++j) {
        *txBuffer = burstWrite[j];
    }
    
    /* Issue write burst command */
    sendCommand(25, baseAddress);

    /* Read response */
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        control->flushTX = true;

        return *this;
    }

    for (int i = 1; i < burstLength; ++i) {
        /* Wait until all data has been consumed */
        while (!status->txBufferEmpty) {  
            /* Error Checks */
            if (status->dataError) {
                error = SD::DAT_ERR;
                status->dataError = false;

                break;
            }

            if (status->dataCRC_Error) {
                error = SD::DAT_CRC_ERR;
                status->dataCRC_Error = false;

                break;
            }
        }

        if (error != SD::NO_ERROR) {
            control->flushTX = true;

            /* Stop burst */
            sendCommand(12, 0);

            /* Flush useless data */
            flushResponseBuffer();
            flushDataBuffer();

            return *this;
        }

        for (int j = 0; j < MAX_32_BIT_BLOCK; ++j) {
            *txBuffer = burstWrite[i * MAX_32_BIT_BLOCK + j];
        }
    }

    /* Stop burst */
    sendCommand(12, 0);

    /* Flush useless data */
    flushResponseBuffer();
    flushDataBuffer();

    return *this;
};


SD& SD::flushResponseBuffer() {
    /* Wait for both FSMs to be idle */
    while (!status->cmdIdle || !status->dataIdle) {  }

    while (!status->respBufferEmpty) {
        /* Read response byte */
        uint8_t temp = *responseBuffer;
    }

    return *this;
};


SD& SD::flushDataBuffer() {
    /* Wait for both FSMs to be idle */
    while (!status->cmdIdle || !status->dataIdle) {  }

    while (!status->rxBufferEmpty) {
        /* Read data */
        uint32_t temp = *rxBuffer;
    }

    return *this;
};


SD& SD::readCID(uint8_t* cidBuffer, errorType_e& error) {
    sendCommand(10, SD::cardRCA << 16);
    
    uint8_t responseBuffer[17];
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        return *this;
    }
    
    /* Copy 16 bytes of CID (skip command index) */
    for (int i = 0; i < 15; i++) {
        cidBuffer[i] = responseBuffer[i + 1];
    }
    
    return *this;
};

SD& SD::readCSD(uint8_t* csdBuffer, errorType_e& error) {
    /* CMD9: SEND_CSD */
    sendCommand(9, cardRCA << 16);
    
    uint8_t responseBuffer[17];
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        return *this;
    }

    /* Copy 15 bytes of CSD (skip command index) */
    for (int i = 0; i < 15; i++) {
        csdBuffer[i] = responseBuffer[i + 1];
    }
    
    return *this;
};

SD& SD::readSCR(uint8_t* scrBuffer, errorType_e& error) {
    /* Select card */
    sendCommand(7, cardRCA << 16);
    flushResponseBuffer();
    
    /* APP_CMD */
    sendCommand(55, cardRCA << 16);
    flushResponseBuffer();
    
    /* ACMD51: SEND_SCR */
    sendCommand(51, 0);
    flushResponseBuffer();
    
    /* Read 8 bytes from RX buffer */
    for (int i = 0; i < 2; i++) {
        while (status->rxBufferEmpty && !status->dataTimeout) { }

        if (status->dataTimeout) {
            error = SD::DAT_TIMEOUT;
            status->dataTimeout = false;

            flushDataBuffer();

            return *this;
        }

        uint32_t word = *rxBuffer;
        
        /* Convert to big-endian bytes */
        scrBuffer[i * 4 + 0] = (word >> 24) & 0xFF;
        scrBuffer[i * 4 + 1] = (word >> 16) & 0xFF;
        scrBuffer[i * 4 + 2] = (word >> 8) & 0xFF;
        scrBuffer[i * 4 + 3] = word & 0xFF;
    }
    
    flushDataBuffer();

    return *this;
};

SD& SD::readOCR(uint8_t* ocrBuffer, errorType_e& error) {
    /* CMD58: READ_OCR */
    sendCommand(58, 0);
    
    /* Get R3 Response */
    uint8_t responseBuffer[6];
    readResponse(responseBuffer, error);

    if (error != SD::NO_ERROR) {
        return *this;
    }
    
    /* OCR is in bytes 1-4 of response */
    for (int i = 0; i < 4; i++) {
        ocrBuffer[i] = responseBuffer[i + 1];
    }
    
    return *this;
};

bool SD::isCardInserted() {
    return status->cardDetected;
};

uint64_t SD::getCardCapacity() {
    /* Big endian, so cardCSD[0] holds bits 127...64 */
    uint8_t csdVersion = (cardCSD[0] >> 62) & 0x3;

    if (csdVersion == 0) {
        /* CSD v1.0 (SDSC) */

        /* Extract block length */
        uint32_t blkLen = (cardCSD[0] >> 16) & 0xF;

        /* Extract card capacity */
        uint32_t cSize = ((cardCSD[0] & 0x3FF) << 2) | ((cardCSD[1] >> 62) & 0x3);
        uint32_t cSizeMult = (cardCSD[1] >> 47) & 0x7;

        return ((uint64_t) (cSize + 1)) << (cSizeMult + 2 + blkLen);
    } else {
        /* CSD v2.0 (SDHC / SDXC) */
        
        uint32_t cSize = ((cardCSD[0] & 0x3F) << 16) | ((cardCSD[1] >> 48) & 0xFFFF);

        return ((uint64_t) (cSize + 1)) * 512 * 1024;
    }
};

uint32_t SD::getCardStatus(errorType_e& error) {
    uint8_t resp[6];

    /* SEND_STATUS: argument is in RCA upper 16 bits */
    sendCommand(13, (uint32_t) cardRCA << 16);
    readResponse(resp, error);

    if (error != SD::NO_ERROR) {
        return 0;
    }


    
}

#endif 