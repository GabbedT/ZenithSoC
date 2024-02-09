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
    
    /* Reset and put PHY on sleep */    
    reset();
    powerDown();

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

    /* Wake up PHY */
    wakeUp();

    /* Configure PHY */
    configure(speed, duplexMode);

    /* Configure auto negotiation */
    setAutoNegotiation(autoNegotiation);

    return *this;
};


Ethernet& Ethernet::enableTestMode(bool enable) {
    volatile union phyControl_s phyControl; 

    /* Set test */
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.loopback = enable;
    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::setMacInterrupt(uint8_t index, bool enable, bool* error) {
    if (index > 3) {
        *error = true;

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
    return macCtrlStatus->idleTX;
};


bool Ethernet::isReceiving() {
    return macCtrlStatus->idleRX;
};


bool Ethernet::isEmptyTX() {
    return macCtrlStatus->emptyTX;
};


bool Ethernet::isFullTX() {
    return macCtrlStatus->fullTX;
};


bool Ethernet::isEmptyRX() {
    return macCtrlStatus->emptyRX;
};


bool Ethernet::isFullRX() {
    return macCtrlStatus->fullRX;
};


struct Ethernet::ethInterrupt_s Ethernet::getInterrupt() {
    /* Get PHY interrupt pending status */
    union phyInterrupt_s phyIntPending;
    phyIntPending.raw = readPHYRegister(_InterruptSource_);

    /* Get MAC interrupt pending status */
    union macInterrupt_s macIntPending;
    macIntPending.raw = *macInterrupt;

    /* Fuse infos */
    struct ethInterrupt_s interrupt;
    interrupt.mac = macIntPending;
    interrupt.phy = phyIntPending;

    return interrupt;
};


bool Ethernet::isLinked() {
    union phyStatus_s status;
    status.raw = readPHYRegister(_BasicStatus_);

    return status.fields.linkStatus;
};


bool Ethernet::energyOn() {
    union phyModeCtrl_s status;
    status.raw = readPHYRegister(_ModeCtrlStatus_);

    return status.fields.energyOn;
};


/*****************************************************************/
/*                      MAC COMMUNICATION                        */
/*****************************************************************/

Ethernet& Ethernet::sendFrame(const uint8_t* packet, uint32_t length, struct Ethernet::macAddr_s destMac, bool* error) {
    /* Check if the length exceed the maximum payload size */
    if (length > MAX_PAYLOAD_LENGTH) {
        *error = true;

        return *this;
    }

    /* Send payload bytes to the TX buffer */
    for (int i = 0; i < length; ++i) {
        *macTxBuffer = packet[i];
    }

    /* Compute the padding bytes */
    uint32_t padding = (length < MIN_PAYLOAD_LENGTH) ? (MIN_PAYLOAD_LENGTH - length) : 0;

    if (padding != 0) {
        for (int i = 0; i < padding; ++i) {
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


Ethernet& Ethernet::receiveFrame(uint8_t* buffer, uint32_t length, bool* error) {
    if (length > MAX_PAYLOAD_LENGTH || length < MIN_PAYLOAD_LENGTH) {
        *error = true;

        return *this;
    }

    /* Fill the buffer */
    for (int i = 0; i < length; ++i) {
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
    volatile union phyControl_s phyControl; 

    /* Read control register and set reset */    
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.reset = true;
    phyControl.fields.powerDown = false;
    phyControl.fields.isolate = true;

    /* Write it again to reset the PHY */
    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::powerDown() {
    volatile union phyControl_s phyControl; 

    /* Put PHY on sleep */
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.powerDown = true;
    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::wakeUp() {
    volatile union phyControl_s phyControl; 

    /* Wake up PHY */
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.powerDown = false;
    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::configure(ethSpeed_e speed, ethDuplex_e duplexMode) {
    volatile union phyControl_s phyControl;

    /* Configure PHY */
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.duplexMode = duplexMode;
    phyControl.fields.speedSelect = speed;

    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::setAutoNegotiation(bool enable) {
    volatile union phyControl_s phyControl;

    /* Setup PHY */
    phyControl.raw = readPHYRegister(_BasicControl_);
    phyControl.fields.enableAutoNegot = enable;
    phyControl.fields.restartAutoNegot = enable;

    writePHYRegister(_BasicControl_, phyControl.raw);

    return *this;
};


Ethernet& Ethernet::setPhyInterrupt(uint8_t index, bool enable, bool* error) {
    if (index > 3) {
        *error = true;

        return *this;
    }

    union phyIntMask_s intMask;
    intMask.raw = readPHYRegister(_InterruptMask_);

    if (enable) {
        intMask.raw |= 1 << index;
    } else {
        intMask.raw &= ~(1 << index);
    }

    writePHYRegister(_InterruptMask_, intMask.raw);

    return *this;
};

#endif