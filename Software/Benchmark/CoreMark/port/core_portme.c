/*
Copyright 2018 Embedded Microprocessor Benchmark Consortium (EEMBC)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Original Author: Shay Gal-on
*/
#include "coremark.h"
#include "core_portme.h"

#include "../../../Library/Driver/UART.h"


/* CSR Readings */
static inline uint64_t rd_cycle64(void) {
    uint32_t hi, hi2, lo;

    do {
        __asm__ volatile ("csrr %0, cycleh" : "=r"(hi));
        __asm__ volatile ("csrr %0, cycle"  : "=r"(lo));
        __asm__ volatile ("csrr %0, cycleh" : "=r"(hi2));
    } while (hi != hi2);

    return ((uint64_t) hi << 32) | lo;
}

static inline uint64_t rd_instret64(void) {
    uint32_t hi, hi2, lo;

    do {
        __asm__ volatile ("csrr %0, instreth" : "=r"(hi));
        __asm__ volatile ("csrr %0, instret"  : "=r"(lo));
        __asm__ volatile ("csrr %0, instreth" : "=r"(hi2));
    } while (hi != hi2);

    return ((uint64_t) hi << 32) | lo;
}

#if VALIDATION_RUN
volatile ee_s32 seed1_volatile = 0x3415;
volatile ee_s32 seed2_volatile = 0x3415;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PERFORMANCE_RUN
volatile ee_s32 seed1_volatile = 0x0;
volatile ee_s32 seed2_volatile = 0x0;
volatile ee_s32 seed3_volatile = 0x66;
#endif
#if PROFILE_RUN
volatile ee_s32 seed1_volatile = 0x8;
volatile ee_s32 seed2_volatile = 0x8;
volatile ee_s32 seed3_volatile = 0x8;
#endif
volatile ee_s32 seed4_volatile = ITERATIONS;
volatile ee_s32 seed5_volatile = 0;
/* Porting : Timing functions
        How to capture time and convert to seconds must be ported to whatever is
   supported by the platform. e.g. Read value from on board RTC, read value from
   cpu clock cycles performance counter etc. Sample implementation for standard
   time.h and windows.h definitions included.
*/
CORETIMETYPE
barebones_clock()
{
#error \
    "You must implement a method to measure time in barebones_clock()! This function should return current time.\n"
}
/* Define : TIMER_RES_DIVIDER
        Divider to trade off timer resolution and total time that can be
   measured.

        Use lower values to increase resolution, but make sure that overflow
   does not occur. If there are issues with the return value overflowing,
   increase this value.
        */
#define GETMYTIME(_t)              (*_t = barebones_clock())
#define MYTIMEDIFF(fin, ini)       ((fin) - (ini))
#define TIMER_RES_DIVIDER          1
#define SAMPLE_TIME_IMPLEMENTATION 1
#define EE_TICKS_PER_SEC           (CLOCKS_PER_SEC / TIMER_RES_DIVIDER)

/** Define Host specific (POSIX), or target specific global time variables. */
static CORETIMETYPE start_time_val, stop_time_val;

static uint64_t g_start_cyc, g_stop_cyc;
static uint64_t g_start_ins, g_stop_ins;

uint64_t cycles() { return g_stop_cyc - g_start_cyc; };
uint64_t instret() { return g_stop_ins - g_start_ins; };

/* Function : start_time
        This function will be called right before starting the timed portion of
   the benchmark.

        Implementation may be capturing a system timer (as implemented in the
   example code) or zeroing some system parameters - e.g. setting the cpu clocks
   cycles to 0.
*/
void start_time(void) {
    g_start_ins = rd_instret64();
    g_start_cyc = rd_cycle64();
}
/* Function : stop_time
        This function will be called right after ending the timed portion of the
   benchmark.

        Implementation may be capturing a system timer (as implemented in the
   example code) or other system parameters - e.g. reading the current value of
   cpu cycles counter.
*/
void stop_time(void) {
    g_stop_ins = rd_instret64();
    g_stop_cyc = rd_cycle64();
}
/* Function : get_time
        Return an abstract "ticks" number that signifies time on the system.

        Actual value returned may be cpu cycles, milliseconds or any other
   value, as long as it can be converted to seconds by <time_in_secs>. This
   methodology is taken to accommodate any hardware or simulated platform. The
   sample implementation returns millisecs by default, and the resolution is
   controlled by <TIMER_RES_DIVIDER>
*/
CORE_TICKS get_time(void) {
    return (CORE_TICKS) (g_stop_cyc - g_start_cyc);
}
/* Function : time_in_secs
        Convert the value returned by get_time to seconds.

        The <secs_ret> type is used to accommodate systems with no support for
   floating point. Default implementation implemented by the EE_TICKS_PER_SEC
   macro above.
*/
secs_ret
time_in_secs(CORE_TICKS ticks)
{
    secs_ret retval = ((secs_ret)ticks) / (secs_ret)EE_TICKS_PER_SEC;
    return (secs_ret) ((float) ticks / (float) CLK_FREQUENCY);
}

ee_u32 default_num_contexts = 1;

/* Function : portable_init
        Target specific initialization code
        Test for some common mistakes.
*/
void
portable_init(core_portable *p, int *argc, char *argv[])
{

    g_uart.init(115200, true, UART::EVEN, UART::STOP1, UART::BIT8);

    (void)argc; // prevent unused warning
    (void)argv; // prevent unused warning

    if (sizeof(ee_ptr_int) != sizeof(ee_u8 *))
    {
        ee_printf(
            "ERROR! Please define ee_ptr_int to a type that holds a "
            "pointer!\n");
    }
    if (sizeof(ee_u32) != 4)
    {
        ee_printf("ERROR! Please define ee_u32 to a 32b unsigned type!\n");
    }
    p->portable_id = 1;
}
/* Function : portable_fini
        Target specific final code
*/
void
portable_fini(core_portable *p)
{
    ee_printf("CYCLES=%lu\n", (unsigned long) cycles());
    ee_printf("INSTRET=%lu\n", (unsigned long) instret());

    uint64_t c = cycles(); uint64_t i = instret();

    if (i) {
        ee_printf("CPIx1000=%lu\n", (unsigned long) ((c * 1000) / i));
    }
    
    p->portable_id = 0;
}
