#ifndef ETHERNET_H
#define ETHERNET_H

#include <inttypes.h>

#include "UART.h"

#include "../mmio.h"


class Ethernet {

public:

    UART debug;

    /* Function errors */
    enum ethError_e { NO_ERROR, TX_FULL, RX_EMPTY, LENGTH_EXCEEDED, LENGTH_SUBCEEDED, INDEX_OUT_OF_RANGE };

    /* Bandwidth */
    enum ethSpeed_e { MBPS10, MBPS100 };

    /* Medium Dependent Interface */
    enum ethChannel_e { MDI, MDI_X };

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
        /* Payload buffer status */
        unsigned int payloadEmptyRX : 1;
        unsigned int payloadFullRX : 1;
        unsigned int payloadEmptyTX : 1;
        unsigned int payloadFullTX : 1;

        /* Packet buffer status */
        unsigned int packetEmptyRX : 1;
        unsigned int packetFullRX : 1;
        unsigned int packetEmptyTX : 1;
        unsigned int packetFullTX : 1;

        /* TX and RX FSMs status */
        unsigned int idleRX : 1;
        unsigned int idleTX : 1;

        /* Enable interrupt for each event */
        unsigned int interruptEnable : 4;     

        /* Select MAC speed */   
        unsigned int speed : 1;     

        unsigned int padding : 17;   
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

            unsigned int reserved1 : 8;
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


    /* Special Control / Status Indication Register */
    union phyCtrlStatusInd_s {
        struct fields {
            unsigned int reserved0 : 4;

            /* HCDSPEED value */
            unsigned int xpol : 1;

            unsigned int reserved1 : 6;

            /* Disable SQE test */
            unsigned int sqeTest : 1;

            unsigned int reserved2 : 1;

            /* Manual channel select */
            unsigned int channelSelect : 1;

            unsigned int reserved3 : 1;

            /* HP Auto-MDIX Control */
            unsigned int autoMDIX : 1;
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

    /**
     * @brief Construct a new Ethernet object, reset PHY chip. Disable
     * interrupts and clear old ones. 
     */
    Ethernet();

    /**
     * @brief Destroy the Ethernet object, reset PHY chip and power it down. Disable
     * interrupts and clear old ones. 
     */
    ~Ethernet();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/
    
    /**
     * @brief Initialize Ethernet device with speed, duplex settings and auto negotiation with the
     * other end. The PHY chip is waken up and setted along with the MAC.
     * 
     * @param speed Select between 10 - 100 Mbps modes.
     * @param duplexMode Half or Full duplex mode.
     * @param autoNegotiation Enable auto negotiation with the other end device.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& init(ethSpeed_e speed, ethDuplex_e duplexMode, bool autoNegotiation);

    /**
     * @brief Enable or disable a specific interrupt for the MAC device, if the index is out of bound,
     * the error pointer will be changed into: INDEX_OUT_OF_RANGE.
     * 
     * @param index Interrupt mask bit position.
     * @param enable Enable or disable interrupt.
     * @param error Pointer to an error variable.
     * 
     * @warning The parameter index must not be greater than 3!
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setMacInterrupt(uint8_t index, bool enable, ethError_e* error);


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/

    /**
     * @brief Check if the transmitter FSM is sending data to the PHY or not.
     * 
     * @return The status of the transmitter FSM.
     */
    bool isSending();

    /**
     * @brief Check if the PHY is sending received data to the MAC or not.
     * 
     * @return The status of the receiver FSM.
     */
    bool isReceiving();

    /**
     * @brief Check if the TX payload buffer register is empty.
     * 
     * @return The status of the TX payload buffer.
     */
    bool isEmptyPayloadTX();

    /**
     * @brief Check if the TX payload buffer register is full.
     * 
     * @return The status of the TX payload buffer.
     */
    bool isFullPayloadTX();

    /**
     * @brief Check if the RX payload buffer register is empty.
     * 
     * @return The status of the RX payload buffer.
     */
    bool isEmptyPayloadRX();

    /**
     * @brief Check if the RX payload buffer register is full.
     * 
     * @return The status of the RX payload buffer.
     */
    bool isFullPayloadRX();

    /**
     * @brief Check if the TX packet descriptor buffer register is empty.
     * 
     * @return The status of the TX packet descriptor buffer.
     */
    bool isEmptyPacketTX();

