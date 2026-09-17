#ifndef TEST_CASES_H
#define TEST_CASES_H

#include "platform.h"
#include "VGA.h"

#include <stdint.h>

#define FRAME_BUFFER_BASE 0x01000000
#define DISPLAY_BASE      0x08000000

/*
 * Generate the deterministic background pattern used by the display tests.
 * The coordinate bits are distributed across the RGB nibbles so that the
 * tests can detect address, pixel-order, and row/column indexing errors.
 */
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
static bool doubleBufferingTest();

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

/*
 * Generate a deterministic color-table entry for a sprite index. Each color
 * channel uses a different multiplier, making neighboring indices visibly
 * different and exposing color-table or sprite-indexing errors.
 */
static VGA::pixel_s sprite_test_color(uint8_t index) {
    VGA::pixel_s color = {};

    color.red = static_cast<uint16_t>((index * 3 + 1) & 0xF);
    color.green = static_cast<uint16_t>((index * 5 + 2) & 0xF);
    color.blue = static_cast<uint16_t>((index * 7 + 4) & 0xF);

    return color;
}

/*
 * Convert the structured VGA sprite color into the 16-bit framebuffer/display
 * representation used when comparing the rendered output.
 */
static uint16_t sprite_test_color_value(uint8_t index) {
    return VGA::pixel2int16(sprite_test_color(index));
}

/*
 * Calculate the expected output pixel for a background plus an 8x8 sprite.
 * Sprite index 0xF is treated as transparent; every other index replaces the
 * background with the corresponding color-table entry.
 */
static uint16_t sprite_test_expected_pixel(uint32_t x, uint32_t y, uint32_t sprite_x, uint32_t sprite_y, uint16_t background) {
    if ((x < sprite_x) || (x >= sprite_x + 8) || (y < sprite_y) || (y >= sprite_y + 8)) {
        return background;
    }

    const uint8_t color_index = sprite_test_pattern[(y - sprite_y) * 8 + (x - sprite_x)];

    return color_index == 0xF ? background : sprite_test_color_value(color_index);
}

/*
 * Poll the sticky early-frame-done event until the VGA has finished the last
 * visible line. The event is cleared with the driver's write-one-to-clear
 * operation, and a bounded loop prevents a stalled display from hanging the
 * firmware forever.
 */
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

/*
 * Generate a second deterministic pattern that is intentionally different
 * from test_pixel(), allowing the double-buffer test to detect stale pixels
 * from the previous framebuffer.
 */
static inline uint16_t double_buffer_pixel(uint32_t x, uint32_t y) {
    const uint16_t red = 0xD;
    const uint16_t green = static_cast<uint16_t>((x >> 3) & 0xF);
    const uint16_t blue = static_cast<uint16_t>((x + y) & 0xF);

    return static_cast<uint16_t>((red << 8) | (green << 4) | blue);
}

/*
 * Fill a 640x480 framebuffer, display one complete frame without sprites,
 * then compare every pixel in the simulated display memory with the expected
 * coordinate pattern.
 */
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


/*
 * Fill a 640x480 background, configure the sprite pattern and color table,
 * and render two frames with the sprite at different positions. Each frame
 * is checked pixel by pixel, including transparent sprite entries that must
 * preserve the framebuffer background.
 */
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


/*
 * Fill a 320x240 framebuffer and display it in low-resolution mode. Every
 * source pixel must appear in a 2x2 block, so the test checks horizontal and
 * vertical duplication for all source coordinates.
 */
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


/*
 * Combine low-resolution 2x2 pixel duplication with sprite rendering. The
 * test renders the same background twice with the sprite at two positions and
 * verifies the complete 640x480 output, including transparent sprite pixels.
 */
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


/*
 * Verify framebuffer switching at the early-frame boundary. The test fills
 * two framebuffer regions with different patterns, displays the first one,
 * changes the framebuffer base after earlyFrameDone, and waits for the next
 * frame. The final display memory must contain only the second pattern,
 * proving that the old sequencer and line-buffer contents were flushed.
 */
static bool doubleBufferingTest() {
    const uint32_t framebuffer_width = 320;
    const uint32_t framebuffer_height = 240;
    const uint32_t display_width = 640;
    const uint32_t display_height = 480;
    const uint32_t framebuffer_size = framebuffer_width * framebuffer_height * 2;
    const uint32_t second_framebuffer_base = FRAME_BUFFER_BASE + framebuffer_size;

    volatile uint16_t* first_framebuffer = reinterpret_cast<volatile uint16_t*>(FRAME_BUFFER_BASE);
    volatile uint16_t* second_framebuffer = reinterpret_cast<volatile uint16_t*>(second_framebuffer_base);
    volatile uint16_t* display = reinterpret_cast<volatile uint16_t*>(DISPLAY_BASE);

    vp_println("");
    vp_println("===========================================");
    vp_println("       VGA DOUBLE BUFFERING TEST START    ");
    vp_println("===========================================");

    vp_println("[VGA] Loading first frame buffer...");

    for (uint32_t y = 0; y < framebuffer_height; ++y) {
        for (uint32_t x = 0; x < framebuffer_width; ++x) {
            first_framebuffer[y * framebuffer_width + x] = test_pixel(x, y);
        }
    }

    vp_println("[VGA] Loading second frame buffer...");

    for (uint32_t y = 0; y < framebuffer_height; ++y) {
        for (uint32_t x = 0; x < framebuffer_width; ++x) {
            second_framebuffer[y * framebuffer_width + x] = double_buffer_pixel(x, y);
        }
    }

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;
    vga.enableSprite(false)
       .setResolution(VGA::_320x240)
       .setFrameBuffer(FRAME_BUFFER_BASE, framebuffer_size, &error);

    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] First frame buffer setup error");

        return false;
    }

    vp_println("[VGA] Displaying first buffer...");
    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] First frame timeout");

        return false;
    }

    const uint32_t first_display_index = 0;
    if (display[first_display_index] != test_pixel(0, 0)) {
        vp_println("[ERROR] First frame was not displayed");
        
        return false;
    }

    vp_println("[VGA] Switching framebuffer at early frame done...");
    vga.setFrameBuffer(second_framebuffer_base, framebuffer_size, &error);

    if (error != VGA::NO_ERROR) {
        vp_println("[ERROR] Second frame buffer setup error");
        return false;
    }

    if (!waitForFrame(vga)) {
        vp_println("[ERROR] Second frame timeout");
        return false;
    }

    vga.enableDisplay(false);

    vp_println("[VGA] Checking second displayed buffer...");

    for (uint32_t y = 0; y < display_height; ++y) {
        for (uint32_t x = 0; x < display_width; ++x) {
            const uint16_t expected = double_buffer_pixel(x >> 1, y >> 1);
            const uint16_t actual = display[y * display_width + x];

            if (actual != expected) {
                vp_print("[ERROR] Double buffering mismatch! Expected: ");
                vp_print_hex(expected);
                vp_print(" Actual: ");
                vp_println_hex(actual);
                return false;
            }
        }
    }

    vp_println("[VGA] Double buffering test passed");
    return true;
}

#endif
