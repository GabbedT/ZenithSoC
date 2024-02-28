#ifndef ETHERNET_CPP
#define ETHERNET_CPP

#include "../Library/Driver/Ethernet.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <inttypes.h>

/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

Ethernet::Ethernet() : 
    ethBaseAddress ( (uint32_t *) (ETHERNET_BASE)              ),
    macCtrlStatus  ( (macCtrlStatus_s *) (ethBaseAddress + 32) ),
    macTxPktDesc   ( (uint64_t *) (ethBaseAddress + 33)        ),
    macTxBuffer    ( (uint8_t *) (ethBaseAddress + 35)         ),
    macRxPktDesc   ( (uint64_t *) (ethBaseAddress + 36)        ),        
    macRxBuffer    ( (uint8_t *) (ethBaseAddress + 38)         ),
    macInterrupt   ( (uint32_t *) (ethBaseAddress + 39)        ) {  
    
    /* Reset and Wake up PHY */    
    reset(); 
    wakeUp();

    /* Disable interrupts */
    macCtrlStatus->interruptEnable = 0;
    *macInterrupt = 0;
};


Ethernet::~Ethernet() {  
    /* Reset and put PHY on sleep */
    reset();
    powerDown();

    /* Disable interrupts */
    macCtrlStatus->interruptEnable = 0;
    *macInterrupt = 0;
};


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

Ethernet& Ethernet::init(ethSpeed_e speed, ethDuplex_e duplexMode, bool autoNegotiation) {
    /* Configure MAC speed */
    macCtrlStatus->speed = speed; 

    /* Configure auto negotiation */
    setAutoNegotiation(autoNegotiation);

    /* Configure PHY */
    configure(speed, duplexMode);

    return *this;
};


Ethernet& Ethernet::setMacInterrupt(uint8_t index, bool enable, Ethernet::ethError_e* error) {
    if (index > 3) {
        *error = INDEX_OUT_OF_RANGE;

        return *this;
    }

    if (enable) {
        macCtrlStatus->interruptEnable |= 1 << index;
    } else {
        macCtrlStatus->interruptEnable &= ~(1 << index);
    }

    return *this;
};


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/

bool Ethernet::isSending() {
    return !macCtrlStatus->idleTX;
};


bool Ethernet::isReceiving() {
    return !macCtrlStatus->idleRX;
};


bool Ethernet::isEmptyPayloadTX() {
    return macCtrlStatus->payloadEmptyTX;
};


bool Ethernet::isFullPayloadTX() {
    return macCtrlStatus->payloadFullTX;
};


bool Ethernet::isEmptyPayloadRX() {
    return macCtrlStatus->payloadEmptyRX;
};


bool Ethernet::isFullPayloadRX() {
    return macCtrlStatus->payloadFullRX;
};

bool Ethernet::isEmptyPacketTX() {
    return macCtrlStatus->packetEmptyTX;
};


bool Ethernet::isFullPacketTX() {
    return macCtrlStatus->packetFullTX;
};


bool Ethernet::isEmptyPacketRX() {
    return macCtrlStatus->packetEmptyRX;
};


bool Ethernet::isFullPacketRX() {
    return macCtrlStatus->packetFullRX;
};


struct Ethernet::ethInterrupt_s Ethernet::getInterrupt() {
    /* Get PHY interrupt pending status */
    union phyInterrupt_s phyIntPending; phyIntPending.raw = 0; 
    phyIntPending.raw = readPHYRegister(_InterruptSource_);

    /* Get MAC interrupt pending status */
    union macInterrupt_s macIntPending; macIntPending.raw = 0; 
    macIntPending.raw = *macInterrupt;

    /* Fuse infos */
    struct ethInterrupt_s interrupt;
    interrupt.mac = macIntPending;
    interrupt.phy = phyIntPending;

    return interrupt;
};


bool Ethernet::isLinked() {
    union phyStatus_s status; status.raw = 0; 
    status.raw = readPHYRegister(_BasicStatus_);

    return status.fields.linkStatus;
};


bool Ethernet::energyOn() {
    union phyModeCtrl_s status; status.raw = 0; 
    status.raw = readPHYRegister(_ModeCtrlStatus_);

    return status.fields.energyOn;
};


Ethernet::ethChannel_e Ethernet::getChannel() {
    union phyCtrlStatusInd_s status; status.raw = 0; 
    status.raw = readPHYRegister(_CtrlStatusIndication_);

    return (Ethernet::ethChannel_e) status.fields.channelSelect;
};


uint16_t Ethernet::getErrorCount() {
    uint16_t status = 0;
    status = readPHYRegister(_SymbolErrorCnt_);

    return status;
};


/*****************************************************************/
/*                      MAC COMMUNICATION                        */
/*****************************************************************/

