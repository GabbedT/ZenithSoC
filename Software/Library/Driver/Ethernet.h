#ifndef ETHERNET_H
#define ETHERNET_H

#include <inttypes.h>

#include "../mmio.h"


class Ethernet {

public:

    /* Bandwidth */
    enum ethSpeed_e { MBPS10, MBPS100 };

    /* MAC Address */
    struct macAddr_s { uint8_t byte[6]; };

    /* Interrupt event */
    enum ethEvent_e {
        /* External PHY interrupt */
        PHY_INTERRUPT = 1 << 0,

        /* Transmission ended */
        TX_DONE = 1 << 1,

        /* Packed received */
        RX_DONE = 1 << 2,

        /* Error during reception */
        RX_ERROR = 1 << 3
    };

    /* PHY Speed indication */
    enum phySpeed_e { 
        HFDPLX_10BASE_T = 0b001, 
        FLDPLX_10BASE_T = 0b101, 
        HFDPLX_100BASE_TX = 0b010, 
        FLDPLX_100BASE_TX = 0b110 
    };


    /* Status register fields */
    struct macCtrlStatus_s {
        /* Buffer status */
        unsigned int emptyRX : 1;
        unsigned int fullRX : 1;
        unsigned int emptyTX : 1;
        unsigned int fullTX : 1;

        /* TX and RX FSMs status */
        unsigned int idleRX : 1;
        unsigned int idleTX : 1;

        /* Enable interrupt for each event */
        unsigned int interruptEnable : 4;     

        /* Select MAC speed */   
        unsigned int speed : 1;        
    };


    /* Packet descriptor register */
    union macDescriptor_s {
        struct fields {
            /* Payload length */
            unsigned int length : 16;

            /* MAC Address */
            uint8_t address[6];
        } fields;

        uint64_t raw;
    };


    /* Interrupt register fields */
    union macInterrupt_s {
        struct fields {
            unsigned int phyInterrupt : 1;
            unsigned int txDone : 1;
            unsigned int rxDone : 1;
            unsigned int rxError : 1;

            unsigned int padding : 28;
        } fields;

        uint32_t raw;
    };


    enum ethDuplex_e { HALF_DUPLEX, FULL_DUPLEX }; 

    /* PHY Basic Control Register fields */
    union phyControl_s {
        struct fields {
            uint8_t reserved : 8; 

            /* Duplex select (Ignored if Auto-Negotiation is enabled) */
            ethDuplex_e duplexMode : 1;
            
            /* Restart auto-negotiate process (self clearing) */
            unsigned int restartAutoNegot : 1;

            /* Electrical isolation of PHY from the RMI */
            unsigned int isolate : 1;

            /* Put PHY to sleep (Auto-Negotiation Enable must be cleared) */
            unsigned int powerDown : 1;

            /* Enable auto negotiation with the other part */
            unsigned int enableAutoNegot : 1;

            /* Bandwidth select */
            ethSpeed_e speedSelect : 1;

            /* Loopback mode to test (TX is redirected to RX) */
            unsigned int loopback : 1;

            /* Software reset (self clearing) */
            unsigned int reset : 1;
        } fields;

        uint16_t raw;
    };


    /* PHY Basic Status Register fields */
    union phyStatus_s {
        struct fields {
            /* Supports or not extended capabilities registers */
            unsigned int extendedCap : 1;

            /* Jabber condition detected */
            unsigned int jabberDetect : 1;

            /* Ethernet link status */
            unsigned int linkStatus : 1;

            /* Able to perform auto-negotiation function */
            unsigned int autoNegotAbility : 1;

            /* Remote fault condition detected */        
            unsigned int remoteFault : 1;

            /* Auto-negotiate process completed */
            unsigned int autoNegotCompleted : 1;

            unsigned int reserved : 2;

            /* Extended status information in register */
            unsigned int extendedStatus : 1;

            unsigned int HalfDuplex_100BASE_T2 : 1;
            unsigned int FullDuplex_100BASE_T2 : 1;
            unsigned int HalfDuplex_10BASE_T : 1;
            unsigned int FullDuplex_10BASE_T : 1;
            unsigned int FullDuplex_100BASE_TX : 1;
            unsigned int HalfDuplex_100BASE_TX : 1;

            unsigned int _100BASE_T4 : 1;
        } fields;

        uint16_t raw;
    };


    /* PHY Identifier 2 */
    union phyIdentifier2_s {
        struct fields {
            unsigned int revisionNumber : 4;
            unsigned int modelNumber : 6;
            unsigned int phyID : 6;
        };

        uint16_t raw;
    };
    


    /* Auto Negotiation Advertisement Register */
    union phyAutoNegAdv_s {
        struct fields {
            unsigned int selectorField : 5;

            unsigned int capabilities : 5;
            
            /* PAUSE supported by partner station */
            unsigned int pauseOperation : 1;

            unsigned int reserved : 2;

            /* Remote fault detected */
            unsigned int remoteFault : 1;

            /* Link code word received from partner */
            unsigned int acknowledge : 1;

            /* Next page capable */
            unsigned int nextPage : 1;
        } fields;

        uint16_t raw;
    };


    /* Auto Negotiation Expansion Register */
    union phyAutoNegExp_s {
        struct fields {
            /* Link partner has auto-negotiation ability */
            unsigned int linkAutoNegAble : 1;

            /* New page received */
            unsigned int pageReceived : 1;

            /* Local device has next page ability */
            unsigned int nextPageAble : 1;

            /* Link partner has next page ability */
            unsigned int linkNextPageAble : 1;

            /* Fault detected by parallel detection logic */
            unsigned int pDetectFault : 1;

            unsigned int reserved : 11;
        } fields;

