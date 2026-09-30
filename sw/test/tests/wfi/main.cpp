/*
 * How this test works:
 * Arm a one-shot timer for 1 ms, enable interrupts, and execute WFI.
 * Returning too early fails the timer checks; never waking hits the simulator
 * timeout. A valid wakeup must record exactly one timer interrupt.
 */
#include <stdint.h>

#include "driver/Timer.h"
#include "interrupt.h"

namespace {

constexpr uint64_t ONE_MILLISECOND = 100'000;

}

extern "C" int main() {
    Timer timer;

    /* Keep the interrupt masked until every timer register is coherent. */
    asm volatile ("csrc mstatus, %0" :: "r"(1 << 3) : "memory");
    const uint32_t previous = interruptCount[TIMER_INTERRUPT];

    timer.stop()
         .setTime(0)
         .setThreshold(ONE_MILLISECOND)
         .setTimerMode(Timer::ONE_SHOT)
         .clearInterrupt()
         .setInterrupt(true)
         .start();

    asm volatile ("fence" ::: "memory");
    asm volatile ("csrs mstatus, %0" :: "r"(1 << 3) : "memory");

    /* If WFI is incorrectly implemented as a NOP, the checks immediately
     * below execute before the 1 ms compare interrupt and fail. If it never
     * wakes, the simulator's MAX_CYCLES guard fails the run. */
    asm volatile ("wfi" ::: "memory");

    const bool passed = interruptCount[TIMER_INTERRUPT] == previous + 1 &&
                        (interruptEvent[TIMER_INTERRUPT] & (1 << 1)) != 0 &&
                        timer.isHalted() &&
                        timer.getTime() >= ONE_MILLISECOND &&
                        unexpectedInterrupts == 0;

    return passed ? 0 : 1;
}
