#include "../../lib/Serial_IO.h"
#include "../../lib/driver/VGA.h"

#include <stdint.h>

#ifndef DEMO_FRAMES
#define DEMO_FRAMES 0
#endif

#ifndef VGA_HIGH_RES
#define VGA_HIGH_RES 0
#endif

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

constexpr int FOCAL_LENGTH = (WIDTH * 25) / 32;
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
uint16_t* const DEPTH = reinterpret_cast<uint16_t*>(DDR_CPU_BASE + DEPTH_OFFSET);

constexpr int16_t SIN_Q14[64] = {
         0,  1606,  3196,  4756,  6270,  7723,  9102, 10394,
     11585, 12665, 13623, 14449, 15137, 15679, 16069, 16305,
     16384, 16305, 16069, 15679, 15137, 14449, 13623, 12665,
     11585, 10394,  9102,  7723,  6270,  4756,  3196,  1606,
         0, -1606, -3196, -4756, -6270, -7723, -9102,-10394,
    -11585,-12665,-13623,-14449,-15137,-15679,-16069,-16305,
    -16384,-16305,-16069,-15679,-15137,-14449,-13623,-12665,
    -11585,-10394, -9102, -7723, -6270, -4756, -3196, -1606
};

struct Vec3 {
    int16_t x;
    int16_t y;
    int16_t z;
};

struct ScreenVertex {
    int16_t x;
    int16_t y;
    uint16_t z;
};

constexpr Vec3 CUBE[8] = {
    {-64, -64, -64}, { 64, -64, -64},
    { 64,  64, -64}, {-64,  64, -64},
    {-64, -64,  64}, { 64, -64,  64},
    { 64,  64,  64}, {-64,  64,  64}
};

constexpr uint8_t FACES[6][4] = {
    {0, 1, 2, 3}, {4, 7, 6, 5},
    {0, 3, 7, 4}, {1, 5, 6, 2},
    {0, 4, 5, 1}, {3, 2, 6, 7}
};

constexpr uint16_t FACE_COLORS[6] = {
    0xf22, 0x2df, 0x3e4, 0xf93, 0xa4f, 0xff2
};

inline int32_t edge(const ScreenVertex& a,
                    const ScreenVertex& b,
                    int32_t x,
                    int32_t y) {
    return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
}

void clear_targets(uint16_t* frame) {
    for (int y = 0; y < HEIGHT; ++y) {
        const uint16_t background = static_cast<uint16_t>(
            0x001 + (((y * 8) / HEIGHT) << 4));
        const uint32_t row = y * WIDTH;
        for (int x = 0; x < WIDTH; ++x) {
            frame[row + x] = background;
            DEPTH[row + x] = 0xffff;
        }
    }
}

void draw_triangle(uint16_t* frame,
                   ScreenVertex a,
                   ScreenVertex b,
                   ScreenVertex c,
                   uint16_t color) {
    int32_t area = edge(a, b, c.x, c.y);
    if (area == 0)
        return;
    if (area < 0) {
        const ScreenVertex tmp = b;
        b = c;
        c = tmp;
        area = -area;
    }

    int min_x = a.x;
    int max_x = a.x;
    int min_y = a.y;
    int max_y = a.y;
    const ScreenVertex vertices[2] = {b, c};
    for (const ScreenVertex& vertex : vertices) {
        if (vertex.x < min_x) min_x = vertex.x;
        if (vertex.x > max_x) max_x = vertex.x;
        if (vertex.y < min_y) min_y = vertex.y;
        if (vertex.y > max_y) max_y = vertex.y;
    }
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= WIDTH) max_x = WIDTH - 1;
    if (max_y >= HEIGHT) max_y = HEIGHT - 1;

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            const int32_t w0 = edge(b, c, x, y);
            const int32_t w1 = edge(c, a, x, y);
            const int32_t w2 = edge(a, b, x, y);
            if (w0 < 0 || w1 < 0 || w2 < 0)
                continue;

            const uint16_t z = static_cast<uint16_t>(
                (w0 * a.z + w1 * b.z + w2 * c.z) / area);
            const uint32_t index = y * WIDTH + x;
            if (z < DEPTH[index]) {
                DEPTH[index] = z;
                frame[index] = color;
            }
        }
    }
}

void render_cube(uint16_t* frame, uint8_t angle) {
    clear_targets(frame);

    const int32_t sy = SIN_Q14[angle & 63];
    const int32_t cy = SIN_Q14[(angle + 16) & 63];
    const uint8_t x_angle = static_cast<uint8_t>((angle >> 1) + 7);
    const int32_t sx = SIN_Q14[x_angle & 63];
    const int32_t cx = SIN_Q14[(x_angle + 16) & 63];
    ScreenVertex projected[8];

    for (unsigned i = 0; i < 8; ++i) {
        const int32_t x1 = (CUBE[i].x * cy + CUBE[i].z * sy) >> 14;
        const int32_t z1 = (-CUBE[i].x * sy + CUBE[i].z * cy) >> 14;
        const int32_t y2 = (CUBE[i].y * cx - z1 * sx) >> 14;
        const int32_t z2 = (CUBE[i].y * sx + z1 * cx) >> 14;
        const int32_t depth = z2 + 280;

        projected[i].x = static_cast<int16_t>(
            WIDTH / 2 + (x1 * FOCAL_LENGTH) / depth);
        projected[i].y = static_cast<int16_t>(
            HEIGHT / 2 - (y2 * FOCAL_LENGTH) / depth);
        projected[i].z = static_cast<uint16_t>(depth);
    }

    for (unsigned face = 0; face < 6; ++face) {
        const ScreenVertex& a = projected[FACES[face][0]];
        const ScreenVertex& b = projected[FACES[face][1]];
        const ScreenVertex& c = projected[FACES[face][2]];
        const ScreenVertex& d = projected[FACES[face][3]];
        draw_triangle(frame, a, b, c, FACE_COLORS[face]);
        draw_triangle(frame, a, c, d, FACE_COLORS[face]);
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
    volatile const uint16_t* const sweep = DEPTH;
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
    Serial_IO::println("[VGA] ZenithSoC rotating cube demo");
    Serial_IO::printf("[VGA] render resolution: %ux%u RGB444\n", WIDTH, HEIGHT);
    Serial_IO::printf("[VGA] buffers: 0x%x / 0x%x (%u bytes each)\n",
                      FRAME0_OFFSET,
                      FRAME1_OFFSET,
                      FRAME_BYTES);

    VGA vga;
    VGA::error_e error = VGA::NO_ERROR;

    render_cube(FRAME0, 0);
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
        render_cube(back,
                    static_cast<uint8_t>(((frame + 1) * ANGLE_STEP) & 63));
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
            Serial_IO::printf("[VGA] displayed frame %u, angle=%u/64\n",
                              frame,
                              (frame * ANGLE_STEP) & 63);

#if DEMO_FRAMES > 0
        if (frame >= DEMO_FRAMES) {
            Serial_IO::println("[VGA] finite simulation completed successfully");
            tohost = 1;
            return 0;
        }
#endif
    }
}
