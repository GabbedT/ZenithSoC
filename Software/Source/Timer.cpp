#ifndef TIMER_CPP
#define TIMER_CPP

#include "../Library/Driver/Timer.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <stdint.h>


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
    baseAddress   ( (uint32_t *) TIMER_BASE + (timerNumber * 5) ),
    value         ( (uint64_t *) baseAddress                    ),
    threshold     ( (uint64_t *) baseAddress + 2                ),
    configuration ( (timerConfig_s *) baseAddress + 4          ) {
    
    /* Count from zero to maximum 64 bit integer */
    *value = 0;
    *threshold = TIMER_MAX_TIME;

    /* Disable timer counting and interrupt generation */
    configuration->enableTimer = 0;
    configuration->interruptEnable = 0;

    /* Default configuration */
    configuration->timerMode = FREE_RUNNING;
};



/**
 * @brief The Timer constructor initializes the timer with the given threshold value and mode.
 * 
 * @param timerNumber An integer value that represents the specific timer
 * id in the system. It is used to calculate the base address of the timer and configure the timer accordingly.
 * @param threshold The threshold parameter is a 64 bit value that represents the maximum value that
 * the timer will count up to before triggering an interrupt.
 * @param mode It represents the action performed by the timer once it reaches the threshold value.
 */
Timer::Timer(uint32_t timerNumber, uint64_t threshold, timerMode_e mode) : 
    baseAddress   ( (uint32_t *) TIMER_BASE + (timerNumber * 5) ),
    value         ( (uint64_t *) baseAddress                    ),
    threshold     ( (uint64_t *) baseAddress + 8                ),
    configuration ( (timerConfig_s *) baseAddress + 16          ) {
    
    /* Count from zero to threshold */
    *value = 0;
    *this->threshold = threshold;

    configuration->timerMode = mode;
};



/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

/**
 * @brief Sets or clear the interrupt enable flag in the Timer configuration.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::setInterrupt(bool enable) {
    configuration->interruptEnable = enable;

    return *this;
};


/**
 * @brief The function sets the timer mode.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::setTimerMode(timerMode_e mode) {
    configuration->timerMode = mode;    

    return *this;
};



/**
 * @brief Returns the current configuration register of the Timer object.
 * 
 * @return Returns the current configuration register of the Timer object.
 */
struct Timer::timerConfig_s Timer::getConfiguration() const {
    return *configuration;
};



/*****************************************************************/
/*                             VALUE                             */
/*****************************************************************/

/**
 * @brief Sets the value of the Timer to a specified time unit count.
 * 
 * @param timeCount Time count based on the timer increment rate.
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::setTime(uint64_t timeCount) {
    *value = timeCount;

    return *this;
};



/**
 * @brief Read the 64 bit timer current value.
 * 
 * @return Return the value register.
 */
uint64_t Timer::getTime() const {
    return *value;
};


/*****************************************************************/
/*                           THRESHOLD                           */
/*****************************************************************/

/**
 * @brief Sets the threshold value for the Timer object, the threshold serves as a limit that
 * once reached, the timer generate an interrupt.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::setThreshold(uint64_t threshold) {
    *this->threshold = threshold;

    return *this;
};


/**
 * @brief Read the 64 bit compare register.
 * 
 * @return The value stored in the compare register.
 */
uint64_t Timer::getThreshold() const {
    return *threshold;
};


/*****************************************************************/
/*                            UTILITY                            */
/*****************************************************************/

/**
 * @brief Enable timer: start incrementing.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::start() {
    configuration->enableTimer = 1;

    return *this;
};


/**
 * @brief Disable timer: stop incrementing.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::stop() {
    configuration->enableTimer = 0;

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

    /* Start incrementin again */
    configuration->restart = 1;

    return *this;
}


/**
 * @brief Get the time elapsed in milliseconds.
 * 
 * @return Return a 64 bit integer rapresenting the number of milliseconds.
 */
uint64_t Timer::getMillis() const {
    uint64_t timerValue = getTime();

    /* 1ms is 10^6 ns, however the timer might increment once Xns, 
     * then divide the divisor value for the incrementing period */
    return timerValue / (1'000'000 / CLOCK_PERIOD);
};



/**
 * @brief Returns the elapsed time in different formats (nanoseconds, milliseconds,
 * seconds, minutes, or hours) based on the input format.
 * 
 * @param format Used to specify the desired format for the time elapsed.values for "format" are:
 * 
 * @return a float value representing the time elapsed in the specified format.
 */
float Timer::timeElapsed(timeFormat_e format) const {
    float timerValue = (float) getTime();

    switch (format) {
        case NANO:
            return timerValue;
        break;

        case MILLI:
            return timerValue / (1e+6 / CLOCK_PERIOD);
        break;

        case SECONDS:
            return timerValue / (1e+9 / CLOCK_PERIOD);
        break;

        case MINUTES:
            return (timerValue / (1e+9 / CLOCK_PERIOD)) / 60;
        break;

        case HOURS:
            return (timerValue / (1e+9 / CLOCK_PERIOD)) / 3600;
        break;
        
        default:
            return 0.0F;
        break;
    }
}

#endif 