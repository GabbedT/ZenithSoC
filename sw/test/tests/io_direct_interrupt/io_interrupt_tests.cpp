#include "io_interrupt_tests.h"

#include "interrupt.h"

#include "driver/AudioCapture.h"
#include "driver/Ethernet.h"
#include "driver/GPIO.h"
#include "driver/SD.h"
#include "driver/SPI.h"
#include "driver/Timer.h"
#include "driver/TraceUnit.h"
#include "driver/UART.h"

namespace {

    void write(UART& uart, const char* text) {
        while (*text) {
            uart.sendByte(*text++);
        }
    }


    void report(UART& uart, const char* name, bool passed) {
        write(uart, passed ? "[PASS] " : "[FAIL] ");
        write(uart, name);
        write(uart, "\n");
    }


    void drainUART(UART& uart) {
        while (!uart.getCtrlStatus()->emptyTX) {  }

        /* emptyTX precedes the end of the last serialized frame */
        waitCycles(512);
    }

    void prepareInterrupt() {
        asm volatile ("csrc mstatus, %0" :: "r"(1u << 3) : "memory");
        asm volatile ("fence" ::: "memory");
    }


    void releaseInterrupt() {
        asm volatile ("fence" ::: "memory");
        asm volatile ("csrs mstatus, %0" :: "r"(1u << 3) : "memory");
    }


    __attribute__((always_inline)) inline bool runGPIOEvent(GPIO& gpio, GPIO::triggerLevel_e level, bool from, bool to) {
        prepareInterrupt();

        gpio.setInterruptEnable(1, false)
            .setPinValue(0, from)
            .setTriggerLevel(1, level)
            .setInterruptPending(1, false);

        waitCycles(32);

        uint32_t previous = interruptCount[GPIO_INTERRUPT];

        gpio.setInterruptEnable(1, true)
            .setPinValue(0, to);

        releaseInterrupt();

        return waitInterrupt(GPIO_INTERRUPT, previous) &&
               ((interruptEvent[GPIO_INTERRUPT] & (1u << 1)) != 0);
    }

}


bool testUARTTXEmptyInterrupt(UART& uart) {
    uart.setModeRX(false);
    drainUART(uart);
    *uart.event = 0;

    /* Empty FIFO interrupt */
    prepareInterrupt();
    uint32_t previous = interruptCount[UART_INTERRUPT];
    uart.setInterrupt(UART::TX_EMPTY);
    releaseInterrupt();

    bool emptyPassed = waitInterrupt(UART_INTERRUPT, previous) && ((interruptEvent[UART_INTERRUPT] & UART::TX_EMPTY) != 0);
    report(uart, "UART TX empty interrupt", emptyPassed);

    return emptyPassed;
}


bool testUARTTXDoneInterrupt(UART& uart) {
    /* End of a physically serialized frame */
    drainUART(uart);
    *uart.event = 0;
    prepareInterrupt();
    uint32_t previous = interruptCount[UART_INTERRUPT];
    uart.setInterrupt(UART::DATA_TX)
        .sendByte('T');
    releaseInterrupt();

    bool txPassed = waitInterrupt(UART_INTERRUPT, previous) && ((interruptEvent[UART_INTERRUPT] & UART::DATA_TX) != 0);
    report(uart, "UART TX done interrupt", txPassed);

    return txPassed;
}


bool testUARTRXDoneInterrupt(UART& uart) {
    /* The Verilator wrapper routes TX back to RX */
    drainUART(uart);
    *uart.event = 0;
    prepareInterrupt();
    uint32_t previous = interruptCount[UART_INTERRUPT];
    uart.setModeRX(true)
        .setInterrupt(UART::DATA_RX)
        .sendByte('R');
    releaseInterrupt();

    bool rxPassed = waitInterrupt(UART_INTERRUPT, previous) && ((interruptEvent[UART_INTERRUPT] & UART::DATA_RX) != 0);
    uint8_t received = rxPassed ? uart.receiveByte() : 0;
    uart.setModeRX(false);

    rxPassed &= received == 'R';
    report(uart, "UART RX done interrupt", rxPassed);

    return rxPassed;
}


