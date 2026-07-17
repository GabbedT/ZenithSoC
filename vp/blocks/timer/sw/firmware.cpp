#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

#include "Timer.h"

extern "C" int main(void) {
    Timer timer(0);

    /* Exit flag */
    bool error = false;

    for (int i = 0; i < 3; ++i) {           /* Timer mode */
        Timer::timerMode_e mode;

        switch (i) {
            case 0: mode = Timer::FREE_RUNNING;
                break;

            case 1: mode = Timer::ONE_SHOT;
                break;

            case 2: mode = Timer::WRAP_AROUND;
                break;

            default: mode = Timer::FREE_RUNNING;
                break;
        }

        timer.init(32, mode).setInterrupt(true).start();

        int timeout = 0;

        while (!timer.interruptConfiguration->interruptPending) { 
            ++timeout;

            if (timeout > 40) {
                vp_println("[ERROR] Interrupt timeout");
                TEST_FAIL();
            }
        }

        error = timer.getThreshold() != 32;
        error |= timer.getConfiguration()->timerMode != mode;

        if (i == Timer::ONE_SHOT) {
            error |= !timer.isHalted();
        } else {
            error |= timer.isHalted();
        }

        if (error) {
            vp_println("[ERROR] Timer mode or interrupt error");
            vp_print_hex(i);
            vp_putchar(' ');
            vp_print_hex(timer.getThreshold());
            vp_putchar(' ');
            vp_print_hex(timer.getConfiguration()->timerMode);
            vp_putchar(' ');
            vp_print_hex(timer.isHalted());
            vp_putchar('\n');
            TEST_FAIL();
        }

        timer.setInterrupt(false).clearInterrupt().stop();

        for (int j = 0; j < 4; ++j) {
            timer.getTime();
        }

        if (timer.interruptConfiguration->interruptPending) {
            vp_println("[ERROR] Timer interrupt was not cleared");
            TEST_FAIL();
        }
    }

    /* Start from a known value and verify that the free running timer increments */
    timer.init(UINT64_MAX, Timer::FREE_RUNNING).setTime(10).start();
    uint64_t startTime = timer.getTime();
    uint64_t stopTime = timer.stop().getTime();

    if (stopTime < startTime) {
        vp_println("[ERROR] Timer did not count");
        TEST_FAIL();
    }

    /* Clears the counter */
    timer.setTimerMode(Timer::FREE_RUNNING).setTime(12).restart();

    /* PWM configures a free running timer with PWM output enabled */
    timer.stop();
    timer.pwm(32, 16);

    if (!timer.getConfiguration()->enablePWM ||
        timer.getConfiguration()->timerMode != Timer::FREE_RUNNING) {
        vp_println("[ERROR] PWM configuration error");
        TEST_FAIL();
    }

    /* A zero delay must halt immediately and leave the timer at zero */
    timer.stop().setTime(0);
    timer.delay(0);

    if (!timer.isHalted() || timer.getMillis() != 0 ||
        timer.timeElapsed(Timer::NANO) != 0 ||
        timer.timeElapsed(Timer::MILLI) != 0 ||
        timer.timeElapsed(Timer::SECONDS) != 0 ||
        timer.timeElapsed(Timer::MINUTES) != 0 ||
        timer.timeElapsed(Timer::HOURS) != 0) {
        vp_println("[ERROR] Timer utility error");
        TEST_FAIL();
    }

    TEST_PASS();
}
