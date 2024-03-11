#include "../../Library/Driver/GPIO.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Driver/Ethernet.h"
#include "../../Library/SerialOut.h"

#include <inttypes.h>

#define CRC_SIZE 4

// #define _TEST_
// #define _BROADCAST_

void getRegister(uint8_t addr, const char *name, Ethernet& ethernet);
void packetLoop(GPIO& gpio, Ethernet& ethernet, struct Ethernet::macAddr_s macDestination, uint16_t txEtherType, bool eth_II);

uint8_t RX_payload[Ethernet::MAX_PAYLOAD_LENGTH];
uint8_t TX_payload[] =  "LLC[INIZIO FRAME ETHERNET]\n\n"
                        "MEMBRI DEL TEAM FRANCO:\n"
                        "Gabriele: Il piu' figo di tutti naturalmente.\n"
                        "Emanuele: Lecca il buco del culo di Nanouk e la figa di Zoe.\n"
                        "Andrea: Si sta scagazzando nelle mutande per TdS di domani.\n"
                        "Hermes: Si crede fortissimo quando si incazza ma in realta e' un cucciolo di topo.\n\n"
                        "[FINE FRAME ETHERNET]";


extern "C" void ethernet() {

    GPIO gpio(0);
    Timer timer(0);
    Ethernet ethernet;

    gpio.init(0x00, 0xF0, 0x00, 0x00);

    timer.init(-1, Timer::ONE_SHOT)
         .setInterrupt(false);


    SerialOut::printf("===================== TEST START =====================\n\n Reading PHY LAN8720A registers...\n");

    timer.delay(10);

    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::IEEE_8023);

    struct Ethernet::macAddr_s macDestination;

    #ifndef _BROADCAST_

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


    SerialOut::printf("\n\n\n===================== IEEE 802.3 100Mbps Test =====================");
    
    #ifndef _TEST_

    timer.delay(1000);
    SerialOut::println("\n\n[LINK STATUS] Linking...\n");
    
    while (!ethernet.isLinked()) { }

    SerialOut::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, false);


    SerialOut::printf("\n\n\n===================== IEEE 802.3 10Mbps Test =====================");

    ethernet.init(Ethernet::MBPS10, Ethernet::FULL_DUPLEX, false, Ethernet::IEEE_8023);

    #ifndef _TEST_
    
    timer.delay(1000);
    SerialOut::println("\n\n[LINK STATUS] Linking...\n");

    while (!ethernet.isLinked()) { }

    SerialOut::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, false);


    SerialOut::printf("\n\n\n===================== Ethernet II 100Mbps Test =====================");
    
    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::ETHERNET_II);

    #ifndef _TEST_

    timer.delay(1000);
    SerialOut::println("\n\n[LINK STATUS] Linking...\n");

    while (!ethernet.isLinked()) { }

    SerialOut::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, true);


    SerialOut::printf("\n\n\n===================== Ethernet II 10Mbps Test =====================");

    ethernet.init(Ethernet::MBPS10, Ethernet::FULL_DUPLEX, false, Ethernet::ETHERNET_II);

    #ifndef _TEST_
    
    timer.delay(1000);
    SerialOut::println("\n\n[LINK STATUS] Linking...\n");
    
    while (!ethernet.isLinked()) { }

    SerialOut::println("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);

    #endif

    packetLoop(gpio, ethernet, macDestination, 1600, true);


    return;
}


void getRegister(uint8_t addr, const char *name, Ethernet& ethernet) {
    uint16_t reg = ethernet.readPHYRegister(addr);
    SerialOut::printf(name);

    char str[17];

    /* EOS char */    
    str[16] = '\0';

    for (int i = 0; i < 16; ++i) {
        bool lsb = reg & 1;
        str[15 - i] = lsb ? '1' : '0';

        reg >>= 1;
    }

    SerialOut::println(str);
}


void packetLoop(GPIO& gpio, Ethernet& ethernet, struct Ethernet::macAddr_s macDestination, uint16_t txEtherType, bool eth_II) {
    volatile bool oldRcv, newRcv; oldRcv = newRcv = false;
    uint16_t rxEtherType;
    Ethernet::ethError_e error = Ethernet::NO_ERROR;
    
    while (!gpio.getPinValue(4)) {
        newRcv = ethernet.isReceiving();

        if (!oldRcv & newRcv) {
            SerialOut::println("[Packet] Is arriving\n");
            
            if (eth_II) {
                ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, txEtherType, &error);
            } else {
                ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, &error);
            }

            if (error != Ethernet::NO_ERROR) {
                SerialOut::println("ERROR DURING TX!\n\n");

                continue;
            }
        } else if (oldRcv & !newRcv) {
            SerialOut::println("[Packet] Done!\n");

            volatile union Ethernet::macDescriptor_s rxDescriptor = ethernet.getRxDescriptor();

            if (eth_II) {
                ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &rxEtherType, &error);
            } else {
                ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &error);
            }

            if (error != Ethernet::NO_ERROR) {
                SerialOut::println("ERROR DURING RX!\n");

                continue;
            }


            SerialOut::printf("[RX] Ethernet Frame: \nMAC Source: ");

            for (int i = 0; i < 6; ++i) {
                SerialOut::writeH(rxDescriptor.fields.address[5 - i]);

                if (i != 5) {
                    SerialOut::printf("-");
                }
            }

            SerialOut::printf("\nLength: ");
            SerialOut::writeH(rxDescriptor.fields.length >> 8);
            SerialOut::writeH(rxDescriptor.fields.length);

            if (eth_II) {
                SerialOut::printf("\nEtherType: ");
                SerialOut::writeH(rxEtherType >> 8);
                SerialOut::writeH(rxEtherType);
            }

            SerialOut::printf("\nPayload: ");
    
            for (int i = 0; i < rxDescriptor.fields.length; ++i) {
                SerialOut::writeH(RX_payload[i]);
            }

            SerialOut::printf("\nCRC: ");
    
            for (int i = rxDescriptor.fields.length; i < rxDescriptor.fields.length + 4; ++i) {
                SerialOut::writeH(RX_payload[i]);
            }

            SerialOut::printf("\n\n");
        }

        oldRcv = newRcv;
    }
}