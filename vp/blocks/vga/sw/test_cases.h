#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "platform.h"
#include "VGA.h"

#include <stdint.h>

#define FRAME_BUFFER_BASE 0x01000000
#define DISPLAY_BASE      0x08000000

static inline uint16_t test_pixel(uint32_t x, uint32_t y) {
    const uint16_t red = static_cast<uint16_t>((x >> 4) & 0xF);
    const uint16_t green = static_cast<uint16_t>((y >> 4) & 0xF);
    const uint16_t blue = static_cast<uint16_t>((x ^ y) & 0xF);

    return static_cast<uint16_t>((red << 8) | (green << 4) | blue);
}

static bool displayHighResolutionTest();
static bool displayHighResolutionSpriteTest();
static bool displayLowResolutionTest();
static bool displayLowResolutionSpriteTest();

static const uint8_t sprite_test_pattern[64] = {
    0xF, 0x1, 0x2, 0x3, 0x4, 0xF, 0x6, 0x7,
    0x8, 0xF, 0xA, 0xB, 0xC, 0xD, 0xF, 0x0,
    0x1, 0x2, 0xF, 0x4, 0x5, 0x6, 0x7, 0x8,
    0x9, 0xA, 0xB, 0xF, 0xD, 0xE, 0x0, 0x1,
    0x2, 0xF, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9,
    0xA, 0xB, 0xC, 0xD, 0xF, 0x0, 0x1, 0x2,
    0x3, 0x4, 0x5, 0x6, 0x7, 0xF, 0x9, 0xA,
    0xB, 0xC, 0xD, 0xE, 0x0, 0x1, 0x2, 0xF
};

static VGA::pixel_s sprite_test_color(uint8_t index) {
    VGA::pixel_s color = {};

    color.red = static_cast<uint16_t>((index * 3 + 1) & 0xF);
    color.green = static_cast<uint16_t>((index * 5 + 2) & 0xF);
    color.blue = static_cast<uint16_t>((index * 7 + 4) & 0xF);

    return color;
}

static uint16_t sprite_test_color_value(uint8_t index) {
    return VGA::pixel2int16(sprite_test_color(index));
}

static uint16_t sprite_test_expected_pixel(uint32_t x, uint32_t y, uint32_t sprite_x, uint32_t sprite_y, uint16_t background) {
    if ((x < sprite_x) || (x >= sprite_x + 8) || (y < sprite_y) || (y >= sprite_y + 8)) {
        return background;
    }

    const uint8_t color_index = sprite_test_pattern[(y - sprite_y) * 8 + (x - sprite_x)];

    return color_index == 0xF ? background : sprite_test_color_value(color_index);
}

static bool waitForFrame(VGA& vga) {
    for (uint32_t poll = 0; poll < 2000000; ++poll) {
        vp_delay_cycles(5);

        if (vga.earlyFrameDone()) {
            vga.clearEarlyFrameDone();
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

    vp_println("");
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
    vga.clearEarlyFrameDone();
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
                vp_print("[ERROR] High-resolution image mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);

                return false;
            }
        }
    }

    vp_println("[VGA] High-resolution display test passed\n\n");
    return true;
}


static bool displayHighResolutionSpriteTest() {
    const uint32_t width = 640;
    const uint32_t height = 480;
    const uint32_t first_sprite_x = 73;
    const uint32_t first_sprite_y = 61;
    const uint32_t second_sprite_x = 411;
    const uint32_t second_sprite_y = 307;
    volatile uint16_t* frame_buffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("");
    vp_println("==========================================");
    vp_println(" HIGH RESOLUTION SPRITE TEST START ");
    vp_println("==========================================");

    vp_println("[VGA] Loading frame buffer...");

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            frame_buffer[y * width + x] = test_pixel(x, y);
        }
    }

    vp_println("[VGA] Done!");

    VGA::pixel_s sprite_colors[VGA::COLOR_TABLE_SIZE];
    for (uint32_t index = 0; index < VGA::COLOR_TABLE_SIZE; ++index) {
        sprite_colors[index] = sprite_test_color(index);
    }

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;
    vga.setResolution(VGA::_640x480_)
       .setColorTable(sprite_colors)
       .setPatternTable(sprite_test_pattern)
       .setSpritePosition(first_sprite_x, first_sprite_y)
       .enableSprite(true)
       .setFrameBuffer(FRAME_BUFFER_BASE, width * height * 2, &error);

    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] Frame buffer setup error");
        return false;
    }

    vp_println("[VGA] Displaying first sprite position...");
    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");
        return false;
    }

    vga.enableDisplay(false);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            const uint16_t expected = sprite_test_expected_pixel(x, y, first_sprite_x, first_sprite_y, test_pixel(x, y));
            const uint16_t actual = display[y * width + x];

            if (actual != expected) {
                vp_print("[ERROR] High-resolution sprite mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);
                return false;
            }
        }
    }

    vga.setSpritePosition(second_sprite_x, second_sprite_y);

    vp_println("[VGA] Displaying second sprite position...");
    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");
        return false;
    }

    vga.enableDisplay(false);

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            const uint16_t expected = sprite_test_expected_pixel(
                x, y, second_sprite_x, second_sprite_y, test_pixel(x, y));
            const uint16_t actual = display[y * width + x];

            if (actual != expected) {
                vp_print("[ERROR] High-resolution sprite mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);
                return false;
            }
        }
    }

    vp_println("[VGA] High-resolution sprite test passed");
    return true;
}


