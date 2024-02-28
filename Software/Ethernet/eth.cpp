#include "../Library/Driver/UART.h"
#include "../Library/Driver/GPIO.h"
#include "../Library/Driver/Timer.h"
#include "../Library/Driver/Ethernet.h"

#include <inttypes.h>

#define CRC_SIZE 4

// #define _TEST_
// #define _BROADCAST_

void printHex(uint8_t hex, UART& uart);
void print(const char *str, UART& device);
void getRegister(uint8_t addr, const char *name, Ethernet& ethernet, UART& uart);

extern "C" void eth() {

    Timer timer(0);
    UART uart(0);
    Ethernet ethernet;

    timer.init(-1, Timer::ONE_SHOT)
         .setInterrupt(false);

    uart.init(115200, true, UART::EVEN, UART::STOP1, UART::BIT8)
        .setFlowControl(false);

    print("===================== TEST START =====================\n\n Reading PHY LAN8720A registers...\n", uart);

    timer.delay(10);

    ethernet.init(Ethernet::MBPS10, Ethernet::FULL_DUPLEX, false);

    #ifndef _TEST_

    timer.delay(10000);

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_PHYIdentifier1_, "\n[2] PHY IDENTIFIER 1 REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_PHYIdentifier2_, "\n[3] PHY IDENTIFIER 2 REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_AutoNegAdvertisement_, "\n[4] AUTO NEGOTIATION ADVERTISEMENT REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_AutoNegLinkAbility_, "\n[5] AUTO NEGOTIATION LINK PARTNER REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_AutoNegExpansion_, "\n[6] AUTO NEGOTIATION EXPANSION REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_ModeCtrlStatus_, "\n[17] MODE CONTROL/STATUS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SpecialModes_, "\n[18] SPECIAL MODES REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SymbolErrorCnt_, "\n[26] SYMBOL ERROR COUNTER REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_CtrlStatusIndication_, "\n[27] SPECIAL CONTROL/STATUS INDICATIONS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_InterruptSource_, "\n[29] INTERRUPT SOURCE REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_InterruptMask_, "\n[30] INTERRUPT MASK REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SpecialCtrlStatus_, "\n[31] PHY SPECIAL CONTROL/STATUS REGISTER: ", ethernet, uart);

    timer.delay(1000);
    print("\n\n[LINK STATUS] Linking...\n", uart);
    while (!ethernet.isLinked()) { }
    print("[LINK STATUS] Enstablished!\n\n", uart);
    timer.delay(1000);

    #endif

    uint8_t RX_payload[Ethernet::MAX_PAYLOAD_LENGTH];
    uint8_t TX_payload[] = "LLC [INIZIO FRAME ETHERNET]\n\n"
                           "MEMBRI DEL TEAM FRANCO:\n"
                           "Gabriele: Il piu' figo di tutti naturalmente.\n"
                           "Emanuele: Lecca il buco del culo di Nanouk e la figa di Zoe.\n"
                           "Andrea: Si sta scagazzando nelle mutande per TdS di domani.\n"
                           "Hermes: Si crede fortissimo quando si incazza ma in realta e' un cucciolo di topo.\n\n"
                           "[FINE FRAME ETHERNET]";


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

    Ethernet::ethError_e error = Ethernet::NO_ERROR;

    ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, &error);

    #ifndef _TEST_

    if (error != Ethernet::NO_ERROR) {
        print("ERROR DURING TX!\n\n", uart);

        return;
    }

    #endif 

    volatile bool oldRcv, newRcv; oldRcv = newRcv = false;
    
    while (1) {
        newRcv = ethernet.isReceiving();

        if (!oldRcv & newRcv) {
            print("[Packet] Is arriving\n", uart);
            
            ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, &error);
        } else if (oldRcv & !newRcv) {
            print("[Packet] Done!\n", uart);

            volatile union Ethernet::macDescriptor_s rxDescriptor = ethernet.getRxDescriptor();
            ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &error);


            print("[RX] Ethernet Frame: \nMAC Source: ", uart);

            for (int i = 0; i < 6; ++i) {
                printHex(rxDescriptor.fields.address[5 - i], uart);

                if (i != 5) {
                    print("-", uart);
                }
            }

            print("\nLength: ", uart);
            printHex(rxDescriptor.fields.length >> 8, uart);
            printHex(rxDescriptor.fields.length, uart);

            print("\nPayload: ", uart);
    
            for (int i = 0; i < rxDescriptor.fields.length; ++i) {
                printHex(RX_payload[i], uart);
            }

            print("\nCRC: ", uart);
    
            for (int i = rxDescriptor.fields.length; i < rxDescriptor.fields.length + 4; ++i) {
                printHex(RX_payload[i], uart);
            }

            print("\n\n", uart);
        }

        oldRcv = newRcv;
    }

    return;
}

void print(const char *str, UART& device) {
    unsigned int size; 

    for (size = 0; str[size] != '\0'; ++size) { }

    device.loadBufferTX((uint8_t *) str, size);

    return;
}


void getRegister(uint8_t addr, const char *name, Ethernet& ethernet, UART& uart) {
    uint16_t reg = ethernet.readPHYRegister(addr);
    print(name, uart);

    char str[17];

    /* EOS char */    
    str[16] = '\0';

    for (int i = 0; i < 16; ++i) {
        bool lsb = reg & 1;
        str[15 - i] = lsb ? '1' : '0';

        reg >>= 1;
    }

    print(str, uart);
}


void printHex(uint8_t hex, UART& uart) {
    uint8_t nibble[2];
    char str[2];

    nibble[1] = (hex & 0xF0) >> 4;
    nibble[0] = hex & 0x0F;

    for (int i = 1; i >= 0; --i) {
        if (nibble[i] < 10) {
            /* 0 - 9 */
            str[0] = '0' + nibble[i];
            str[1] = '\0';
        } else {
            /* A - F */
            str[0] = 'A' + nibble[i] - 10;
            str[1] = '\0';
        }

        uart.loadBufferTX((uint8_t *) str, 2);
    }
}