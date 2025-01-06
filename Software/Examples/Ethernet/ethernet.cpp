/*
 *  Program example on how to use the Ethernet device with it's driver
 *  in the Zenith SoC platform. The program use 4 different Ethernet
 *  modes (IEEE 802.3 100Mbps / 10Mbps and Ethernet II 100Mbps / 10Mbps),
 *  once the Ethernet MAC receives a packet it's printed out through the
 *  UART device and a packet is transmitted 
 */

#include "../../Library/Driver/GPIO.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Driver/Ethernet.h"
#include "../../Library/Serial_IO.h"

#include <inttypes.h>

#define CRC_SIZE 4

// #define _TEST_
// #define _BROADCAST_

void getRegister(uint8_t addr, const char *name, Ethernet& ethernet);
void packetLoop(GPIO& gpio, Ethernet& ethernet, struct Ethernet::macAddr_s macDestination, uint16_t txEtherType, bool eth_II);

uint8_t RX_payload[Ethernet::MAX_PAYLOAD_LENGTH];
uint8_t TX_payload[] =  "LLC[INIZIO FRAME ETHERNET] PAYLOAD PAYLOAD [FINE FRAME ETHERNET]";


extern "C" void ethernet() {

    /* Setup a timer to wait until the Ethernet 
     * PHY has been correctly resetted ~200us */
    Timer timer(0);

    timer.init(-1, Timer::ONE_SHOT)
         .setInterrupt(false);

    #ifndef _TEST_ 
    
    timer.delay(10); 
    
    #endif


    GPIO gpio(0);
    Ethernet ethernet;

    /* GPIO used to switch modes later */
    gpio.init(0x00, 0xF0, 0x00, 0x00);

    Serial_IO::init();

    Serial_IO::printf("===================== TEST START =====================\n\n Reading PHY LAN8720A registers...\n");

    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::IEEE_8023);

    struct Ethernet::macAddr_s macDestination;

    #ifndef _BROADCAST_

    /* Receiving machine's MAC address */
    macDestination.byte[0] = 0x16;
    macDestination.byte[1] = 0xAA;
    macDestination.byte[2] = 0x57;
    macDestination.byte[3] = 0xC1;
    macDestination.byte[4] = 0xBB;
    macDestination.byte[5] = 0xD8;

    #else 

    for (int i = 0; i < 6; ++i) {
        macDestination.byte[i] = 0xFF;
    }

    #endif 


    #ifndef _TEST_

    timer.delay(10000);

    /* Print all PHY registers */
    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);
    getRegister(Ethernet::_PHYIdentifier1_, "\n[2] PHY IDENTIFIER 1 REGISTER: ", ethernet);
    getRegister(Ethernet::_PHYIdentifier2_, "\n[3] PHY IDENTIFIER 2 REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegAdvertisement_, "\n[4] AUTO NEGOTIATION ADVERTISEMENT REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegLinkAbility_, "\n[5] AUTO NEGOTIATION LINK PARTNER REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegExpansion_, "\n[6] AUTO NEGOTIATION EXPANSION REGISTER: ", ethernet);
    getRegister(Ethernet::_ModeCtrlStatus_, "\n[17] MODE CONTROL/STATUS REGISTER: ", ethernet);
    getRegister(Ethernet::_SpecialModes_, "\n[18] SPECIAL MODES REGISTER: ", ethernet);
    getRegister(Ethernet::_SymbolErrorCnt_, "\n[26] SYMBOL ERROR COUNTER REGISTER: ", ethernet);
    getRegister(Ethernet::_CtrlStatusIndication_, "\n[27] SPECIAL CONTROL/STATUS INDICATIONS REGISTER: ", ethernet);
    getRegister(Ethernet::_InterruptSource_, "\n[29] INTERRUPT SOURCE REGISTER: ", ethernet);
    getRegister(Ethernet::_InterruptMask_, "\n[30] INTERRUPT MASK REGISTER: ", ethernet);
    getRegister(Ethernet::_SpecialCtrlStatus_, "\n[31] PHY SPECIAL CONTROL/STATUS REGISTER: ", ethernet);

    #endif


    Serial_IO::printf("\n\n\n===================== IEEE 802.3 100Mbps Test =====================");

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);
    
    #ifndef _TEST_

    timer.delay(1000);
    Serial_IO::println("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, false);


    Serial_IO::printf("\n\n\n===================== IEEE 802.3 10Mbps Test =====================");

    ethernet.init(Ethernet::MBPS10, Ethernet::FULL_DUPLEX, false, Ethernet::IEEE_8023);

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);

    #ifndef _TEST_
    
    timer.delay(1000);
    Serial_IO::println("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, false);


    Serial_IO::printf("\n\n\n===================== Ethernet II 100Mbps Test =====================");
    
    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::ETHERNET_II);

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);

    #ifndef _TEST_

    timer.delay(1000);
    Serial_IO::println("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, true);


    Serial_IO::printf("\n\n\n===================== Ethernet II 10Mbps Test =====================");

    ethernet.init(Ethernet::MBPS10, Ethernet::FULL_DUPLEX, false, Ethernet::ETHERNET_II);

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);

    #ifndef _TEST_
    
    timer.delay(1000);
    Serial_IO::println("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, true);


    return;
}


