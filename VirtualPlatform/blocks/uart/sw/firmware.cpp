#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

#include "UART.h"

extern "C" int main(void) {
    UART uart(0);

    uart.init();

    for (int i = 0; i < 200; ++i) {
        const uint8_t toSend[] = "Hello Virtual Platform! :D";
        uint32_t size = sizeof(toSend);

        vp_println((const char*) toSend);

        uart.loadBufferTX((uint8_t*)toSend, size);

        uint8_t toReceive[sizeof(toSend)] = {0};
        uart.unloadBufferRX(toReceive, size);

        vp_println((const char*) toReceive);
    }

    TEST_PASS();
}