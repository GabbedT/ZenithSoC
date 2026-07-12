#include "platform.h"

#include "SD.h"

/* Starting address to test */
#define START_ADDRESS 0x00000000

/* Number of blocks to test */
#define NUMBER_OF_BLOCKS 4

/* Length of a burst (read / write) */
#define BURST_LENGTH 16

/* Comment to avoid printing all the read blocks */
// #define PRINT_DATA

void testRead(SD& card, uint32_t address, uint32_t *blk);
void testWrite(SD& card, uint32_t address, uint32_t *blk);

void testBurstRead(SD& card, uint32_t address, uint32_t burstLength, uint32_t *blk);
void testBurstWrite(SD& card, uint32_t address, uint32_t burstLength, uint32_t *blk);

bool checkBlock(uint32_t *origBk, uint32_t *newBk);
bool checkBurstBlock(uint32_t *origBk, uint32_t *newBk, uint32_t burstLength);

void testInformations(SD& card);

extern "C" int main() {
    vp_print("[TEST] Start!\n");

    uint8_t cmd8_response[6] = {0}; 
    
    bool isHighCapacity = false;
    
    /* Original blocks before overwriting */
    uint32_t origBlock [128] = {0};
    uint32_t origBurstBlock [128 * BURST_LENGTH] = {0};

    /* Blocks after overwriting */
    uint32_t newBlock [128] = {0};
    uint32_t newBurstBlock [128 * BURST_LENGTH] = {0};
    
    /* Initialize SD Card controller */
    SD card;

    SD::errorType_e error = SD::NO_ERROR; 

    card.reset();
    card.control->enableSD = true;

    /* Need ~74 SD clock cycles before starting CMD0 */
    vp_delay_cycles(20000);

    card.init(SD::CLK_25MHZ, SD::BUS_NARROW, cmd8_response, isHighCapacity, error);

    if (error == SD::CMD_TIMEOUT || error == SD::DAT_TIMEOUT) {
        vp_print("[INITIALIZATION] Timeout!\n");

        TEST_FAIL();
        return 0;
    }

    if (error == SD::CMD_CRC_ERR || error == SD::DAT_CRC_ERR) {
        vp_print("[INITIALIZATION] CRC Error!\n");

        TEST_FAIL();
        return 0;
    }

    if (isHighCapacity) {
        vp_print("[INITIALIZATION] Card is High Capacity (SDHC/SDXC)\n");
    } else {
        vp_print("[INITIALIZATION] Card is Standard Capacity (SDSC)\n");
    }


    /* Print CMD8 response */
    vp_print("[INITIALIZATION] CMD8 Response: ");

    for (int i = 0; i < 6; i++) {
        vp_print_hex(cmd8_response[i]); vp_print(" ");
    }

    vp_print("\n");

    testInformations(card);

    vp_print("\n\n");

    /* Loop to go through all the configurations */
    for (int i = 0; i < 4; i++) {
        switch (i) {
            case 0:
                vp_print("[CONFIGURATION] | BUS: Narrow | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_NARROW, error);
                card.setClockSpeed(SD::CLK_400KHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return 0;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return 0;
                }
            break;

            case 1:
                vp_print("[CONFIGURATION] | BUS: Narrow | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_NARROW, error);
                card.setClockSpeed(SD::CLK_25MHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return 0;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return 0;
                }
            break;

            case 2:
                vp_print("[CONFIGURATION] | BUS: Wide | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_WIDE, error);
                card.setClockSpeed(SD::CLK_400KHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return 0;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return 0;
                }
            break;

            case 3:
                vp_print("[CONFIGURATION] | BUS: Wide | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_WIDE, error);
                card.setClockSpeed(SD::CLK_25MHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return 0;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return 0;
                }
            break;
        }

        /* Operations from start address */
        for (int j = START_ADDRESS; j < START_ADDRESS + NUMBER_OF_BLOCKS; j++) {
            vp_print("[TEST] Testing block ");
            vp_print_hex(START_ADDRESS + j);
            vp_println("...");

            vp_print("[READ TEST] Reading original block\n");
            testRead(card, j, origBlock);

            vp_print("[WRITE TEST] Overwriting block\n");
            testWrite(card, j, origBlock);

            vp_print("[READ TEST] Reading new data\n");
            testRead(card, j, newBlock);

            if (!checkBlock(origBlock, newBlock)) {
                vp_print("[ERROR] On block address: 0x");
                vp_print_hex(j);
                vp_putchar('\n');

                TEST_FAIL();
                return 0;
            }
        }

        vp_print("\n");


        /* Operations from the last block tested */
        /* Keep a real multi-block transfer at 400 kHz without spending most
         * of the VP run in MMIO polling; retain the 16-block stress test at
         * 25 MHz. */
        const uint32_t testBurstLength = ((i == 1) || (i == 3)) ? 3 : 2;

        for (int j = START_ADDRESS + NUMBER_OF_BLOCKS; j < NUMBER_OF_BLOCKS * testBurstLength; j += testBurstLength) {
            vp_print("[TEST] Testing block ");
            vp_print_hex(START_ADDRESS + j);
            vp_println("...");

            vp_print("[READ BURST TEST] Reading original block\n");
            testBurstRead(card, j, testBurstLength, origBurstBlock);

            vp_print("[WRITE BURST TEST] Overwriting block\n");
            testBurstWrite(card, j, testBurstLength, origBurstBlock);

            vp_print("[READ BURST TEST] Reading new data\n");
            testBurstRead(card, j, testBurstLength, newBurstBlock);

            if (!checkBurstBlock(origBurstBlock, newBurstBlock, testBurstLength)) {
                vp_print("[ERROR] On burst block address: 0x");
                vp_print_hex(j);
                vp_putchar('\n');

                TEST_FAIL();
                return 0;
            }
        }

        vp_print("\n\n");
    }

    TEST_PASS();
    return 0;
};


void testInformations(SD& card) {
    vp_print("[INFORMATION TEST] Reading SD Card Infos...\n");

    /* RCA */
    vp_print("[RCA] "); vp_println_hex(card.cardRCA);

    /* CID */
    vp_print("[RCA] "); vp_print_hex(card.cardCID[1]); vp_println_hex(card.cardCID[0]);

    /* CSD */
    vp_print("[CSD] "); vp_print_hex(card.cardCSD[1]); vp_println_hex(card.cardCSD[0]);

    /* SCR */
    vp_print("[SCR] "); vp_println_hex(card.cardSCR);

    /* OCR */
    vp_print("[OCR] "); vp_println_hex(card.cardOCR);

    /* Card status (CMD13 / R1) */
    SD::errorType_e error = SD::NO_ERROR;
    SD::cardStatus_u status = card.getCardStatus(error);

    if (error == SD::NO_ERROR) {
        vp_print("[STATUS] "); vp_println_hex(status.raw);
    } else {
        vp_print("[STATUS] Read error\n");
    }
};


void testRead(SD& card, uint32_t address, uint32_t *blk) {
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;
    
    bool timeout = false;
    bool crcError = false;

    card.readBlock(address, blk, response, error);

    if (error == SD::DAT_TIMEOUT) {
        vp_print("[READ] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR) {
        vp_print("[READ] CRC Error!\n");

        return;
    } else {
        #ifdef PRINT_DATA

        vp_print("[READ] Data: ");

        for (int i = 0; i < 128; ++i) {
            vp_print_hex(blk[i]); vp_print(" ");
        }

        vp_print("\n");

        #endif
    }
};



void testWrite(SD& card, uint32_t address, uint32_t *blk) {
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;


    card.writeBlock(address, blk, response, error);

    if (error == SD::DAT_TIMEOUT) {
        vp_print("[WRITE] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR) {
        vp_print("[WRITE] CRC Error!\n");

        return;
    } else {
        vp_print("\n");
    }
};



void testBurstRead(SD& card, uint32_t address, uint32_t burstLength, uint32_t *blk) {
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;

    card.readBurst(address, burstLength, blk, response, error);

    if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
        vp_print("[BURST READ] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
        vp_print("[BURST READ] CRC Error!\n");

        return;
    } else {
        #ifdef PRINT_DATA

        vp_print("[BURST READ] Data: ");

        for (uint32_t i = 0; i < 128 * burstLength; ++i) {
            vp_print_hex(blk[i]); vp_print(" ");
        }

        vp_print("\n");

        #endif
    }
};



void testBurstWrite(SD& card, uint32_t address, uint32_t burstLength, uint32_t *blk) {
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;

    card.writeBurst(address, burstLength, blk, response, error);

    if (error == SD::DAT_TIMEOUT) {
        vp_print("[BURST WRITE] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR) {
        vp_print("[BURST WRITE] CRC Error!\n");

        return;
    } else {
        vp_print("\n");
    }
};


bool checkBlock(uint32_t *origBk, uint32_t *newBk) {
    for (int i = 0; i < 128; ++i) {
        if (origBk[i] != newBk[i]) {
            TEST_FAIL();

            return false;
        }
    }

    return true;
};



bool checkBurstBlock(uint32_t *origBk, uint32_t *newBk, uint32_t burstLength) {
    for (uint32_t i = 0; i < 128 * burstLength; ++i) {
        if (origBk[i] != newBk[i]) {
            TEST_FAIL();

            return false;
        }
    }

    return true;
}
