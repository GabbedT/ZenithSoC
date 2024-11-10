#include <cstdint>

/* Frequency of the system in Hz */
#define SYSTEM_FREQUENCY 100000000
#define CLOCK_PERIOD 10

/* Number of timers in the system */
#define TIMER_NUMBER 1

/* Maximum time count */
#define TIMER_MAX_TIME -1

/* Number of GPIO groups */
#define GPIO_GROUP_NUMBER 1

/* Number of UART */
#define UART_NUMBER 1

/* The UART ID for debugging */
#define DEBUG_UART 0

/* SPI slave number */
#define SPI_SLAVES 1

/* Size of non cachable memory */
#define NC_MEMORY_SIZE 2048


#define SET_BIT(old, index) old |= 1 << index 

#define CLEAR_BIT(old, index) old |= ~(1 << index) 