Ethernet& Ethernet::sendFrame(const uint8_t* packet, uint32_t length, struct Ethernet::macAddr_s destMac, Ethernet::ethError_e* error) {
    /* Check if the length exceed the maximum payload size */
    if (length > MAX_PAYLOAD_LENGTH) {
        *error = LENGTH_EXCEEDED;

        return *this;
    }

    /* Check if buffers are full */
    if (isFullPacketTX()) {
        *error = TX_FULL;

        return *this;
    }
    
    /* Wait in case of a buffer full */
    while (isFullPacketTX()) {  }

    /* Send payload bytes to the TX buffer */
    for (int i = 0; i < length; ++i) {
        *macTxBuffer = packet[i];
    }

    /* Compute the padding bytes */
    uint32_t padding = (length < MIN_PAYLOAD_LENGTH) ? (MIN_PAYLOAD_LENGTH - length) : 0;

    if (padding != 0) {
        for (int i = 0; i < padding; ++i) {
            while (isFullPayloadTX()) {  }
            *macTxBuffer = 0;
        }
    }

    union tempDescriptor_u {
        struct fields {
            /* Payload length */
            unsigned int length : 16;

            /* MAC Address */
            uint8_t macByte0;
            uint8_t macByte1;
            uint8_t macByte2;
            uint8_t macByte3;
            uint8_t macByte4;
            uint8_t macByte5;
        } fields;

        uint64_t raw;
    };

    /* Create the 8-byte descriptor */
    union tempDescriptor_u descriptor;

    descriptor.fields.length = length + padding;
    descriptor.fields.macByte0 = destMac.byte[0];
    descriptor.fields.macByte1 = destMac.byte[1];
    descriptor.fields.macByte2 = destMac.byte[2];
    descriptor.fields.macByte3 = destMac.byte[3];
    descriptor.fields.macByte4 = destMac.byte[4];
    descriptor.fields.macByte5 = destMac.byte[5];

    /* Send the descriptor the the descriptor buffer in the MAC */
    *macTxPktDesc = descriptor.raw;

    return *this;
};


Ethernet& Ethernet::receiveFrame(uint8_t* buffer, uint32_t length, Ethernet::ethError_e* error) {
    if (length > MAX_PAYLOAD_LENGTH) {
        *error = LENGTH_EXCEEDED;

        return *this;
    } else if (length < MIN_PAYLOAD_LENGTH) {
        *error = LENGTH_SUBCEEDED;

        return *this;
    }

    /* Fill the buffer */
    for (int i = 0; i < length + 4; ++i) {
        buffer[i] = *macRxBuffer;
    }

    return *this;
};


union Ethernet::macDescriptor_s Ethernet::getRxDescriptor() {
    union macDescriptor_s descriptor;
    descriptor.raw = *macRxPktDesc;

    return descriptor;
};



/*****************************************************************/
/*                      PHY CONFIGURATION                        */
/*****************************************************************/

Ethernet& Ethernet::reset() {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0; 

    /* Read control register and set reset */    
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.reset = true;

    /* Write it again to reset the PHY */
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::powerDown() {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0;

    /* The Auto-Negotiation Enable must be cleared before setting the Power Down */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.enableAutoNegot = false;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    /* Put PHY on sleep */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.powerDown = true;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::wakeUp() {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0;

    /* Wake up PHY */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.powerDown = false;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::configure(ethSpeed_e speed, ethDuplex_e duplexMode) {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0;

    /* Configure PHY */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.speedSelect = speed;
    phyRegister.fields.duplexMode = duplexMode;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::setTestMode(bool enable) {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0;

    /* Set test */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.loopback = enable;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::setHeartbeatTest(bool enable) {
    volatile union phyCtrlStatusInd_s phyRegister; phyRegister.raw = 0;

    /* Set heartbeat */
    phyRegister.raw = readPHYRegister(_CtrlStatusIndication_);
    phyRegister.fields.sqeTest = !enable;
    writePHYRegister(_CtrlStatusIndication_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::setAutoNegotiation(bool enable) {
    volatile union phyControl_s phyRegister; phyRegister.raw = 0;

    /* Setup PHY */
    phyRegister.raw = readPHYRegister(_BasicControl_);
    phyRegister.fields.enableAutoNegot = enable;
    phyRegister.fields.restartAutoNegot = enable;
    writePHYRegister(_BasicControl_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::setChannel(Ethernet::ethChannel_e channel) {
    volatile union phyCtrlStatusInd_s phyRegister; phyRegister.raw = 0;

    /* Clear AMDIXCTRL field and set CH_SELECT field */
    phyRegister.raw = readPHYRegister(_CtrlStatusIndication_);
    phyRegister.fields.channelSelect = channel;
    phyRegister.fields.autoMDIX = true;
    writePHYRegister(_CtrlStatusIndication_, phyRegister.raw);

    return *this;
};


Ethernet& Ethernet::setPhyInterrupt(uint8_t index, bool enable, Ethernet::ethError_e* error) {
    if (index > 6) {
        *error = INDEX_OUT_OF_RANGE;

        return *this;
    }

    union phyIntMask_s intMask;
    intMask.raw = readPHYRegister(_InterruptMask_);

    if (enable) {
        intMask.fields.mask |= 1 << index;
    } else {
        intMask.fields.mask &= ~(1 << index);
    }

    writePHYRegister(_InterruptMask_, intMask.raw);

    return *this;
};

#endif