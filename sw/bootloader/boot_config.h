#ifndef ZENITH_BOOT_CONFIG_H
#define ZENITH_BOOT_CONFIG_H

/*
 * Defaults for the shared SD-card bootloader.
 *
 * The Makefile normally overrides these values for each application.  They
 * are kept here as usable defaults as well, so the bootloader sources remain
 * easy to reuse from another build system.
 */

#ifndef BOOT_IMAGE_BLOCK
#define BOOT_IMAGE_BLOCK 0x2000u
#endif

#ifndef BOOT_IMAGE_BLOCKS
#define BOOT_IMAGE_BLOCKS 64u
#endif

#ifndef BOOT_LOAD_ADDRESS
#define BOOT_LOAD_ADDRESS 0x80000000u
#endif

#ifndef BOOT_ENTRY_ADDRESS
#define BOOT_ENTRY_ADDRESS BOOT_LOAD_ADDRESS
#endif

#ifndef BOOT_UART_INDEX
#define BOOT_UART_INDEX 0u
#endif

#ifndef BOOT_BAUD_RATE
#define BOOT_BAUD_RATE 115200u
#endif

/* Conservative delay for DDR2 power-up and MIG calibration. */
#ifndef BOOT_DDR_WAIT_MS
#define BOOT_DDR_WAIT_MS 50u
#endif

/* SD::CLK_25MHZ and SD::BUS_WIDE, expressed as build-friendly enum values. */
#ifndef BOOT_SD_CLOCK
#define BOOT_SD_CLOCK 1u
#endif

#ifndef BOOT_SD_BUS
#define BOOT_SD_BUS 1u
#endif

#ifndef BOOT_LOGGING
#define BOOT_LOGGING 1
#endif

/* Number of 32-bit words read back from BOOT_LOAD_ADDRESS after the cache
 * writeback.  Zero disables the DDR dump. */
#ifndef BOOT_DUMP_WORDS
#define BOOT_DUMP_WORDS 0u
#endif

#endif