bool testUARTInterrupts(UART& uart) {
    return testUARTTXEmptyInterrupt(uart) &&
           testUARTTXDoneInterrupt(uart) &&
           testUARTRXDoneInterrupt(uart);
}


bool testUARTFullInterrupt(UART& uart) {
    /* Fill TX buffer, i expect RX buffer to go full */
    drainUART(uart);
    *uart.event = 0;
    prepareInterrupt();
    uint32_t previous = interruptCount[UART_INTERRUPT];
    uart.setModeRX(true)
        .setInterrupt(UART::RX_FULL);

    while (!uart.status->fullTX) {
        uart.sendByte('F');
    }

    releaseInterrupt();

    bool fullPassed = waitInterrupt(UART_INTERRUPT, previous) && ((interruptEvent[UART_INTERRUPT] & UART::RX_FULL) != 0);
    uart.disableInterrupt(UART::RX_FULL);

    while (!uart.getCtrlStatus()->emptyRX) {
        uart.receiveByte();
    }

    uart.setModeRX(false);

    report(uart, "UART RX full interrupt", fullPassed);

    return fullPassed;
}


bool testTimerInterrupt(UART& uart) {
    Timer timer;

    prepareInterrupt();
    uint32_t previous = interruptCount[TIMER_INTERRUPT];

    timer.stop()
         .setTime(0)
         .setThreshold(128)
         .setTimerMode(Timer::ONE_SHOT)
         .clearInterrupt()
         .setInterrupt(true)
         .start();
    releaseInterrupt();

    bool passed = waitInterrupt(TIMER_INTERRUPT, previous) &&
                  ((interruptEvent[TIMER_INTERRUPT] & (1u << 1)) != 0) &&
                  timer.isHalted();

    report(uart, "Timer compare interrupt", passed);

    return passed;
}


bool testGPIOInterrupts(UART& uart) {
    return testGPIOPosedgeInterrupt(uart) &&
           testGPIONegedgeInterrupt(uart) &&
           testGPIOBothEdgeInterrupt(uart) &&
           testGPIOHighLevelInterrupt(uart);
}


bool testGPIOPosedgeInterrupt(UART& uart) {
    GPIO gpio;

    /* GPIO0 output is connected to GPIO1 input by the Verilator wrapper */
    gpio.init(0x00, 1u << 1, 0x00, GPIO::POSEDGE);

    bool posedgePassed = runGPIOEvent(gpio, GPIO::POSEDGE, false, true);
    report(uart, "GPIO positive edge interrupt", posedgePassed);

    return posedgePassed;
}


bool testGPIONegedgeInterrupt(UART& uart) {
    GPIO gpio;
    gpio.init(0x00, 1u << 1, 0x00, GPIO::NEGEDGE);

    bool negedgePassed = runGPIOEvent(gpio, GPIO::NEGEDGE, true, false);
    report(uart, "GPIO negative edge interrupt", negedgePassed);

    return negedgePassed;
}


bool testGPIOBothEdgeInterrupt(UART& uart) {
    GPIO gpio;
    gpio.init(0x00, 1u << 1, 0x00, GPIO::BOTH);

    bool bothPassed = runGPIOEvent(gpio, GPIO::BOTH, false, true);
    report(uart, "GPIO both-edge interrupt", bothPassed);

    return bothPassed;
}


bool testGPIOHighLevelInterrupt(UART& uart) {
    GPIO gpio;
    gpio.init(0x00, 1u << 1, 0x00, GPIO::HIGH);

    bool highPassed = runGPIOEvent(gpio, GPIO::HIGH, false, true);
    report(uart, "GPIO high-level interrupt", highPassed);

    return highPassed;
}


