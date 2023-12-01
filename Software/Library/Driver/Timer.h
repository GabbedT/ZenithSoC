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
        unsigned int timerMode : 1;
        unsigned int interruptEnable : 1;
        unsigned int halted : 1;
        unsigned int padding : 28;
    };


private:

    /* Base memory address */
    uint32_t* const baseAddress;

    /* Timer register containing the count value */
    uint64_t* volatile const value;

    /* Timer register containing the interrupt value, 
     * once the value reach the threshold an interrupt
     * in generated */
    uint64_t* volatile const threshold;

    /* Configuration register */
    timerConfig_s* volatile const configuration;


public:

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
    Timer& setInterrupt(bool enable);

    /**
     * @brief The function sets the timer mode.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& setTimerMode(timerMode_e mode);

    /**
     * @brief Returns the current configuration register of the Timer object.
     * 
     * @return Returns the current configuration register of the Timer object.
     */
    struct timerConfig_s getConfiguration() const;


/*****************************************************************/
/*                             VALUE                             */
/*****************************************************************/

    /**
     * @brief Sets the value of the Timer to a specified time unit count.
     * 
     * @param timeCount Time count based on the timer increment rate.
     * @return The Timer object itself to chain the function call.
     */
    Timer& setTime(uint64_t timeCount);
    
    /**
     * @brief Read the 64 bit timer current value.
     * 
     * @return Return the value register.
     */
    uint64_t getTime() const;


/*****************************************************************/
/*                           THRESHOLD                           */
/*****************************************************************/

    /**
     * @brief Sets the threshold value for the Timer object, the threshold serves as a limit that
     * once reached, the timer generate an interrupt.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& setThreshold(uint64_t threshold);

    /**
     * @brief Read the 64 bit compare register.
     * 
     * @return The value stored in the compare register.
     */
    uint64_t getThreshold() const;


/*****************************************************************/
/*                            UTILITY                            */
/*****************************************************************/

    /**
     * @brief Enable timer: start incrementing.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& start();

    /**
     * @brief Disable timer: stop incrementing.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& stop();

    /**
     * @brief Clear the timer counter and starts incrementing again. Valid only
     * if the counter is in ONE_HOT mode.
     * 
     * @return The Timer object itself to chain the function call.
     */
    Timer& restart();


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
    float timeElapsed(timeFormat_e format) const;
};

#endif