#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "platform.h"
#include "VGA.h"

#include <stdint.h>

#define FRAME_BUFFER_BASE 0x01000000u
#define DISPLAY_BASE      0x08000000u

static inline uint16_t test_pixel(uint32_t x, uint32_t y) {
    const uint16_t red = static_cast<uint16_t>((x >> 4) & 0xF);
    const uint16_t green = static_cast<uint16_t>((y >> 4) & 0xF);
    const uint16_t blue = static_cast<uint16_t>((x ^ y) & 0xF);

    return static_cast<uint16_t>((red << 8) | (green << 4) | blue);
}

static bool displayHighResolutionTest();
static bool displayLowResolutionTest();

static bool waitForFrame(VGA& vga) {
    for (uint32_t poll = 0; poll < 2000000; ++poll) {
        vp_delay_cycles(5);

        if (vga.frameDone()) {
            return true;
        }
    }

    return false;
}

static bool displayHighResolutionTest() {
    const uint32_t width = 640;
    const uint32_t height = 480;
    volatile uint16_t* frame_buffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("===========================================");
    vp_println("    HIGH RESOLUTION DISPLAY TEST START     ");
    vp_println("===========================================");


    vp_println("[VGA] Loading frame buffer...");

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            frame_buffer[y * width + x] = test_pixel(x, y);
        }
    }

    vp_println("[VGA] Done!");


    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;
    vga.enableSprite(false)
       .setResolution(VGA::_640x480_)
       .setFrameBuffer(FRAME_BUFFER_BASE, width * height * 2, &error);
    
    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] Frame buffer setup error");
        
        return false;
    }


    vp_println("[VGA] Displaying image...");
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");

        return false;
    }


    vp_println("[VGA] Done! Checking display memory...");
    vga.enableDisplay(false);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            const uint16_t expected = test_pixel(x, y);
            const uint16_t actual = display[y * width + x];

            if (actual != expected) {
                vp_println("[VGA] High-resolution image mismatch");
                return false;
            }
        }
    }

    vp_println("[VGA] High-resolution display test passed\n\n");
    return true;
}

static bool displayLowResolutionTest() {
    const uint32_t width = 320;
    const uint32_t height = 240;
    volatile uint16_t* frame_buffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("===========================================");
    vp_println("     LOW RESOLUTION DISPLAY TEST START     ");
    vp_println("===========================================");

    
    vp_println("[VGA] Loading frame buffer...");

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            frame_buffer[y * width + x] = test_pixel(x, y);
        }
    }

    vp_println("[VGA] Done!");
    

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;
    vga.enableSprite(false)
       .setResolution(VGA::_320x240)
       .setFrameBuffer(FRAME_BUFFER_BASE, width * height * 2, &error);
    
    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] Frame buffer setup error");
        
        return false;
    }


    vp_println("[VGA] Displaying image...");
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");

        return false;
    }


    vp_println("[VGA] Done! Checking display memory...");
    vga.enableDisplay(false);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            const uint16_t expected = test_pixel(x, y);
            const uint32_t display_index = (y * 2) * 640 + (x * 2);

            if (display[display_index] != expected || display[display_index + 1] != expected ||
                display[display_index + 640] != expected || display[display_index + 641] != expected) {
                vp_println("[ERROR] Low-resolution image mismatch");
                
                return false;
            }
        }
    }

    vp_println("[VGA] Low-resolution display test passed");
    return true;
}

#endif
