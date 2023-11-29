#include <cstdint>

/* Frequency of the system in Hz */
#define SYSTEM_FREQUENCY 100000000
#define CLOCK_PERIOD 1 / SYSTEM_FREQUENCY

/* Number of timers in the system */
#define TIMER_NUMBER 1

/* Maximum time count */
#define TIMER_MAX_TIME UINT64_MAX

/* Number of GPIO groups */
#define GPIO_GROUP_NUMBER 1

/* Number of UART */
#define UART_NUMBER 1



#define SET_BIT(old, index) old |= 1 << index 

#define CLEAR_BIT(old, index) old |= ~(1 << index) 

