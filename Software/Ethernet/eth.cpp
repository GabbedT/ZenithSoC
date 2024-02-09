#include "../Library/Driver/UART.h"
#include "../Library/Driver/GPIO.h"
#include "../Library/Driver/Timer.h"
#include "../Library/Driver/Ethernet.h"

#include <inttypes.h>

void printHex(uint8_t hex, UART& uart);
void print(const char *str, UART& device);
void getRegister(uint8_t addr, const char *name, Ethernet& ethernet, UART& uart);
extern "C" void eth() {

    Timer timer(0);
    UART uart(0);
    Ethernet ethernet;

    // timer.init(-1, Timer::ONE_SHOT)
    //      .setInterrupt(false);

    uart.init(115200, true, UART::EVEN, UART::STOP1, UART::BIT8)
        .setFlowControl(false);

    // timer.delay(1);

    print("===================== TEST START =====================\n\n Reading PHY LAN8720A registers...\n", uart);

    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false)
            .enableTestMode(true);

    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_PHYIdentifier1_, "\n[2] PHY IDENTIFIER 1 REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_PHYIdentifier2_, "\n[3] PHY IDENTIFIER 2 REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_AutoNegAdvertisement_, "\n[4] AUTO NEGOTIATION ADVERTISEMENT REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_AutoNegExpansion_, "\n[6] AUTO NEGOTIATION EXPANSION REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_ModeCtrlStatus_, "\n[17] MODE CONTROL/STATUS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SpecialModes_, "\n[18] SPECIAL MODES REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SymbolErrorCnt_, "\n[26] SYMBOL ERROR COUNTER REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_CtrlStatusIndication_, "\n[27] SPECIAL CONTROL/STATUS INDICATIONS REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_InterruptSource_, "\n[29] INTERRUPT SOURCE REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_InterruptMask_, "\n[30] INTERRUPT MASK REGISTER: ", ethernet, uart);
    getRegister(Ethernet::_SpecialCtrlStatus_, "\n[31] PHY SPECIAL CONTROL/STATUS REGISTER: \n\n", ethernet, uart);


    uint8_t RX_payload[Ethernet::MIN_PAYLOAD_LENGTH + 4];
    uint8_t TX_payload[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
                            0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
                            0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF,
                            0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF};
    
    struct Ethernet::macAddr_s macDestination;

    for (int i = 0; i < 6; ++i) {
        macDestination.byte[i] = 0x00;
    }

    bool error = false;

    print("[TX] Ethernet Frame: \nMAC Destination: FF-FF-FF-FF-FF-FF \nMAC Source: FF-FF-FF-FF-FF-FF \nLength: 48 Bytes \nPayload: ", uart);

    for (int i = 0; i < sizeof(TX_payload); ++i) {
        printHex(TX_payload[i], uart);
    }

    print("\n\n", uart);


    ethernet.sendFrame(TX_payload, sizeof(TX_payload), macDestination, &error);

    if (error) {
        print("ERROR DURING TX!\n\n", uart);

        return;
    }

    while (ethernet.isEmptyRX()) {  }

    union Ethernet::macDescriptor_s rxDescriptor = ethernet.getRxDescriptor();

    ethernet.receiveFrame(RX_payload, rxDescriptor.fields.length, &error);

    if (error) {
        print("ERROR DURING RX!\n\n", uart);

        return;
    }

    print("[RX] Ethernet Frame: \nMAC Source: ", uart);

    for (int i = 0; i < 6; ++i) {
        printHex(rxDescriptor.fields.address[i], uart);

        if (i != 5) {
            print("-", uart);
        }
    }

    print("\nLength: ", uart);
    printHex(rxDescriptor.fields.length, uart);

    print("\nPayload: ", uart);
    
    for (int i = 0; i < sizeof(RX_payload) - 4; ++i) {
        printHex(RX_payload[i], uart);
    }

    print("\nCRC: ", uart);
    
    for (int i = sizeof(RX_payload); i < sizeof(RX_payload) + 4; ++i) {
        printHex(RX_payload[i], uart);
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

    for (int i = 0; i < 16; ++i) {
        bool lsb = reg & 1;

        char str[2];
        str[0] = lsb ? '1' : '0';
        str[1] = '\0';

        print(str, uart);

        reg >>= 1;
    }
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