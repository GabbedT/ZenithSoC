#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

#include "UART.h"


int nPasses = 0;

void main(void) {
    UART uart(0);

    /* Default initialization */
    uart.init();

        

    if ((rx & 0xFF) == 0x55) {
        vp_println("[UART] Loopback PASS");
        TEST_PASS();
    } else {
        vp_println("[UART] Loopback FAIL");
        TEST_FAIL();
    }
}