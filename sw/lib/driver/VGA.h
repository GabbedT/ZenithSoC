#ifndef VGA_H
#define VGA_H

#include <stdint.h>

#include "../mmio.h"

class VGA {

public:

    /* Pixel format */
    struct pixel_s {
        uint16_t blue : 4;
        uint16_t green : 4;
        uint16_t red : 4;

        uint16_t padding : 4;
    };

    static inline uint16_t pixel2int16(const pixel_s& pixel) {
        return (pixel.red << 8) | (pixel.green << 4) | pixel.blue;
    }


    enum error_e { NO_ERROR,
                   FRAME_BUFFER_MISALIGNED,
                   FRAME_BUFFER_ADDRESS,
                   FRAME_BUFFER_SIZE,
                   INDEX_OUT_OF_RANGE };

    /* VGA Resolution */
    enum resolution_e { _320x240, _640x480_ };

    /* Register byte offsets */
    static constexpr uint32_t STATUS_REGISTER             = 0x00;
    static constexpr uint32_t FRAME_BUFFER_BASE_REGISTER  = 0x04;
    static constexpr uint32_t FRAME_BUFFER_SIZE_REGISTER  = 0x08;
    static constexpr uint32_t EVENT_REGISTER              = 0x0C;
    static constexpr uint32_t SPRITE_REGISTER             = 0x10;

    static constexpr uint32_t COLOR_TABLE_BASE   = 0x14;
    static constexpr uint32_t PATTERN_TABLE_BASE = 0x54;

    /* Sizes */
    static constexpr uint32_t COLOR_TABLE_SIZE   = 16;
    static constexpr uint32_t PATTERN_TABLE_SIZE = 64;
    static constexpr uint32_t DEVICE_SIZE        = 0x154;

    /* Status register fields */
    struct statusRegister_s {
        /* Frame has been displayed */
        unsigned int frameDone : 1;

        /* VGA video display zone */
        unsigned int videoActive : 1;

        /* VGA resolution setting */
        unsigned int resolution : 2;

        /* Enable VGA display */
        unsigned int enableVideo : 1;

        /* Enable interrupts */
        unsigned int bufferEmptyInt : 1;
        unsigned int videoActiveInt : 1;
        unsigned int frameDoneInt : 1;
        unsigned int ddrErrorInt : 1;

        /* Vertical synchronization counter */
        unsigned int vsyncCounter : 10;

        unsigned int padding : 13;
    };
    
    /* Event register fields */
    struct eventRegister_s {
        unsigned int bufferEmpty : 1;

        unsigned int videoActive : 1;

        unsigned int frameDone : 1;

        unsigned int ddrError : 1;

        unsigned int padding : 28;
    };
    
    /* Sprite register */
    struct spriteRegister_s {
        unsigned int enable : 1;

        unsigned int xPosition : 10;

        unsigned int yPosition : 10;

        unsigned int padding : 11;
    };
    


    uint32_t* const vgaBaseAddress;

    /* Status register */
    volatile struct statusRegister_s* const status;  

    /* Frame buffer base address register */
    volatile uint32_t* const frameBufferBase;

    /* Frame buffer size register, in bytes */
    volatile uint32_t* const frameBufferSize;

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

    VGA& setFrameBuffer(uint32_t base, uint32_t size, error_e* error = nullptr);

    VGA& setInterruptEnable(eventRegister_s event);

    VGA& enableDisplay(bool enable);

    VGA& enableSprite(bool enable);


/*****************************************************************/
/*                            STATUS                             */
/*****************************************************************/ 

    inline bool videoOn() {
        return status->videoActive;
    };

    inline bool frameDone() {
        return status->frameDone;
    };

    inline bool bufferEmpty() {
        return event->bufferEmpty;
    };


/*****************************************************************/
/*                            SPRITE                             */
/*****************************************************************/

    VGA& setColorTable(const pixel_s* color);

    VGA& setColor(pixel_s color, uint32_t index, error_e* error = nullptr);

    VGA& setPatternTable(const uint8_t* pattern);

    VGA& setSpritePixel(uint8_t colorIndex, uint32_t index, error_e* error = nullptr);

    VGA& setSpritePosition(uint16_t xPos, uint16_t yPos);

};

#endif
