#ifndef TIMER_CPP
#define TIMER_CPP

#include "../lib/driver/Timer.h"
#include "../lib/mmio.h"
#include "../lib/platform.h"

#define NANO2MILLI_SCALE (uint64_t) (1'000'000 / CLOCK_PERIOD)

#include <inttypes.h>


/****************************************************************/
/*                         CONSTRUCTORS                         */
/****************************************************************/

/**
 * @brief The Timer constructor initializes the timer with default values and disables counting and interrupt
 * generation.
 * 
 * @param timerNumber An integer value that represents the specific timer
 * id in the system. It is used to calculate the base address of the timer and configure the timer accordingly.
 */
Timer::Timer(uint32_t timerNumber) : 
    baseAddress   ( (uint32_t *) (TIMER_BASE + (timerNumber * 8)) ),
    threshold     ( (uint64_t *) baseAddress                      ),
    value         ( (uint64_t *) (baseAddress + 2)                ),
    pwmToggle     ( (uint64_t *) (baseAddress + 4)                ),
    configuration ( (timerConfig_s *) (baseAddress + 6)           ),
    interruptConfiguration ( (timerIrqConfig_s *) (baseAddress + 7) ) {
    
    /* Disable timer counting and interrupt generation */
    configuration->enableTimer = false;
    interruptConfiguration->interruptEnable = false;
    clearInterrupt();

    /* Count from zero to maximum 64 bit integer */
    *value = 0L;
    *threshold = TIMER_MAX_TIME;

    /* Default configuration */
    configuration->timerMode = FREE_RUNNING;
};



/**
 * @brief The Timer destructor disables the timer, resets its value and threshold, and sets the timer mode to FREE_RUNNING.
 */
Timer::~Timer() {
    /* Disable timer counting and interrupt generation */
    configuration->enableTimer = false;
    interruptConfiguration->interruptEnable = false;
    clearInterrupt();

    /* Count from zero to maximum 64 bit integer */
    *value = 0L;
    *threshold = TIMER_MAX_TIME;

    /* Default configuration */
    configuration->timerMode = FREE_RUNNING;
}


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

/**
 * @brief Initializes a Timer pin with specified parameters, disable interrupts.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::init(uint64_t threshold, timerMode_e mode) {
    /* Disable interrupts to avoid any undesired interrupt */
    interruptConfiguration->interruptEnable = false;
    clearInterrupt();

    /* User defined configuration */
    *this->threshold = threshold;
    configuration->timerMode = mode;

    /* Reset timer value and enable interrupts */
    *value = 0L;

    return *this;
}



/*****************************************************************/
/*                              PWM                              */
/*****************************************************************/

Timer& Timer::pwm(uint64_t limit, uint64_t toggle) {
    /* Free running to not halt after reaching the limit */
    configuration->timerMode = FREE_RUNNING;
    interruptConfiguration->interruptEnable = false;

    /* Set limits */
    *this->threshold = limit;
    *this->pwmToggle = toggle;

    /* Start timer */
    configuration->enablePWM = true;
    configuration->enableTimer = true;
    configuration->halted = false;

    return *this;
}



/*****************************************************************/
/*                            UTILITY                            */
/*****************************************************************/

/**
 * @brief Enable timer: start incrementing. The timer needs to be in 
 * ONE_SHOT mode, otherwise the function never returns.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::delay(uint64_t millis) {
    setTimerMode(ONE_SHOT);

    /* Stop and reset timer count */
    stop();
    setTime(0);

    /* Setup threshold and start */
    setThreshold(millis * (SYSTEM_FREQUENCY / 1000));
    start();
    
    /* Wait until timer halts */
    while (!getConfiguration()->halted) {  }

    return *this;
};


/**
 * @brief Clear the timer counter and starts incrementing again.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::restart() {
    /* Clear timer counter */
    *value = 0;

    /* Start incrementing again */
    configuration->halted = false;
    configuration->enableTimer = true;

    return *this;
}


/**
 * @brief Get the time elapsed in milliseconds.
 * 
 * @return Return a 64 bit integer rapresenting the number of milliseconds.
 */
uint64_t Timer::getMillis() const {
    uint64_t timerValue = *this->value;

    /* 1ms is 10^6 ns, however the timer might increment once Xns, 
     * then divide the divisor value for the incrementing period */
    return timerValue / NANO2MILLI_SCALE;
};



/**
 * @brief Returns the elapsed time in different formats (nanoseconds, milliseconds,
 * seconds, minutes, or hours) based on the input format.
 * 
 * @param format Used to specify the desired format for the time elapsed.values for "format" are:
 * 
 * @return a float value representing the time elapsed in the specified format.
 */
uint64_t Timer::timeElapsed(timeFormat_e format) const {
    uint64_t timerValue = *this->value;

    switch (format) {
        case NANO:
            return timerValue;
        break;

        case MILLI:
            return timerValue / (SYSTEM_FREQUENCY / 1000);
        break;

        case SECONDS:
            return timerValue / SYSTEM_FREQUENCY;
        break;

        case MINUTES:
            return (timerValue / SYSTEM_FREQUENCY) / 60;
        break;

        case HOURS:
            return (timerValue / SYSTEM_FREQUENCY) / 3600;
        break;
        
        default:
            return 0;
        break;
    }
}

#endif
