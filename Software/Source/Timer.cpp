#ifndef TIMER_CPP
#define TIMER_CPP

#include "../Library/Driver/Timer.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

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
    configuration ( (timerConfig_s *) (baseAddress + 4)           ) {
    
    /* Disable timer counting and interrupt generation */
    configuration->enableTimer = false;
    configuration->interruptEnable = false;

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
    configuration->interruptEnable = false;

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
 * @brief Initializes a Timer pin with specified parameters and returns a reference to the GPIO
 * object.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::init(uint64_t threshold, timerMode_e mode) {
    /* Disable interrupts to avoid any undesired interrupt */
    configuration->interruptEnable = false;

    /* User defined configuration */
    *this->threshold = threshold;
    configuration->timerMode = mode;

    /* Reset timer value and enable interrupts */
    *value = 0L;
    configuration->interruptEnable = true;

    return *this;
}

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
volatile struct Timer::timerConfig_s* Timer::getConfiguration() const {
    return configuration;
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
Timer& Timer::delay(uint64_t millis) {
    /* Stop and reset timer count */
    stop();
    setTime(0);

    /* Setup threshold and start */
    setThreshold(millis * 100'000);
    start();
    
    /* Wait until timer halts */
    while (!getConfiguration()->halted) {  }

    return *this;
};


/**
 * @brief Enable timer: start incrementing.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::start() {
    configuration->enableTimer = true;
    configuration->halted = false;

    return *this;
};


/**
 * @brief Disable timer: stop incrementing.
 * 
 * @return The Timer object itself to chain the function call.
 */
Timer& Timer::stop() {
    configuration->enableTimer = false;

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
float Timer::timeElapsed(timeFormat_e format) const {
    float timerValue = (float) getTime();

    switch (format) {
        case NANO:
            return timerValue;
        break;

        case MILLI:
            return timerValue / (1e+6F / CLOCK_PERIOD);
        break;

        case SECONDS:
            return timerValue / (1e+9F / CLOCK_PERIOD);
        break;

        case MINUTES:
            return (timerValue / (1e+9F / CLOCK_PERIOD)) / 60;
        break;

        case HOURS:
            return (timerValue / (1e+9F / CLOCK_PERIOD)) / 3600;
        break;
        
        default:
            return 0.0F;
        break;
    }
}

#endif 