bool testSPIInterrupt(UART& uart) {
    SPI spi;
    SPI::spiError_e error = SPI::NO_ERROR;

    /* Leave enough half-periods for the two-stage MISO synchronizer. */
    spi.init(12'500'000, SPI::MODE0, SPI::MSB_FIRST, &error)
       .connect(0, &error);

    *spi.event = 0;
    prepareInterrupt();
    uint32_t previous = interruptCount[SPI_INTERRUPT];

    spi.enableInterrupt(true)
       .transfer<uint8_t>(0xA5);
    releaseInterrupt();

    bool interruptPassed = waitInterrupt(SPI_INTERRUPT, previous) &&
                           ((interruptEvent[SPI_INTERRUPT] & 1u) != 0);

    uint8_t received = 0;
    spi.retrieve(&received, 1, &error);

    bool passed = interruptPassed && (error == SPI::NO_ERROR) && (received == 0xA5);
    report(uart, "SPI transaction interrupt", passed);

    return passed;
}


bool testAPUInterrupt(UART& uart) {
    AudioCapture capture;
    AudioCapture::audioCaptError_e error = AudioCapture::NO_ERROR;

    /* Constant-low PDM input produces a sample below the right threshold */
    capture.init(AudioCapture::RIGHT, false, 2'000'000, 1'000'000, error)
           .setThreshold(1);

    *capture.event = 0;
    prepareInterrupt();
    uint32_t previous = interruptCount[APU_INTERRUPT];
    capture.control->interruptEnable = AudioCapture::RIGHT_THRESHOLD_LOW;
    releaseInterrupt();

    bool passed = (error == AudioCapture::NO_ERROR) &&
                  waitInterrupt(APU_INTERRUPT, previous, 1'000'000) &&
                  ((interruptEvent[APU_INTERRUPT] & AudioCapture::RIGHT_THRESHOLD_LOW) != 0);

    report(uart, "APU capture threshold interrupt", passed);

    return passed;
}


bool testTraceInterrupt(UART& uart) {
    TraceUnit trace;

    prepareInterrupt();
    uint32_t previous = interruptCount[TRACE_INTERRUPT];

    /* Enabling the empty event while the packet FIFO is empty creates an edge */
    trace.enableInterrupt(false, true);
    releaseInterrupt();

    bool passed = waitInterrupt(TRACE_INTERRUPT, previous) &&
                  ((interruptEvent[TRACE_INTERRUPT] & 1u) != 0);

    report(uart, "Trace buffer empty interrupt", passed);

    return passed;
}


bool testEthernetInterrupt(UART& uart) {
    Ethernet ethernet;
    Ethernet::ethError_e error = Ethernet::NO_ERROR;
    Ethernet::macAddr_s destination = {{0x02, 0x00, 0x00, 0x00, 0x00, 0x01}};
    uint8_t payload[46] = {0};

    *ethernet.macInterrupt = 0;
    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::ETHERNET_II)
            .setMacInterrupt(1, true, error);

    uint32_t previous = interruptCount[ETHERNET_INTERRUPT];
    ethernet.sendFrame(payload, sizeof(payload), destination, 0x88B5, error);

    bool passed = (error == Ethernet::NO_ERROR) &&
                  waitInterrupt(ETHERNET_INTERRUPT, previous, 2'000'000) &&
                  ((interruptEvent[ETHERNET_INTERRUPT] & Ethernet::TX_DONE) != 0);

    report(uart, "Ethernet TX interrupt", passed);

    return passed;
}


bool testSDInterrupt(UART& uart) {
    SD sd;

    *reinterpret_cast<volatile uint32_t*>(sd.event) = 0;
    sd.setInterruptEnable(true, 2);
    sd.control->enableSD = true;

    uint32_t previous = interruptCount[SD_INTERRUPT];
    sd.sendCommand(0, 0);

    bool passed = waitInterrupt(SD_INTERRUPT, previous, 2'000'000) &&
                  ((interruptEvent[SD_INTERRUPT] & (1u << 2)) != 0);

    report(uart, "SD command interrupt", passed);

    return passed;
}
