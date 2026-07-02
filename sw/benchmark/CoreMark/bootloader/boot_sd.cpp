#include "../../../lib/driver/SD.h"
#include "../../../lib/driver/UART.h"

#define IMG_BLK_START  0x2000       /* Start SD block */
#define IMG_BLOCKS     64           /* Number of blocks to load */
#define DDR_ENTRY      0x80000000

extern "C" void boot_sd() {
    /* UART for logging */
    UART uart(0);
    uart.init();
    
    const char msg_start[] = "[BOOT] ZenithSoC SD Boot...\r\n";
    for (const char *c = msg_start; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

    SD::errorType_e err = SD::NO_ERROR;
    uint8_t cmd8[6] = {0};
    bool highCap = false;

    SD card;
    card.init(SD::CLK_25MHZ, SD::BUS_NARROW, cmd8, highCap, err);

    if (err != SD::NO_ERROR) {
        const char msg_init_fail[] = "[BOOT] SD Initialization failed!\r\n";
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

    for (int blk = 0; blk < IMG_BLOCKS; ++blk) {
        err = SD::NO_ERROR;

        uint32_t blkAddr = highCap ? (addr + blk) : (addr + (blk * 512));
        card.readBlock(blkAddr, block, nullptr, err);

        if (err != SD::NO_ERROR) {
            const char msg_fail_blk[] = "[BOOT] Fail reading block\r\n";
            for (const char *c = msg_fail_blk; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

            while (1) {  }
        }

        for (int i = 0; i < 128; ++i) {
            ddr[(blk * 128) + i] = block[i];
        }
    }

    const char msg_boot_end[] = "[BOOT] Image loaded! Jumping to CoreMark benchmark...\r\n";
    for (const char *c = msg_boot_end; *c != '\0'; ++c) { uart.sendByte((uint8_t) *c); }

    void (*entry)() = (void(*)()) DDR_ENTRY;
    entry();

    while (1) {  }
};