static bool displayLowResolutionTest() {
    const uint32_t width = 320;
    const uint32_t height = 240;
    volatile uint16_t* frame_buffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("");
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
    vga.clearEarlyFrameDone();
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
                vp_print("[ERROR] Low-resolution image mismatch! Expected: ");
                vp_println_hex(expected);
                vp_print(" Actual x0: ");
                vp_println_hex(display[display_index]);
                vp_print(" Actual x1: ");
                vp_println_hex(display[display_index + 1]);
                vp_print(" Actual y0: ");
                vp_println_hex(display[display_index + 640]);
                vp_print(" Actual y1: ");
                vp_println_hex(display[display_index  + 640 + 1]);
                
                return false;
            }
        }
    }

    vp_println("[VGA] Low-resolution display test passed");
    return true;
}


static bool displayLowResolutionSpriteTest() {
    const uint32_t framebuffer_width = 320;
    const uint32_t framebuffer_height = 240;
    const uint32_t display_width = 640;
    const uint32_t display_height = 480;
    const uint32_t first_sprite_x = 79;
    const uint32_t first_sprite_y = 53;
    const uint32_t second_sprite_x = 327;
    const uint32_t second_sprite_y = 289;
    volatile uint16_t* frame_buffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("");
    vp_println("==========================================");
    vp_println("  LOW RESOLUTION SPRITE TEST START ");
    vp_println("==========================================");

    vp_println("[VGA] Loading frame buffer...");

    for (uint32_t y = 0; y < framebuffer_height; ++y) {
        for (uint32_t x = 0; x < framebuffer_width; ++x) {
            frame_buffer[y * framebuffer_width + x] = test_pixel(x, y);
        }
    }

    vp_println("[VGA] Done!");

    VGA::pixel_s sprite_colors[VGA::COLOR_TABLE_SIZE];
    for (uint32_t index = 0; index < VGA::COLOR_TABLE_SIZE; ++index) {
        sprite_colors[index] = sprite_test_color(index);
    }

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;
    vga.setResolution(VGA::_320x240)
       .setColorTable(sprite_colors)
       .setPatternTable(sprite_test_pattern)
       .setSpritePosition(first_sprite_x, first_sprite_y)
       .enableSprite(true)
       .setFrameBuffer(FRAME_BUFFER_BASE, framebuffer_width * framebuffer_height * 2, &error);

    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] Frame buffer setup error");
        return false;
    }

    vp_println("[VGA] Displaying first sprite position...");
    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");
        return false;
    }

    vga.enableDisplay(false);

    for (uint32_t y = 0; y < display_height; ++y) {
        for (uint32_t x = 0; x < display_width; ++x) {
            const uint16_t expected = sprite_test_expected_pixel(
                x, y, first_sprite_x, first_sprite_y, test_pixel(x >> 1, y >> 1));
            const uint16_t actual = display[y * display_width + x];

            if (actual != expected) {
                vp_print("[ERROR] Low-resolution sprite mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);
                return false;
            }
        }
    }

    vga.setSpritePosition(second_sprite_x, second_sprite_y);

    vp_println("[VGA] Displaying second sprite position...");
    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Frame timeout");
        return false;
    }

    vga.enableDisplay(false);

    for (uint32_t y = 0; y < display_height; ++y) {
        for (uint32_t x = 0; x < display_width; ++x) {
            const uint16_t expected = sprite_test_expected_pixel(
                x, y, second_sprite_x, second_sprite_y, test_pixel(x >> 1, y >> 1));
            const uint16_t actual = display[y * display_width + x];

            if (actual != expected) {
                vp_print("[ERROR] Low-resolution sprite mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);
                return false;
            }
        }
    }

    vp_println("[VGA] Low-resolution sprite test passed");
    return true;
}

#endif
