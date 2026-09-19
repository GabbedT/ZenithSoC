#include "../../lib/Serial_IO.h"
#include "../../lib/driver/VGA.h"

#include <stdint.h>

#ifndef DEMO_FRAMES
#define DEMO_FRAMES 0
#endif

#ifndef VGA_HIGH_RES
#define VGA_HIGH_RES 0
#endif

void render_cube(uint16_t* frame, uint8_t angle);
void render_fractal(uint16_t* frame, uint32_t frame_number);

namespace {

#if VGA_HIGH_RES
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;
constexpr VGA::resolution_e VGA_RESOLUTION = VGA::_640x480_;
constexpr uint8_t ANGLE_STEP = 2;
#else
constexpr int WIDTH = 320;
constexpr int HEIGHT = 240;
constexpr VGA::resolution_e VGA_RESOLUTION = VGA::_320x240;
constexpr uint8_t ANGLE_STEP = 1;
#endif

constexpr uint32_t FRAME_PIXELS = WIDTH * HEIGHT;
constexpr uint32_t FRAME_BYTES = FRAME_PIXELS * sizeof(uint16_t);

/* VGA addresses are DDR-relative; CPU addresses include the 0x8000_0000
 * cached DDR mapping. Keep the application image and render targets apart. */
constexpr uint32_t DDR_CPU_BASE = 0x80000000u;
constexpr uint32_t FRAME0_OFFSET = 0x01000000u;
constexpr uint32_t FRAME1_OFFSET = FRAME0_OFFSET + FRAME_BYTES;
constexpr uint32_t DEPTH_OFFSET = 0x01200000u;

uint16_t* const FRAME0 = reinterpret_cast<uint16_t*>(DDR_CPU_BASE + FRAME0_OFFSET);
uint16_t* const FRAME1 = reinterpret_cast<uint16_t*>(DDR_CPU_BASE + FRAME1_OFFSET);

#if defined(VGA_DEMO_FRACTAL)
constexpr bool FRACTAL_DEMO = true;
#else
constexpr bool FRACTAL_DEMO = false;
#endif

void render_demo(uint16_t* frame, uint32_t frame_number) {
    if (FRACTAL_DEMO) {
        render_fractal(frame, frame_number);
    } else {
        render_cube(frame,
                    static_cast<uint8_t>((frame_number * ANGLE_STEP) & 63));
    }
}

void publish_framebuffer() {
    /* The VGA master is non-coherent. Sweep one 8 KiB direct-mapped cache
     * footprint first: every possible framebuffer line is evicted and all
     * older stores have time to drain before FENCE starts the global flush.
     * This also avoids entering the current flush engine while the final
     * store-controller transaction is still active. */
    constexpr uint32_t CACHE_BYTES = 8 * 1024;
    constexpr uint32_t CACHE_LINE_BYTES = 16;
    volatile const uint16_t* const sweep =
        reinterpret_cast<volatile const uint16_t*>(DDR_CPU_BASE + DEPTH_OFFSET);
    uint16_t sink = 0;

    for (uint32_t byte = 0; byte < CACHE_BYTES; byte += CACHE_LINE_BYTES)
        sink ^= sweep[byte / sizeof(uint16_t)];

    asm volatile ("" :: "r"(sink) : "memory");
    asm volatile ("fence rw, rw" ::: "memory");
}

void wait_for_frame_boundary(VGA& vga) {
    vga.clearEarlyFrameDone();
    while (!vga.earlyFrameDone()) {
        asm volatile ("nop");
    }
    vga.clearEarlyFrameDone();
}

} // namespace

extern "C" {
volatile uint32_t tohost __attribute__((section(".tohost"), used)) = 0;
}

extern "C" int main() {
    Serial_IO::init(115200, false);
#if defined(VGA_DEMO_FRACTAL)
    Serial_IO::println("[VGA] ZenithSoC continuous fractal zoom demo");
#else
    Serial_IO::println("[VGA] ZenithSoC rotating cube demo");
#endif
    Serial_IO::printf("[VGA] render resolution: %ux%u RGB444\n", WIDTH, HEIGHT);
    Serial_IO::printf("[VGA] buffers: 0x%x / 0x%x (%u bytes each)\n",
                      FRAME0_OFFSET,
                      FRAME1_OFFSET,
                      FRAME_BYTES);

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;

    render_demo(FRAME0, 0);
    publish_framebuffer();

    vga.enableSprite(false)
       .setResolution(VGA_RESOLUTION)
       .setFrameBuffer(FRAME0_OFFSET, FRAME_BYTES, &error);
    if (error != VGA::NO_ERROR) {
        Serial_IO::printf("[VGA] framebuffer configuration failed: %d\n", error);
        tohost = 3;
        return 1;
    }

    vga.clearEarlyFrameDone();
    vga.enableDisplay(true);
    Serial_IO::println("[VGA] scanout enabled; starting animation");

    uint16_t* front = FRAME0;
    uint16_t* back = FRAME1;
    uint32_t front_offset = FRAME0_OFFSET;
    uint32_t back_offset = FRAME1_OFFSET;
    uint32_t frame = 0;

    while (true) {
        render_demo(back, frame + 1);
        publish_framebuffer();
        wait_for_frame_boundary(vga);

        vga.setFrameBuffer(back_offset, FRAME_BYTES, &error);
        if (error != VGA::NO_ERROR) {
            Serial_IO::printf("[VGA] buffer swap failed: %d\n", error);
            tohost = 5;
            return 2;
        }

        uint16_t* old_front = front;
        front = back;
        back = old_front;
        const uint32_t old_front_offset = front_offset;
        front_offset = back_offset;
        back_offset = old_front_offset;
        ++frame;

        if (frame == 1 || (frame % 16) == 0)
            Serial_IO::printf("[VGA] displayed frame %u\n", frame);

#if DEMO_FRAMES > 0
        if (frame >= DEMO_FRAMES) {
            Serial_IO::println("[VGA] finite simulation completed successfully");
            tohost = 1;
            return 0;
        }
#endif
    }
}
