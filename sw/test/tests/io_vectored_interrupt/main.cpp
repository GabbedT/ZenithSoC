#include "io_interrupt_tests.h"

#include "interrupt.h"

#include "driver/UART.h"

namespace {

    void write(UART& uart, const char* text) {
        while (*text) {
            uart.sendByte(*text++);
        }
    }

}


extern "C" int main() {
    UART uart;
    uart.init(6'250'000, false, UART::EVEN, UART::STOP1, UART::BIT8);
    uart.setModeRX(false);

    write(uart, "\nZenithSoC vectored IO interrupt test\n");

    uint32_t failures = 0;

#if INTERRUPT_TEST_CASE == 1
    failures += !testUARTTXEmptyInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 2
    failures += !testUARTTXDoneInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 3
    failures += !testUARTRXDoneInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 4
    failures += !testUARTFullInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 5
    failures += !testTimerInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 6
    failures += !testGPIOPosedgeInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 7
    failures += !testGPIONegedgeInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 8
    failures += !testGPIOBothEdgeInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 9
    failures += !testGPIOHighLevelInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 10
    failures += !testTraceInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 11
    failures += !testSPIInterrupt(uart);
#elif INTERRUPT_TEST_CASE == 12
    failures += !testAPUInterrupt(uart);
#endif

    /* Ethernet and SD require external models and stay out of this main */

    failures += unexpectedInterrupts != 0;

    write(uart, failures ? "Vectored IO interrupt suite FAILED\n" : "Vectored IO interrupt suite PASSED\n");

    return failures;
}
