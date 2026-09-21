#include "../lib/driver/SD.h"
#include "../lib/driver/Timer.h"
#include "../lib/driver/UART.h"
#include "boot_config.h"

#include <stdint.h>

#if BOOT_LOGGING
static void boot_print(UART& uart, const char* message) {
    for (const char* c = message; *c != '\0'; ++c) {
        uart.sendByte((uint8_t) *c);
    }
}

static void boot_print_hex32(UART& uart, uint32_t value) {
    static const char digits[] = "0123456789ABCDEF";

    for (int shift = 28; shift >= 0; shift -= 4) {
        uart.sendByte((uint8_t) digits[(value >> shift) & 0xFu]);
    }
}

#else
#define boot_print(uart, message) do { (void) (uart); } while (0)
#define boot_print_sd_debug(uart, card) do { (void) (uart); (void) (card); } while (0)
#endif

static void boot_halt(UART& uart, const char* message) {
    boot_print(uart, message);
    while (1) {  }
}

extern "C" __attribute__((noreturn))
void boot_trap_handler(uint32_t cause, uint32_t epc) {
    UART uart(BOOT_UART_INDEX);
    uart.init(BOOT_BAUD_RATE, false);

    boot_print(uart, "\r\n[BOOT][TRAP] mcause=0x");
#if BOOT_LOGGING
    boot_print_hex32(uart, cause);
#endif
    boot_print(uart, " mepc=0x");
#if BOOT_LOGGING
    boot_print_hex32(uart, epc);
#endif
    boot_print(uart, "\r\n[BOOT][TRAP] Core halted.\r\n");

    while (1) {
        asm volatile ("wfi");
    }
}

extern "C" void boot_sd() {
    UART uart(BOOT_UART_INDEX);
    uart.init(BOOT_BAUD_RATE, false);

    boot_print(uart, "[BOOT] ZenithSoC SD bootloader...\r\n");

    /* The MIG simulation completes calibration in about 104 us and DDR2
     * requires at least 200 us after power-up.  Keep a generous software
     * margin before any boot activity can reach external memory. */
    boot_print(uart, "[BOOT] Waiting for DDR calibration...\r\n");
    Timer timer;
    timer.delay(BOOT_DDR_WAIT_MS);

    SD::errorType_e error = SD::NO_ERROR;
    uint8_t cmd8_response[6] = {0};
    bool high_capacity = false;
    SD card;

    card.init(SD::CLK_400KHZ, SD::BUS_NARROW, cmd8_response, high_capacity, error);

    if (error != SD::NO_ERROR) {
        boot_print_sd_debug(uart, card);
    }

    switch (error) {
        case SD::NO_ERROR:
            break;
        case SD::NO_CARD:
            boot_halt(uart, "[BOOT] No card detected!\r\n");
            break;
        case SD::CMD_CRC_ERR:
            boot_halt(uart, "[BOOT] Command CRC error!\r\n");
            break;
        case SD::CMD_TIMEOUT:
            boot_halt(uart, "[BOOT] Command timeout!\r\n");
            break;
        case SD::DAT_TIMEOUT:
            boot_halt(uart, "[BOOT] Data timeout!\r\n");
            break;
        case SD::CARD_ERR:
            boot_halt(uart, "[BOOT] Card error during initialization!\r\n");
            break;
        default:
            boot_halt(uart, "[BOOT] SD initialization error!\r\n");
            break;
    }

    if (high_capacity) {
        boot_print(uart, "[BOOT] SD ready (high capacity)\r\n");
    } else {
        boot_print(uart, "[BOOT] SD ready (standard capacity)\r\n");
    }

    volatile uint32_t* const load_address = (volatile uint32_t*) BOOT_LOAD_ADDRESS;

    /* SDHC/SDXC use block addresses; SDSC uses byte addresses. */
    const uint32_t first_address = high_capacity ? (uint32_t) BOOT_IMAGE_BLOCK
                                                 : ((uint32_t) BOOT_IMAGE_BLOCK * 512u);

    boot_print(uart, "[BOOT] Loading application");

    for (uint32_t block = 0; block < (uint32_t) BOOT_IMAGE_BLOCKS; ++block) {
        error = SD::NO_ERROR;

        const uint32_t card_address = high_capacity ? (first_address + block)
                                                    : (first_address + (block * 512u));

        card.readBlock(card_address, (uint32_t*) &load_address[128u * block], nullptr, error);

        if (error != SD::NO_ERROR) {
            boot_halt(uart, "\r\n[BOOT] Failed to read application block!\r\n");
        }

#if BOOT_LOGGING
        if ((block & 7u) == 7u) {
            uart.sendByte('.');
        }
#endif
    }

    boot_print(uart, "\r\n[BOOT] Application loaded.\r\n");

    /* First publish and invalidate the cached SD writes.  The diagnostic
     * loads below therefore fetch the actual DDR contents, rather than seeing
     * dirty data that exists only in the CPU cache. */
    asm volatile ("fence rw, rw" ::: "memory");

#if BOOT_LOGGING && (BOOT_DUMP_WORDS > 0)
    boot_dump_ddr(uart, load_address);
#endif

    boot_print(uart, "[BOOT] Jumping to application...\r\n");

    /* The dump refilled clean cache lines.  Invalidate them, and the
     * instruction cache, before entering the freshly loaded image.  No C code
     * may run after this final fence. */
    const uintptr_t entry = (uintptr_t) BOOT_ENTRY_ADDRESS;
    asm volatile ("fence rw, rw\n\t"
                  "jr %0"
                  :: "r" (entry) : "memory");

    __builtin_unreachable();
}
