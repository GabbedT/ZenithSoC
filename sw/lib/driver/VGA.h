#ifndef VGA_H
#define VGA_H

#include <stdint.h>

class VGA {

public:

    /* Pixel format */
    struct pixel_s {
        unsigned int blue : 4;
        unsigned int green : 4;
        unsigned int red : 4;

        unsigned int padding : 4;
    };

    enum error_e { INDEX_OUT_OF_RANGE, TABLE_SIZE_ERROR, ILLEGAL_FRAME_COUNT };

    /* VGA Resolution */
    enum resolution_e { _640x480_, _800x600_ };

    /* Status register fields */
    struct statusRegister_s {
        /* Increment buffer address each write */
        unsigned int autoIncrement : 1;

        /* Frame has been displayed */
        unsigned int frameDone : 1;

        /* VGA video display zone */
        unsigned int videoActive : 1;

        /* Line buffer is empty */
        unsigned int bufferEmpty : 1;

        /* VGA resolution setting */
        unsigned int resolution : 1;

        /* Enable VGA display */
        unsigned int enableVideo : 1;

        /* Enable interrupts */
        unsigned int bufferEmptyInt : 1;
        unsigned int videoActiveInt : 1;
        unsigned int frameDoneInt : 1;

        /* Horizontal line counter */
        unsigned int vsyncCounter : 9;

        unsigned int padding : 14;
    };
    
    /* Event register fields */
    struct eventRegister_s {
        unsigned int bufferEmpty : 1;

        unsigned int videoActive : 1;

        unsigned int frameDone : 1;

        unsigned int padding : 5;
    };
    
    /* Sprite register */
    struct spriteRegister_s {
        unsigned int enable : 1;

        unsigned int yPosition : 10;

        unsigned int xPosition : 10;

        unsigned int padding : 11;
    };
    

private:

    uint32_t* const vgaBaseAddress;

    /* Line buffer memory */
    volatile uint32_t* const lineBufferBase;

    /* Status register */
    volatile struct statusRegister_s* const status;  

    /* Increment register */
    volatile uint32_t* const increment; 

    /* Buffer size register */
    volatile uint32_t* const size;

    /* Event register */
    volatile struct eventRegister_s* const event;

    /* Sprite register */
    volatile struct spriteRegister_s* const sprite;

    /* Sprite pattern table */
    volatile uint32_t* const patternTableBase; 

    /* Sprite color table */
    volatile uint32_t* const colorTableBase; 

public: 

    VGA();

    ~VGA();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    VGA& setResolution(resolution_e resolution);

    resolution_e getResolution();


    VGA& setAutoIncrement(bool enable);

    VGA& setCount(uint32_t count, error_e* error);
    
    uint32_t getCount();


    VGA& setInterruptEnable(bool enable);

    uint8_t getInterruptEnable();


    VGA& setInterrupt(bool enable, uint8_t index, error_e* error);

    VGA& enableDisplay(bool enable);


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/ 

    bool isDisplaying();

    bool bufferEmpty();

    bool frameDone();

    uint16_t getVsyncCounter();


/*****************************************************************/
/*                            SPRITE                             */
/*****************************************************************/

    VGA& enableSprite(bool enable);

    VGA& setColorTable(pixel_s* color, error_e* error);

    VGA& setColor(pixel_s color, uint32_t index, error_e* error);

    VGA& setPatternTable(uint8_t* pattern, bool isPacked, error_e* error);

    VGA& setSpritePixel(uint8_t pixel, uint32_t index, error_e* error);

    VGA& setSpritePosition(uint16_t xPos, uint16_t yPos);

    uint16_t getSpriteX();

    uint16_t getSpriteY();


/*****************************************************************/
/*                             DRAW                              */
/*****************************************************************/

    VGA& drawPixel(pixel_s pixel, uint16_t xPos, uint16_t yPos);

    VGA& drawPixel(pixel_s pixel);
}; 

#endif 