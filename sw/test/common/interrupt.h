#ifndef TEST_INTERRUPT_H
#define TEST_INTERRUPT_H

#include <stdint.h>

enum interruptVector_e {
    TRACE_INTERRUPT = 0,
    SD_INTERRUPT = 1,
    APU_INTERRUPT = 2,
    ETHERNET_INTERRUPT = 3,
    SPI_INTERRUPT = 4,
    GPIO_INTERRUPT = 5,
    UART_INTERRUPT = 6,
    TIMER_INTERRUPT = 7,
    BUS_ERROR_INTERRUPT = 8,
    INTERRUPT_COUNT = 9
};

extern volatile uint32_t interruptCount[INTERRUPT_COUNT];
extern volatile uint32_t interruptEvent[INTERRUPT_COUNT];
extern volatile uint32_t interruptEntry[INTERRUPT_COUNT];
extern volatile uint32_t unexpectedInterrupts;

extern "C" void interruptDispatch(uint32_t cause);
extern "C" void interruptDispatchVectored(uint32_t cause, uint32_t entry);

bool waitInterrupt(interruptVector_e vector, uint32_t previousCount, uint32_t timeout = 250'000);
void waitCycles(uint32_t cycles);

#endif
