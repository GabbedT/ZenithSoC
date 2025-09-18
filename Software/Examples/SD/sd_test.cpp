#include "../../Library/Driver/SD.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Serial_IO.h"

/* Starting address to test */
#define START_ADDRESS 0x00000000

/* Number of blocks to test */
#define NUMBER_OF_BLOCKS 4

/* Length of a burst (read / write) */
#define BURST_LENGTH 16

void testRead(SD& card, uint32_t address, Timer& timer);
void testWrite(SD& card, uint32_t address, Timer& timer);

void testBurstRead(SD& card, uint32_t address, Timer& timer);
void testBurstWrite(SD& card, uint32_t address, Timer& timer);

void testInformations(SD& card);

extern "C" void sd_test() {
    /* Initialize serial output */
    Serial_IO::init();

    /* Initialize timer for performance test */
    Timer timer(0); timer.init(-1, Timer::ONE_SHOT);

    Serial_IO::write("============= Start SD Test ============= \n");

    uint8_t cmd8_response[6] = {0}; bool timeout = false; bool isHighCapacity = false;

    /* Initialize SD Card controller */
    SD card; card.init(SD::CLK_25MHZ, SD::BUS_NARROW, cmd8_response, timeout, isHighCapacity);

    if (timeout) {
        Serial_IO::write("[INITIALIZATION] Timeout!\n");

        return;
    }

    if (isHighCapacity) {
        Serial_IO::write("[INITIALIZATION] Card is High Capacity (SDHC/SDXC)\n");
    } else {
        Serial_IO::write("[INITIALIZATION] Card is Standard Capacity (SDSC)\n");
    }


    /* Print CMD8 response */
    Serial_IO::write("[INITIALIZATION] CMD8 Response: ");

    for (int i = 0; i < 6; i++) {
        Serial_IO::write(cmd8_response[i], Serial_IO::HEX); Serial_IO::write(" ");
    }

    Serial_IO::write("\n");

    testInformations(card);

    Serial_IO::write("\n\n");


    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                Serial_IO::write("[CONFIGURATION] | BUS: Narrow | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_NARROW);
                card.setClockSpeed(SD::CLK_400KHZ);
            break;

            case 1:
                Serial_IO::write("[CONFIGURATION] | BUS: Narrow | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_NARROW);
                card.setClockSpeed(SD::CLK_25MHZ);
            break;

            case 2:
                Serial_IO::write("[CONFIGURATION] | BUS: Wide | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_WIDE);
                card.setClockSpeed(SD::CLK_400KHZ);
            break;

            case 3:
                Serial_IO::write("[CONFIGURATION] | BUS: Wide | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_WIDE);
                card.setClockSpeed(SD::CLK_25MHZ);
            break;
        }

        /* Operations from start address */
        for (int j = START_ADDRESS; j < START_ADDRESS + NUMBER_OF_BLOCKS; j++) {
            Serial_IO::printf("[TEST] Testing block %d...\n", START_ADDRESS + j);

            Serial_IO::write("[READ TEST] Reading original block\n");
            testRead(card, j, timer);

            Serial_IO::write("[WRITE TEST] Overwriting block\n");
            testWrite(card, j, timer);

            Serial_IO::write("[READ TEST] Reading new data\n");
            testRead(card, j, timer);
        }

        Serial_IO::write("\n");

        /* Operations from the last block tested */
        for (int j = START_ADDRESS + NUMBER_OF_BLOCKS; j < NUMBER_OF_BLOCKS * BURST_LENGTH; j += BURST_LENGTH) {
            Serial_IO::printf("[TEST] Testing block %d...\n", START_ADDRESS + j);

            Serial_IO::write("[READ BURST TEST] Reading original block\n");
            testBurstRead(card, j, timer);

            Serial_IO::write("[WRITE BURST TEST] Overwriting block\n");
            testBurstWrite(card, j, timer);

            Serial_IO::write("[READ BURST TEST] Reading new data\n");
            testBurstRead(card, j, timer);
        }

        Serial_IO::write("\n\n");
    }
};


void testInformations(SD& card) {
    Serial_IO::write("[INFORMATION TEST] Reading SD Card Infos...\n");

    /* RCA */
    Serial_IO::printf("[RCA] 0x%xh\n", card.cardRCA);

    /* CID */
    Serial_IO::printf("[CID] 0x%xd%xd\n", card.cardCID[1], card.cardCID[0]);

    /* CSD */
    Serial_IO::printf("[CSD] 0x%xd%xd\n", card.cardCSD[1], card.cardCSD[0]);

    /* SCR */
    Serial_IO::printf("[SCR] 0x%xd\n", card.cardSCR);

    /* OCR */
    Serial_IO::printf("[OCR] 0x%x\n\n", card.cardOCR);
}


void testRead(SD& card, uint32_t address, Timer& timer) {
    uint32_t blockRead[128] = {0}; bool timeout = false;

    /* Starting timer to measure performance for block read */
    timer.restart();
    card.readBlock(address, blockRead, nullptr, timeout);
    timer.stop();

    if (timeout) {
        Serial_IO::write("[READ] Timeout!\n");

        return;
    } else {
        /* Print actual time (timer cycles times the clock period)*/
        Serial_IO::printf("[READ] Time: %l ns\n", timer.getTime() * CLOCK_PERIOD);

        Serial_IO::write("[READ] Data: ");

        for (int i = 0; i < 128; ++i) {
            Serial_IO::write(blockRead[i], Serial_IO::HEX);
        }

        Serial_IO::write("\n");
    }
};



void testWrite(SD& card, uint32_t address, Timer& timer) {
    bool timeout = false; uint8_t responseToken = 0;

    /* Buffer */
    uint32_t blockWrite[128] = {0}; 

    /* Generate data*/
    for (int i = 0; i < 128; ++i) {
        blockWrite[i] = address + i;
    }

    /* Starting timer to measure performance for block read */
    timer.restart();
    card.writeBlock(address, blockWrite, nullptr, responseToken, timeout);
    timer.stop();

    if (timeout) {
        Serial_IO::write("[WRITE] Timeout!\n");

        return;
    } else {
        /* Print actual time (timer cycles times the clock period)*/
        Serial_IO::printf("[WRITE] Time: %l ns\n", timer.getTime() * CLOCK_PERIOD);

        Serial_IO::write("\n");
    }
};



void testBurstRead(SD& card, uint32_t address, Timer& timer) {
    uint32_t blockRead[128 * BURST_LENGTH] = {0}; bool timeout = false;

    /* Starting timer to measure performance for block read */
    timer.restart();
    card.readBurst(address, BURST_LENGTH, blockRead, nullptr, timeout);
    timer.stop();

    if (timeout) {
        Serial_IO::write("[BURST READ] Timeout!\n");

        return;
    } else {
        /* Print actual time (timer cycles times the clock period)*/
        Serial_IO::printf("[BURST READ] Time: %l ns\n", timer.getTime() * CLOCK_PERIOD);

        Serial_IO::write("[BURST READ] Data: ");

        for (int i = 0; i < 128 * BURST_LENGTH; ++i) {
            Serial_IO::write(blockRead[i], Serial_IO::HEX);
        }

        Serial_IO::write("\n");
    }
};



void testBurstWrite(SD& card, uint32_t address, Timer& timer) {
    bool timeout = false; uint8_t responseToken = 0;

    /* Buffer */
    uint32_t blockWrite[128 * BURST_LENGTH] = {0}; 

    /* Generate data */
    for (int i = 0; i < 128 * BURST_LENGTH; ++i) {
        blockWrite[i] = address + i;
    }

    /* Starting timer to measure performance for block read */
    timer.restart();
    card.writeBurst(address, BURST_LENGTH, blockWrite, nullptr, &responseToken, timeout);
    timer.stop();

    if (timeout) {
        Serial_IO::write("[BURST WRITE] Timeout!\n");

        return;
    } else {
        /* Print actual time (timer cycles times the clock period)*/
        Serial_IO::printf("[BURST WRITE] Time: %l ns\n", timer.getTime() * CLOCK_PERIOD);

        Serial_IO::write("\n");
    }
};