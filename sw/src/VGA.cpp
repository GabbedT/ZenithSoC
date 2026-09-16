#ifndef VGA_CPP
#define VGA_CPP

#include "../lib/driver/VGA.h"
#include "../lib/mmio.h"

#include <stdint.h>


VGA::VGA() :

    vgaBaseAddress   ( (uint32_t*) VGA_BASE                                                         ),
    status           ( (volatile struct statusRegister_s*) (VGA_BASE + STATUS_REGISTER)             ),
    frameBufferBase  ( (volatile uint32_t*)                (VGA_BASE + FRAME_BUFFER_BASE_REGISTER)  ),
    frameBufferSize  ( (volatile uint32_t*)                (VGA_BASE + FRAME_BUFFER_SIZE_REGISTER)  ),
    event            ( (volatile struct eventRegister_s*)  (VGA_BASE + EVENT_REGISTER)              ),
    sprite           ( (volatile struct spriteRegister_s*) (VGA_BASE + SPRITE_REGISTER)             ),
    patternTableBase ( (volatile uint32_t*)                (VGA_BASE + PATTERN_TABLE_BASE)          ),
    colorTableBase   ( (volatile uint32_t*)                (VGA_BASE + COLOR_TABLE_BASE)            )
{
    eventRegister_s e;

    e.bufferEmpty = false;
    e.ddrError = false;
    e.frameDone = false;
    e.videoActive = false;

    setInterruptEnable(e);
    enableDisplay(false);
}


VGA::~VGA() {
    eventRegister_s e;
    
    e.bufferEmpty = false;
    e.ddrError = false;
    e.frameDone = false;
    e.videoActive = false;
    
    /* Disable interrupts and VGA output */
    setInterruptEnable(e);
    enableDisplay(false);
};


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

VGA& VGA::setResolution(resolution_e resolution) {
    status->resolution = resolution;

    return *this;
};


VGA& VGA::setFrameBuffer(uint32_t base, uint32_t size, error_e* error) {
    /* Frame buffer needs to be aligned to 16 bytes */
    if (base & 0xF) {
        if (error) {
            *error = FRAME_BUFFER_MISALIGNED;
        }

        return *this;
    }

    /* Check if size is empty or is not a multiplo of 16 bytes*/
    if ((size == 0) || (size & 0xF)) {
        if (error) {
            *error = FRAME_BUFFER_SIZE;
        }

        return *this;
    }

    *frameBufferBase = base;
    *frameBufferSize = size;

    return *this;
};


VGA& VGA::setInterruptEnable(VGA::eventRegister_s event) {
    status->ddrErrorInt = event.ddrError;
    status->frameDoneInt = event.frameDone;
    status->bufferEmptyInt = event.bufferEmpty;
    status->videoActiveInt = event.videoActive;

    return *this;
};



VGA& VGA::enableDisplay(bool enable) {
    status->enableVideo = enable;

    return *this;
};


/*****************************************************************/
/*                            SPRITE                             */
/*****************************************************************/

VGA& VGA::enableSprite(bool enable) {
    sprite->enable = enable;

    return *this;
};


VGA& VGA::setColorTable(const pixel_s* color) {
    for (uint32_t index = 0; index < COLOR_TABLE_SIZE; ++index) {
        colorTableBase[index] = pixel2int16(color[index]);
    }

    return *this;
};


VGA& VGA::setColor(pixel_s color, uint32_t index, error_e* error) {
    if (index >= COLOR_TABLE_SIZE) {
        if (error) {
            *error = INDEX_OUT_OF_RANGE;
        }

        return *this;
    }

    colorTableBase[index] = pixel2int16(color);

    return *this;
};


VGA& VGA::setPatternTable(const uint8_t* pattern) {
    for (uint32_t index = 0; index < PATTERN_TABLE_SIZE; ++index) {
        patternTableBase[index] = pattern[index] & 0xF;
    }

    return *this;
};


VGA& VGA::setSpritePixel(uint8_t colorIndex, uint32_t index, error_e* error) {
    if ((index >= PATTERN_TABLE_SIZE) || (colorIndex > 0xF)) {
        if (error) {
            *error = INDEX_OUT_OF_RANGE;
        }

        return *this;
    }

    patternTableBase[index] = colorIndex;

    return *this;
};


VGA& VGA::setSpritePosition(uint16_t xPos, uint16_t yPos) {
    sprite->xPosition = xPos;
    sprite->yPosition = yPos;

    return *this;
};


#endif
