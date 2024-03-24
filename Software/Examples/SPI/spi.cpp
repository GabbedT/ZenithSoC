#include "../../Library/Driver/GPIO.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Driver/SPI.h"
#include "../../Library/SerialOut.h"

#include <inttypes.h>

/* ADXL362 Commands */
#define READ_REGISTER 0x0B
#define WRITE_REGISTER 0x0A
#define READ_FIFO 0x0D

#define END_BYTE 0x00

#define RESET_BYTE 'R'

// #define _TEST_

extern "C" void spi() {
    SerialOut::init();
    Timer timer(0);

    timer.init(-1, Timer::ONE_SHOT)
         .setInterrupt(false);

    SPI spi(0); SPI::spiError_e error;

    spi.init(1'000'000, SPI::MODE0, SPI::MSB_FIRST, &error)
       .connect(0, &error);

    if (error == SPI::INDEX_OUT_OF_RANGE) {
        SerialOut::write("[SPI] ERROR ON SLAVE CONNECTION\n");
    } else if (error == SPI::ILLEGAL_CLOCK) {
        SerialOut::write("[SPI] ERROR ON CLOCK SELECTION\n");
    }


    uint8_t devCommand[3] = {WRITE_REGISTER, 0x1F, RESET_BYTE};
    uint8_t devDataRX[3];

    spi.exchangeStream(devCommand, devDataRX, sizeof(devCommand));

    #ifndef _TEST_
        timer.delay(100);
    #endif 

    devCommand[1] = 0x2D;
    devCommand[2] = 0x22;

    spi.exchangeStream(devCommand, devDataRX, sizeof(devCommand));

    devCommand[0] = READ_REGISTER;
    devCommand[2] = END_BYTE;

    /* Read all registers */
    for (uint8_t i = 0; i <= 0x2E; ++i) {
        if (i == 0x04) {
            i = 0x08;
        } else if (i == 0x17) {
            i = 0x1F;
        }

        devCommand[1] = i;
        spi.exchangeStream(devCommand, devDataRX, sizeof(devCommand));

        SerialOut::printf("[0x%xb]: 0x%xh\n", i, devDataRX[2]);
    }

    int16_t xAxis, yAxis, zAxis, temperature;
    uint8_t burstCommand[] = {READ_REGISTER, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, END_BYTE};
    uint8_t burstRX[10];

    while (1) {
        spi.exchangeStream(burstCommand, burstRX, sizeof(burstCommand));

        xAxis = yAxis = zAxis = temperature = 0;

        xAxis = burstRX[2] | (burstRX[3] << 8);
        yAxis = burstRX[4] | (burstRX[5] << 8);
        zAxis = burstRX[6] | (burstRX[7] << 8);
        temperature = burstRX[8] | (burstRX[9] << 8);

        SerialOut::println("[X-Axis]: %d", xAxis);
        SerialOut::println("[Y-Axis]: %d", yAxis);
        SerialOut::println("[Z-Axis]: %d", zAxis);
        SerialOut::println("[Temperature]: %d", temperature);

        #ifndef _TEST_
            timer.delay(100);
        #endif 

        for (int i = 0; i < 4; i++) {
            /* Cancel current line */
            SerialOut::write("\x1b[2K"); 
            SerialOut::write("\x1b[1A");
        }
    }
}