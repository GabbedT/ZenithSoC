#include <stdint.h>

#ifndef TIMER_CPP

#define TIMER_CPP

class Timer {

private:

    /* Track the number of instances created */
    static int instanceNumber;

    /* Save the timer id */
    int timerID;

    /* Timer register containing the count value */
    uint64_t* value;

    /* Timer register containing the interrupt value, 
     * once the value reach the threshold an interrupt
     * in generated */
    uint64_t* threshold;

    /* Configuration register */
    uint32_t* configuration;

public:

    enum timerMode_e { FREE_RUNNING, ONE_SHOT };

    /* Constructor: increment the instance number value,
     * initialize the timer ID and the registers */
    Timer(uint64_t threshold, timerMode_e mode);

    /* Deconstructor: frees memory and decrement the instance number value */
    ~Timer();

/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    /* Start timer */
    Timer* start();

    /* Stop timer */
    Timer* stop();

    /* Restart timer after interrupting due to the reach of the threshold 
     * (only in ONE_HOT mode)*/
    Timer* restart();

    /* Set interrupt generation */
    Timer* setInterrupt(bool enable);

    /* Set timer mode */
    Timer* setTimerMode(timerMode_e mode);

    /* Read configuration register */
    uint32_t getConfiguration();


/*****************************************************************/
/*                             VALUE                             */
/*****************************************************************/

    /* Set timer value */
    Timer* setTime(uint64_t timeCount);
    
    /* Read counted time value */
    uint64_t getTime() const;


/*****************************************************************/
/*                           THRESHOLD                           */
/*****************************************************************/

    /* Set interrupt threshold value */
    Timer* setThreshold(uint64_t threshold);

    /* Read interrupt threshold value */
    uint64_t getThreshold() const;


/*****************************************************************/
/*                            UTILITY                            */
/*****************************************************************/

    /* Get time in milliseconds */
    uint64_t getMillis() const;
};

#endif