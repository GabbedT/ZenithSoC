/* Timer base address */
#define TIMER_BASE 0x0000_0000

/* Compare registers offsets */
#define TIMER_COMPARE_LOW 0
#define TIMER_COMPARE_HIGH 1

/* Value registers offsets */
#define TIMER_VALUE_LOW 2
#define TIMER_VALUE_HIGH 3

/* Configuration register offset */
#define TIMER_CONFIG 4

/* Configuration register fields */
#define ENABLE_TIMER 0
#define TIMER_MODE 1
#define TIMER_INTERRUPT 2
#define TIMER_RESTART 3