        uint16_t raw;
    };


    /* Mode and Control Register */
    union phyModeCtrl_s {
        struct fields {
            unsigned int reserved0 : 1;

            /* Indicates whether energy is detected */
            unsigned int energyOn : 1;

            unsigned int reserved1 : 4;

            /* Alternate interrupt system enabled */
            unsigned int altInt : 1;

            unsigned int reserved2 : 2;

            /* Far loopback mode is enabled */
            unsigned int farLoopBack : 1;

            unsigned int reserved3 : 2;

            /* Energy Detect Power-Down is enabled */
            unsigned int edPowerDown : 1;

            unsigned int reserved4 : 2;
        } fields;

        uint16_t raw;
    };


    /* Interrupt Source Register */
    union phyInterrupt_s {
        struct fields {
            unsigned int reserved0 : 1;

            /* Auto-Negotiation Page Received */
            unsigned int aNegPageRcv : 1;

            /* Parallel Detection Fault */
            unsigned int pDetectFault : 1;

            /* Auto-Negotiation LP Acknowledge */
            unsigned int aNegLPAckn : 1;

            /* Link Down (link status negated) */
            unsigned int linkDown : 1;

            /* Remote Fault Detected */
            unsigned int remoteFault : 1;

            /* Auto-Negotiation complete */
            unsigned int aNegCompleted : 1;

            /* ENERGYON generated */
            unsigned int energyOnGen : 1;

            unsigned int reserved1 : 8;
        } fields;

        uint16_t raw;
    };


    /* Interrupt Mask Register */
    union phyIntMask_s {
        struct fields {
            unsigned int reserved0 : 1;

            /* Mask fo interrupt register */
            unsigned int mask : 7;

            unsigned int reserved1 : 1;
        } fields;

        uint16_t raw;
    };


    /* Special Control / Status Register */
    union phySpecialCtrlStatus_s {
        struct fields {
            unsigned int reserved0 : 2;

            /* HCDSPEED value */
            phySpeed_e speedIndication : 3;

            unsigned int reserved1 : 7;

            /* Auto-negotiation is done */
            unsigned int autodone : 1;

            unsigned int reserved2 : 3;
        } fields;

        uint16_t raw;
    };


    struct ethInterrupt_s {
        union phyInterrupt_s phy;

        union macInterrupt_s mac;
    };


    /* PHY Register addresses */
    static const uint8_t _BasicControl_ = 0;
    static const uint8_t _BasicStatus_ = 1;
    static const uint8_t _PHYIdentifier1_ = 2;
    static const uint8_t _PHYIdentifier2_ = 3;
    static const uint8_t _AutoNegAdvertisement_ = 4;
    static const uint8_t _AutoNegLinkAbility_ = 5;
    static const uint8_t _AutoNegExpansion_ = 6;
    static const uint8_t _ModeCtrlStatus_ = 17;
    static const uint8_t _SpecialModes_ = 18;
    static const uint8_t _SymbolErrorCnt_ = 26;
    static const uint8_t _CtrlStatusIndication_ = 27;
    static const uint8_t _InterruptSource_ = 29;
    static const uint8_t _InterruptMask_ = 30;
    static const uint8_t _SpecialCtrlStatus_ = 31;

    /* Ethernet constants */
    static const uint32_t MAX_PAYLOAD_LENGTH = 1500;
    static const uint32_t MIN_PAYLOAD_LENGTH = 42;

private: 

    /* Base memory address */
    volatile uint32_t* const ethBaseAddress;

    /* MAC status */
    volatile struct macCtrlStatus_s* volatile const macCtrlStatus;

    /* Transmit packet descriptor */
    volatile uint64_t* const macTxPktDesc;

    /* Data tx buffer */
    volatile uint8_t* const macTxBuffer;

    /* Received packet descriptor */
    volatile uint64_t* const macRxPktDesc;

    /* Data rx buffer */
    volatile uint8_t* const macRxBuffer;

    /* Interrupt pending register */
    volatile uint32_t* const macInterrupt;


public:

    inline uint16_t readPHYRegister(uint8_t regAddr) {
        return (volatile uint16_t) *(ethBaseAddress + regAddr);
    };

    inline void writePHYRegister(uint8_t regAddr, uint16_t data) {
        *(ethBaseAddress + regAddr) = data;
    };


/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

    Ethernet();

    ~Ethernet();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/
     
    Ethernet& init(ethSpeed_e speed, ethDuplex_e duplexMode, bool autoNegotiation);

    Ethernet& enableTestMode(bool enable);

    Ethernet& setMacInterrupt(uint8_t index, bool enable, bool* error);


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/

    bool isSending();

    bool isReceiving();

    bool isEmptyTX();

    bool isFullTX();

    bool isEmptyRX();

    bool isFullRX();

    struct ethInterrupt_s getInterrupt();

    bool isLinked();

    bool energyOn();

    uint16_t getErrorCount();


/*****************************************************************/
/*                      MAC COMMUNICATION                        */
/*****************************************************************/

    Ethernet& sendFrame(const uint8_t* packet, uint32_t length, struct macAddr_s destMac, bool* error);

    Ethernet& receiveFrame(uint8_t* buffer, uint32_t length, bool* error);

    union macDescriptor_s getRxDescriptor();


/*****************************************************************/
/*                      PHY CONFIGURATION                        */
/*****************************************************************/
    
    Ethernet& reset();

    Ethernet& powerDown();

    Ethernet& wakeUp();

    Ethernet& configure(ethSpeed_e speed, ethDuplex_e duplexMode);

    Ethernet& setAutoNegotiation(bool enable);

    Ethernet& setPhyInterrupt(uint8_t index, bool enable, bool* error);
};

#endif 