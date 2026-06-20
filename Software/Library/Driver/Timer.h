#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

class Timer {

public:

    /* Timer Mode:
     * 
     * FREE_RUNNING: After interrupt, the timer keeps incrementing
     * ONE_SHOT: After interrupt, the timer stops incrementing */
    enum timerMode_e { FREE_RUNNING, ONE_SHOT };

    /* Time format */
    enum timeFormat_e { HOURS, MINUTES, SECONDS, MILLI, NANO };

    /* Configuration register fields */
    struct timerConfig_s {
        unsigned int enableTimer : 1;
        unsigned int enablePWM : 1;
        unsigned int timerMode : 1;
        unsigned int interruptEnable : 1;
        unsigned int halted : 1;
        unsigned int padding : 28;
    };



    /* Base memory address */
    uint32_t* const baseAddress;

    /* Timer register containing the count value */
    volatile uint64_t* const value;

    /* Timer register containing the interrupt value, 
     * once the value reach the threshold an interrupt
     * in generated */
    volatile uint64_t* volatile const threshold;

    /* PWM toggle limit */
    volatile uint64_t* volatile const pwmToggle;

    /* Configuration register */
    volatile struct timerConfig_s* volatile const configuration;



    /**
     * The Timer constructor initializes the timer with default values and disables counting and interrupt
     * generation.
     * 
     * @param timerNumber The timerNumber parameter is an integer value that represents the specific timer
     * id in the system. It is used to calculate the base address of the timer and configure the timer accordingly.
     */
    Timer(uint32_t timerNumber = 0);

    /* Deconstructor */
    ~Timer();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    Timer& init(uint64_t threshold, timerMode_e mode);

    /**
     * @brief Sets or clear the interrupt enable flag in the Timer configuration.
     * 
     * @return The Timer object itself to chain the function call.
     */
    inline Timer& setInterrupt(bool enable) {
        configuration->interruptEnable = enable;

        return *this;
    };

    /**
     * @brief The function sets the timer mode.
     * 
     * @return The Timer object itself to chain the function call.
     */
    inline Timer& setTimerMode(timerMode_e mode) {
        configuration->timerMode = mode;    

        return *this;
    };

    /**
     * @brief Returns the current configuration register of the Timer object.
     * 
     * @return Pointer to the status register.
     */
    inline volatile struct timerConfig_s* getConfiguration() const {
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
    inline Timer& setTime(uint64_t timeCount) {
        *value = timeCount;

        return *this;
    };
    
    /**
     * @brief Read the 64 bit timer current value.
     * 
     * @return Return the value register.
     */
    inline uint64_t getTime() const {
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
    inline Timer& setThreshold(uint64_t threshold) {
        *this->threshold = threshold;

        return *this;
    };

    /**
     * @brief Read the 64 bit compare register.
     * 
     * @return The value stored in the compare register.
     */
    inline uint64_t getThreshold() const {
        return *threshold;
    };


/*****************************************************************/
/*                              PWM                              */
/*****************************************************************/

    /**
     * @brief Set timer to FREE_RUNNING, disable interrupts and enable PWM output
     * 
     * @param limit Upper limit, here timer value resets to 0
     * @param toggle Middle limit, after this PWM output toggles from 1 to 0
     * @return The Timer object itself to chain the function call.
     */
    Timer& pwm(uint64_t limit, uint64_t toggle);


/*****************************************************************/
/*                            UTILITY                            */
/*****************************************************************/

    Timer& delay(uint64_t millis);

    /**
     * @brief Enable timer: start incrementing.
     * 
     * @return The Timer object itself to chain the function call.
     */
    inline Timer& start() {
        configuration->enableTimer = true;
        configuration->halted = false;

        return *this;
    };

    /**
     * @brief Disable timer: stop incrementing.
     * 
     * @return The Timer object itself to chain the function call.
     */
    inline Timer& stop() {
        configuration->enableTimer = false;

        return *this;
    };

    /**
     * @brief Clear the timer counter and starts incrementing again. Valid only
     * if the counter is in ONE_HOT mode.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& restart();

    /**
     * @brief Return the status of the timer.
     * 
     * @return Halted bit of the status register.
     */
    inline bool isHalted() {
        return configuration->halted;
    };

    /**
     * @brief Get the time elapsed in milliseconds.
     * 
     * @return Return a 64 bit integer rapresenting the number of milliseconds.
     */
    uint64_t getMillis() const;

    /**
     * @brief Returns the elapsed time in different formats (nanoseconds, milliseconds,
     * seconds, minutes, or hours) based on the input format.
     * 
     * @param format Used to specify the desired format for the time elapsed.values for "format" are:
     * 
     * @return a float value representing the time elapsed in the specified format.
     */
    uint64_t timeElapsed(timeFormat_e format) const;
};

#endif