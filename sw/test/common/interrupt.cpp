#include "interrupt.h"

#include "mmio.h"

volatile uint32_t interruptCount[INTERRUPT_COUNT] = {0};
volatile uint32_t interruptEvent[INTERRUPT_COUNT] = {0};
volatile uint32_t interruptEntry[INTERRUPT_COUNT] = {0};
volatile uint32_t unexpectedInterrupts = 0;

extern "C" uint32_t tohost;

namespace {

    inline volatile uint32_t& reg32(uint32_t address) {
        return *reinterpret_cast<volatile uint32_t*>(address);
    }

}


extern "C" void interruptDispatch(uint32_t cause) {
    /* A synchronous exception means the test can no longer be trusted */
    if ((cause & 0x80000000) == 0) {
        ++unexpectedInterrupts;
        tohost = (0x7F << 1) | 1;

        return;
    }

    uint32_t vector = cause & 0xFF;

    if (vector >= INTERRUPT_COUNT) {
        ++unexpectedInterrupts;

        return;
    }

    switch (vector) {
        case TRACE_INTERRUPT:
            interruptEvent[vector] = reg32(TRACE_UNIT_BASE);
            reg32(TRACE_UNIT_BASE + 4) &= ~0x3;
        break;

        case SD_INTERRUPT:
            interruptEvent[vector] = reg32(SD_BASE + (4 * 4));
            reg32(SD_BASE) &= ~0x1FF;
            reg32(SD_BASE + (4 * 4)) = 0;
        break;

        case APU_INTERRUPT:
            interruptEvent[vector] = reg32(APU_BASE + (7 * 4));
            reg32(APU_BASE + 4) &= ~((0x3F << 7) | (1 << 16));
            reg32(APU_BASE + (7 * 4)) = 0;
        break;

        case ETHERNET_INTERRUPT:
            interruptEvent[vector] = reg32(ETHERNET_BASE + (39 * 4));
            reg32(ETHERNET_BASE + (32 * 4)) &= ~(0xF << 13);
            reg32(ETHERNET_BASE + (39 * 4)) = 0;
        break;

        case SPI_INTERRUPT:
            interruptEvent[vector] = reg32(SPI_BASE + (3 * 4));
            reg32(SPI_BASE) &= ~(1 << 5);
            reg32(SPI_BASE + (3 * 4)) = 0;
        break;

        case GPIO_INTERRUPT:
            interruptEvent[vector] = reg32(GPIO_BASE + (5 * 4));
            reg32(GPIO_BASE + (2 * 4)) = 0;
            reg32(GPIO_BASE + (5 * 4)) = 0;
        break;

        case UART_INTERRUPT:
            interruptEvent[vector] = reg32(UART_BASE + (3 * 4));
            reg32(UART_BASE) &= ~(0x1F << 4);
            reg32(UART_BASE + (3 * 4)) = 0;
        break;

        case TIMER_INTERRUPT:
            interruptEvent[vector] = reg32(TIMER_BASE + (7 * 4));
            reg32(TIMER_BASE + (7 * 4)) = 1 << 1;
        break;

        case BUS_ERROR_INTERRUPT:
            ++unexpectedInterrupts;
        break;
    }

    ++interruptCount[vector];
}


extern "C" void interruptDispatchVectored(uint32_t cause, uint32_t entry) {
    uint32_t vector = cause & 0xFF;

    if ((cause & 0x80000000) && (vector < INTERRUPT_COUNT)) {
        interruptEntry[vector] = entry;

        if (entry != vector) {
            ++unexpectedInterrupts;
        }
    }

    interruptDispatch(cause);
}


bool waitInterrupt(interruptVector_e vector, uint32_t previousCount, uint32_t timeout) {
    while (timeout--) {
        if (interruptCount[vector] != previousCount) {
            return true;
        }

        asm volatile ("nop");
    }

    return false;
}


void waitCycles(uint32_t cycles) {
    while (cycles--) {
        asm volatile ("nop");
    }
}