    /**
     * @brief Check if the TX packet descriptor buffer register is full.
     * 
     * @return The status of the TX packet descriptor buffer.
     */
    bool isFullPacketTX();

    /**
     * @brief Check if the RX packet descriptor buffer register is empty.
     * 
     * @return The status of the RX packet descriptor buffer.
     */
    bool isEmptyPacketRX();

    /**
     * @brief Check if the RX packet descriptor buffer register is full.
     * 
     * @return The status of the RX packet descriptor buffer.
     */
    bool isFullPacketRX();

    /**
     * @brief Get the MAC and PHY interrupt status.
     * 
     * @return A struct containing both MAC and PHY interrupt status.
     */
    struct ethInterrupt_s getInterrupt();

    /**
     * @brief Check if the PHY link is enstablished.
     * 
     * @return The status of the PHY link.
     */
    bool isLinked();

    /**
     * @brief Check if the PHY is detecting energy.
     * 
     * @return The status of the PHY.
     */
    bool energyOn();

    /**
     * @brief Check the channel mode of the PHY
     * 
     * @return Channel status of the PHY
     */
    ethChannel_e getChannel();

    /**
     * @brief Read the Error Count Register from the PHY.
     * 
     * @return A 16 bit value containing the number of errors.
     */
    uint16_t getErrorCount();


/*****************************************************************/
/*                      MAC COMMUNICATION                        */
/*****************************************************************/

    /**
     * @brief Send an Ethernet frame on the link.
     * 
     * @param packet An array of bytes containing the payload to send.
     * @param length The length of the payload.
     * @param destMac The destination MAC address.
     * @param error Pointer to an error variable.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& sendFrame(const uint8_t* packet, uint32_t length, struct macAddr_s destMac, ethError_e* error);

    /**
     * @brief Receive the payload received plus 4 bytes of CRC.
     * 
     * @param packet An array of bytes to contain the payload and CRC received.
     * @param length The length of the payload.
     * @param error Pointer to an error variable.
     * 
     * @warning The array size must accomodate the entire payload plus 4 bytes of CRC, however
     * the length passed in the function refers only to the payload size!
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& receiveFrame(uint8_t* buffer, uint32_t length, ethError_e* error);

    /**
     * @brief Get the packet received descriptor containing the source MAC address and
     * the payload length
     * 
     * @return The packet descriptor data. 
     */
    union macDescriptor_s getRxDescriptor();


/*****************************************************************/
/*                      PHY CONFIGURATION                        */
/*****************************************************************/
    
    /**
     * @brief Software reset for PHY chip.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& reset();

    /**
     * @brief Put PHY chip on sleep.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& powerDown();

    /**
     * @brief Wake up PHY chip.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& wakeUp();

    /**
     * @brief Configure PHY chip directly with speed and duplex mode.
     * 
     * @param speed Select between 10 - 100 Mbps modes.
     * @param duplexMode Half or Full duplex mode.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& configure(ethSpeed_e speed, ethDuplex_e duplexMode);

    /**
     * @brief Setup the PHY in near-loopback mode to redirect transmitted data to the RX bus of the 
     * RMII interface.
     * 
     * @param enable Enable or disable test mode.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setTestMode(bool enable);

    /**
     * @brief Setup the SQE or Heartbeat bit in the PHY register.
     * 
     * @param enable Enable or disable SQE mode.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setHeartbeatTest(bool enable);

    /**
     * @brief Set PHY chip auto negotiation ability to exchange infos with
     * the device on the other end.
     * 
     * @param enable Enable or disable auto negotiation.
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setAutoNegotiation(bool enable);

    /**
     * @brief Disable Auto-MDIX and force to use a specific channel
     * 
     * @param channel Select MDI or MDI-X channel
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setChannel(ethChannel_e channel);

    /**
     * @brief Enable or disable a specific interrupt for the PHY device, if the index is out of bound,
     * the error pointer will be changed into: INDEX_OUT_OF_RANGE.
     * 
     * @param index Interrupt mask bit position.
     * @param enable Enable or disable interrupt.
     * @param error Pointer to an error variable.
     * 
     * @warning The parameter index must not be greater than 6!
     * 
     * @return The Ethernet object itself to chain the function call.
     */
    Ethernet& setPhyInterrupt(uint8_t index, bool enable, ethError_e* error);
};

#endif 