void getRegister(uint8_t addr, const char *name, Ethernet& ethernet) {
    uint16_t reg = ethernet.readPHYRegister(addr);

    Serial_IO::printf(name);
    Serial_IO::writeB(reg);
}


void packetLoop(GPIO& gpio, Ethernet& ethernet, struct Ethernet::macAddr_s macDestination, uint16_t txEtherType, bool eth_II) {
    volatile bool oldRcv, newRcv; oldRcv = newRcv = false;
    uint16_t rxEtherType;
    Ethernet::ethError_e error = Ethernet::NO_ERROR;
    
    while (!gpio.getPinValue(4)) {
        newRcv = ethernet.isReceiving();

        if (!oldRcv & newRcv) {
            Serial_IO::println("[Packet] Is arriving\n");
            
            if (eth_II) {
                ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, txEtherType, &error);
            } else {
                ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, &error);
            }

            if (error != Ethernet::NO_ERROR) {
                Serial_IO::println("ERROR DURING TX!\n\n");

                continue;
            }
        } else if (oldRcv & !newRcv) {
            Serial_IO::println("[Packet] Done!\n");

            volatile union Ethernet::macDescriptor_s rxDescriptor = ethernet.getRxDescriptor();

            if (eth_II) {
                ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &rxEtherType, &error);
            } else {
                ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &error);
            }

            if (error != Ethernet::NO_ERROR) {
                Serial_IO::println("ERROR DURING RX!\n");

                continue;
            }


            Serial_IO::printf("[RX] Ethernet Frame: \nMAC Source: ");

            for (int i = 0; i < 6; ++i) {
                Serial_IO::writeH(rxDescriptor.fields.address[5 - i]);

                if (i != 5) {
                    Serial_IO::printf("-");
                }
            }

            Serial_IO::printf("\nLength: ");
            Serial_IO::writeH((uint16_t) rxDescriptor.fields.length);

            if (eth_II) {
                Serial_IO::printf("\nEtherType: ");
                Serial_IO::writeH(rxEtherType);
            }

            Serial_IO::printf("\nPayload: ");
    
            for (int i = 0; i < rxDescriptor.fields.length; ++i) {
                Serial_IO::writeH(RX_payload[i]);
            }

            Serial_IO::printf("\nCRC: ");
    
            for (int i = rxDescriptor.fields.length; i < rxDescriptor.fields.length + 4; ++i) {
                Serial_IO::writeH(RX_payload[i]);
            }

            Serial_IO::printf("\n\n");
        }

        oldRcv = newRcv;
    }
}