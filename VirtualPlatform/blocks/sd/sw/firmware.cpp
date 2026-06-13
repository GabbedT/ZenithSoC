#include "platform.h"

#include "SD.h"

/* Starting address to test */
#define START_ADDRESS 0x00000000

/* Number of blocks to test */
#define NUMBER_OF_BLOCKS 4

/* Length of a burst (read / write) */
#define BURST_LENGTH 16

void testRead(SD& card, uint32_t address);
void testWrite(SD& card, uint32_t address);

void testBurstRead(SD& card, uint32_t address);
void testBurstWrite(SD& card, uint32_t address);

void testInformations(SD& card);

extern "C" int main() {
    vp_print("[TEST] Start!\n");

    uint8_t cmd8_response[6] = {0}; 
    
    bool isHighCapacity = false;
    
    
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


    for (int i = 1; i < 4; i++) {
        switch (i) {
            case 0:
                vp_print("[CONFIGURATION] | BUS: Narrow | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_NARROW, error);
                card.setClockSpeed(SD::CLK_400KHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return;
                }
            break;

            case 1:
                vp_print("[CONFIGURATION] | BUS: Narrow | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_NARROW, error);
                card.setClockSpeed(SD::CLK_25MHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return;
                }
            break;

            case 2:
                vp_print("[CONFIGURATION] | BUS: Wide | Speed: 400 KHz |\n");

                card.setBusWidth(SD::BUS_WIDE, error);
                card.setClockSpeed(SD::CLK_400KHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return;
                }
            break;

            case 3:
                vp_print("[CONFIGURATION] | BUS: Wide | Speed: 25 MHz |\n");
                
                card.setBusWidth(SD::BUS_WIDE, error);
                card.setClockSpeed(SD::CLK_25MHZ);

                if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
                    vp_print("[CONFIGURATION] Timeout!\n");

                    return;
                } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
                    vp_print("[CONFIGURATION] CRC Error!\n");

                    return;
                }
            break;
        }

        // /* Operations from start address */
        // for (int j = START_ADDRESS; j < START_ADDRESS + NUMBER_OF_BLOCKS; j++) {
        //     vp_print("[TEST] Testing block ");
        //     vp_print_hex(START_ADDRESS + j);
        //     vp_println("...");

        //     vp_print("[READ TEST] Reading original block\n");
        //     testRead(card, j);

        //     vp_print("[WRITE TEST] Overwriting block\n");
        //     testWrite(card, j);

        //     vp_print("[READ TEST] Reading new data\n");
        //     testRead(card, j);
        // }

        vp_print("\n");

        /* Operations from the last block tested */
        for (int j = START_ADDRESS + NUMBER_OF_BLOCKS; j < NUMBER_OF_BLOCKS * BURST_LENGTH; j += BURST_LENGTH) {
            vp_print("[TEST] Testing block ");
            vp_print_hex(START_ADDRESS + j);
            vp_println("...");

            vp_print("[READ BURST TEST] Reading original block\n");
            testBurstRead(card, j);

            vp_print("[WRITE BURST TEST] Overwriting block\n");
            testBurstWrite(card, j);

            vp_print("[READ BURST TEST] Reading new data\n");
            testBurstRead(card, j);
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
};


void testRead(SD& card, uint32_t address) {
    uint32_t blockRead[128] = {0}; 
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;
    
    bool timeout = false;
    bool crcError = false;

    card.readBlock(address, blockRead, response, error);

    if (error == SD::DAT_TIMEOUT) {
        vp_print("[READ] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR) {
        vp_print("[READ] CRC Error!\n");

        return;
    } else {
        vp_print("[READ] Data: ");

        for (int i = 0; i < 128; ++i) {
            vp_print_hex(blockRead[i]); vp_print(" ");
        }

        vp_print("\n");
    }
};



void testWrite(SD& card, uint32_t address) {
    /* Buffer */
    uint32_t blockWrite[128] = {0}; 
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;


    /* Generate data*/
    for (int i = 0; i < 128; ++i) {
        blockWrite[i] = address + i;
    }

    card.writeBlock(address, blockWrite, response, error);

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



void testBurstRead(SD& card, uint32_t address) {
    uint32_t blockRead[128 * BURST_LENGTH] = {0}; 
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;

    card.readBurst(address, BURST_LENGTH, blockRead, response, error);

    if (error == SD::DAT_TIMEOUT || error == SD::CMD_TIMEOUT) {
        vp_print("[BURST READ] Timeout!\n");

        return;
    } else if (error == SD::DAT_CRC_ERR || error == SD::CMD_CRC_ERR) {
        vp_print("[BURST READ] CRC Error!\n");

        return;
    } else {
        vp_print("[BURST READ] Data: ");

        for (int i = 0; i < 128 * BURST_LENGTH; ++i) {
            vp_print_hex(blockRead[i]); vp_print(" ");
        }

        vp_print("\n");
    }
};



void testBurstWrite(SD& card, uint32_t address) {
    /* Buffer */
    uint32_t blockWrite[128 * BURST_LENGTH] = {0}; 
    uint8_t response[6] = {0};
    SD::errorType_e error = SD::NO_ERROR;

    /* Generate data */
    for (int i = 0; i < 128 * BURST_LENGTH; ++i) {
        blockWrite[i] = address + i;
    }

    card.writeBurst(address, BURST_LENGTH, blockWrite, response, error);

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