#include <stdint.h>
#include <stdarg.h>
#include "boardsupport.h"

volatile uint64_t embench_start_cycle = 0;
volatile uint64_t embench_stop_cycle  = 0;
volatile uint64_t embench_cycles      = 0;


static void uart_putc(char c) {
    volatile struct uartCtrlStatus_s* status = (volatile struct uartCtrlStatus_s*) (UART_BASE);

    volatile uint8_t* txbuf = (volatile uint8_t*) (UART_BASE + 0x4);

    while (status->fullTX) {
        /* wait */
    }

    *txbuf = c;
}

static void uart_puts(const char* s) {
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r');
        }

        uart_putc(*s);
        s++;
    }
}

static void uart_put_u64(uint64_t v) {
    char buf[21];
    int i = 0;

    if (v == 0) {
        uart_putc('0');
        return;
    }

    while (v > 0) {
        buf[i++] = '0' + (v % 10);
        v /= 10;
    }

    while (i > 0) {
        uart_putc(buf[--i]);
    }
}


static inline uint32_t read_cycle_lo(void) {
    uint32_t v;
    __asm__ volatile ("rdcycle %0" : "=r"(v));
    return v;
}

static inline uint32_t read_cycle_hi(void) {
    uint32_t v;
    __asm__ volatile ("rdcycleh %0" : "=r"(v));
    return v;
}

static inline uint64_t read_cycle64(void) {
    uint32_t hi0, lo, hi1;

    do {
        hi0 = read_cycle_hi();
        lo  = read_cycle_lo();
        hi1 = read_cycle_hi();
    } while (hi0 != hi1);

    return ((uint64_t) hi0 << 32) | lo;
}

void initialise_board(void) {
    volatile struct uartCtrlStatus_s* status = (volatile struct uartCtrlStatus_s*) (UART_BASE);

    /* Initialize UART */
    status->parityEnable = 1;
    status->dataBits = BIT8;
    status->stopBits = STOP1;
    status->parityMode = EVEN;
    status->clockDivider = (CLK_FREQUENCY / (BAUDRATE * 16)) - 1;
    status->enableTX = 1;
}

void initialize_board(void) {
    initialise_board();
}

void start_trigger(void) {
    uart_puts("===================================\n"  );
    uart_puts("       STARTING BENCHMARK\n"            );
    uart_puts("===================================\n\n");

    __asm__ volatile ("" ::: "memory");
    embench_start_cycle = read_cycle64();
}

void stop_trigger(void) {
    embench_stop_cycle = read_cycle64();
    embench_cycles = embench_stop_cycle - embench_start_cycle;

    uart_puts("EMBENCH_CYCLES=");
    uart_put_u64(embench_cycles);
    uart_puts("\n");
    
    uart_puts("===================================\n"  );
    uart_puts("         ENDING BENCHMARK\n"            );
    uart_puts("===================================\n\n");

    __asm__ volatile ("" ::: "memory");
}