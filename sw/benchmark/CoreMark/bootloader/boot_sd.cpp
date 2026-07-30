#include "../../../lib/driver/SD.h"
#include "../../../lib/driver/UART.h"

#define IMG_BLK_START  0x2000       /* Start SD block */
#ifndef IMG_BLOCKS
#define IMG_BLOCKS     64           /* Default: 32 KiB application image */
#endif
#define DDR_ENTRY      0x80000000
#define DDR_PACE_CYCLES 2048

static void sendHexNibble(UART& uart, uint8_t value) {
    value &= 0x0F;
    uart.sendByte((uint8_t) (value < 10 ? ('0' + value) : ('A' + value - 10)));
}

static void sendHexWord(UART& uart, uint32_t value) {
    for (int shift = 28; shift >= 0; shift -= 4) {
        sendHexNibble(uart, (uint8_t) (value >> shift));
    }
}

extern "C" void boot_sd() {
    /* UART for logging */
    UART uart(0);
    uart.init(115200, false);
    
    const char msg_start[] = "[BOOT] ZenithSoC SD Boot...\r\n";
    for (const char *c = msg_start; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

    SD::errorType_e err = SD::NO_ERROR;
    uint8_t cmd8[6] = {0};
    bool highCap = false;

    SD card;
    /* Keep the conservative clock for the complete boot transfer.  The RTL
     * simulation tolerates 25 MHz, but the physical Nexys A7 SD path does not
     * have input timing constraints/tuning and can stall on CMD17 data. */
    card.init(SD::CLK_400KHZ, SD::BUS_NARROW, cmd8, highCap, err);

    if (err == SD::NO_CARD) {
        const char msg_init_fail[] = "[BOOT] No card detected!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }

    if (err == SD::CMD_CRC_ERR) {
        const char msg_init_fail[] = "[BOOT] Command CRC error!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }

    if (err == SD::CMD_TIMEOUT) {
        const char msg_init_fail[] = "[BOOT] Command timeout!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }

    if (err == SD::DAT_TIMEOUT) {
        const char msg_init_fail[] = "[BOOT] Data timeout!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }

    if (err == SD::CARD_ERR) {
        const char msg_init_fail[] = "[BOOT] Card error during initialization!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }

    if (err != SD::NO_ERROR) {
        const char msg_init_fail[] = "[BOOT] Initialization error D:!\r\n";
        for (const char *c = msg_init_fail; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

        while (1) {  }
    }


    if (highCap) {
        const char msg_cap[] = "[BOOT] SD OK, High Capacity!\r\n";
        for (const char *c = msg_cap; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }
    } else {
        const char msg_cap[] = "[BOOT] SD OK, No High Capacity!\r\n";
        for (const char *c = msg_cap; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }
    }

    /* DDR */
    volatile uint32_t* ddr = (volatile uint32_t*) DDR_ENTRY;

    /* Build address, SDHC = block address, SDSC = byte address */
    uint32_t addr = highCap ? IMG_BLK_START : (IMG_BLK_START * 512);

    /* Load blocks */
    uint32_t block[128];

    const char msg_load[] = "[BOOT] Loading: ";
    for (const char *c = msg_load; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

    for (int blk = 0; blk < IMG_BLOCKS; ++blk) {
        err = SD::NO_ERROR;

        uint32_t blkAddr = highCap ? (addr + blk) : (addr + (blk * 512));
        card.readBlock(blkAddr, block, nullptr, err);

        if (err != SD::NO_ERROR) {
            const char msg_fail_blk[] = "[BOOT] Fail reading block\r\n";
            for (const char *c = msg_fail_blk; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

            while (1) {  }
        }

        if (blk == 0) { uart.sendByte('R'); }
        if (blk >= 64) { uart.sendByte('r'); }

        for (int i = 0; i < 128; ++i) {
            ddr[(blk * 128) + i] = block[i];

            /* The deployed base bitstream predates DDR FIFO backpressure.
             * Flush after exactly one 16-byte cache line, so each fence can
             * enqueue at most one dirty-line writeback.  Waiting until the
             * end would flush the complete 8 KiB D-cache and overflow the
             * bridge when the physical MIG applies backpressure. */
            if ((i & 3) == 3) {
                asm volatile ("fence rw, rw" ::: "memory");

                for (volatile int pace = 0; pace < DDR_PACE_CYCLES; ++pace) {
                    asm volatile ("nop");
                }
            }
        }

        if (blk == 0) { uart.sendByte('W'); }
        if (blk >= 64) { uart.sendByte('w'); }
        if ((blk & 7) == 7) { uart.sendByte('.'); }
    }

    uart.sendByte('\r');
    uart.sendByte('\n');

    const uint32_t firstInstruction = ddr[0];
    const char msg_ddr[] = "[BOOT] DDR[0]: 0x";
    for (const char *c = msg_ddr; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }
    sendHexWord(uart, firstInstruction);
    uart.sendByte('\r');
    uart.sendByte('\n');

    if (firstInstruction != 0x88000137u) {
        const char msg_bad_image[] = "[BOOT] Invalid image in DDR!\r\n";
        for (const char *c = msg_bad_image; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }
        while (1) {  }
    }

    const char msg_boot_end[] = "[BOOT] Image loaded! Jumping to CoreMark benchmark...\r\n";
    for (const char *c = msg_boot_end; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

    /* Write back dirty cache lines, invalidate both caches, then transfer
     * control without executing any further C code. */
    asm volatile ("fence rw, rw\n\tjr %0" :: "r" (DDR_ENTRY) : "memory");
    __builtin_unreachable();
};
