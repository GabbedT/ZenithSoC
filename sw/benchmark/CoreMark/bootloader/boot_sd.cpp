#include "../../Library/Driver/SD.h"
#include "../../Library/Driver/UART.h"
#include "../../Library/Serial_IO.h"

#define IMG_BLK_START  0x2000       /* Start SD block */
#define IMG_BLOCKS     64           /* Number of blocks to load */
#define DDR_ENTRY      0x80000000

extern "C" void boot_sd() {
    /* UART for logging */
    UART uart(0);

    Serial_IO::println("[BOOT] ZenithSoC SD Boot...");

    SD::errorType_e err = SD::NO_ERROR;
    uint8_t cmd8[6] = {0};
    bool highCap = false;

    SD card;
    card.init(SD::CLK_25MHZ, SD::BUS_NARROW, cmd8, err, highCap);

    if (err != SD::NO_ERROR) {
        Serial_IO::println("[BOOT] SD Initialization failed!");
        while (1) {  }
    }


    if (highCap) {
        Serial_IO::println("[BOOT] SD OK, High Capacity!");
    } else {
        Serial_IO::println("[BOOT] SD OK, No High Capacity!");
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
            Serial_IO::printf("[BOOT] Fail reading block %d\n", blk);
            while (1) {  }
        }

        for (int i = 0; i < 128; ++i) {
            ddr[(blk * 128) + i] = block[i];
        }
    }

    Serial_IO::println("[BOOT] Image loaded! Jumping to CoreMark benchmark...\n\n");

    void (*entry)() = (void(*)()) DDR_ENTRY;
    entry();

    while (1) {  }
}