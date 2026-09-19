#include <stdint.h>

#ifndef VGA_HIGH_RES
#define VGA_HIGH_RES 0
#endif

namespace {

#if VGA_HIGH_RES
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;
constexpr int MAX_ITERATIONS = 56;
#else
constexpr int WIDTH = 320;
constexpr int HEIGHT = 240;
constexpr int MAX_ITERATIONS = 48;
#endif

/* Q2.14 keeps the hot loop on the RV32 integer multiplier. The zoom spans
 * roughly 64:1, which is enough to continuously explore the selected detail
 * while avoiding the expensive 64-bit products used by the first version. */
constexpr int FRACTION_BITS = 14;
constexpr int32_t ESCAPE_RADIUS_SQUARED = 4 << FRACTION_BITS;
constexpr int32_t COMPONENT_LIMIT = 46340;

/* A well-known Seahorse Valley location, suitable for a long zoom. */
constexpr int32_t CENTER_X = -12183; // -0.743643887037151
constexpr int32_t CENTER_Y = 2159;   //  0.131825904205330

constexpr uint32_t INITIAL_SPAN_X = 52429u; // 3.2
constexpr uint32_t INITIAL_SPAN_Y = 39322u; // 2.4
constexpr uint32_t ZOOM_ONE = 1u << 16;
constexpr uint32_t ZOOM_FACTOR = 67174u; // 1.025x per frame
constexpr uint32_t ZOOM_MAX = 64u << 16;

/* RGB444 palette. The low entries are dark blues; the high entries add
 * cyan, violet and warm tones around the Mandelbrot boundary. */
constexpr uint16_t PALETTE[64] = {
    0x001, 0x012, 0x023, 0x034, 0x045, 0x056, 0x067, 0x078,
    0x089, 0x09a, 0x0ab, 0x0bc, 0x0cd, 0x0de, 0x0ef, 0x1ff,
    0x2ff, 0x3ef, 0x4df, 0x5cf, 0x6bf, 0x7af, 0x89f, 0x98f,
    0xa7f, 0xb6f, 0xc5f, 0xd4f, 0xe3f, 0xf2f, 0xf1e, 0xf0d,
    0xf1c, 0xf2b, 0xf3a, 0xf49, 0xf58, 0xf67, 0xf76, 0xf85,
    0xf94, 0xfa3, 0xfb2, 0xfc1, 0xfd0, 0xfe1, 0xff2, 0xff3,
    0xef3, 0xdf3, 0xcf3, 0xbf3, 0xaf3, 0x9f3, 0x8f3, 0x7f3,
    0x6f2, 0x5e2, 0x4d2, 0x3c2, 0x2b2, 0x1a2, 0x092, 0x082
};

uint32_t scaled_span(uint32_t initial_span, uint32_t zoom) {
    return static_cast<uint32_t>(
        (static_cast<uint64_t>(initial_span) * ZOOM_ONE) / zoom);
}

uint16_t color_for_escape(int iteration, int32_t magnitude_squared) {
    if (iteration >= MAX_ITERATIONS)
        return 0x001;

    const uint32_t magnitude = static_cast<uint32_t>(
        magnitude_squared >> (FRACTION_BITS - 6));
    const uint32_t palette_index =
        (static_cast<uint32_t>(iteration) * 5u + magnitude) & 63u;
    return PALETTE[palette_index];
}

} // namespace

void render_fractal(uint16_t* frame, uint32_t frame_number) {
    static uint32_t zoom = ZOOM_ONE;
    static bool zooming_in = true;

    if (frame_number != 0) {
        if (zooming_in) {
            const uint64_t next_zoom =
                (static_cast<uint64_t>(zoom) * ZOOM_FACTOR) >> 16;
            if (next_zoom >= ZOOM_MAX) {
                zoom = ZOOM_MAX;
                zooming_in = false;
            } else {
                zoom = static_cast<uint32_t>(next_zoom);
            }
        } else {
            if (zoom <= ZOOM_FACTOR) {
                zoom = ZOOM_ONE;
                zooming_in = true;
            } else {
                zoom = static_cast<uint32_t>(
                    (static_cast<uint64_t>(zoom) << 16) / ZOOM_FACTOR);
            }
        }
    }

    const uint32_t span_x = scaled_span(INITIAL_SPAN_X, zoom);
    const uint32_t span_y = scaled_span(INITIAL_SPAN_Y, zoom);
    const int32_t step_x = static_cast<int32_t>(span_x / WIDTH);
    const int32_t step_y = static_cast<int32_t>(span_y / HEIGHT);
    const int32_t left = CENTER_X - static_cast<int32_t>(span_x / 2);
    const int32_t top = CENTER_Y + static_cast<int32_t>(span_y / 2);

    for (int y = 0; y < HEIGHT; ++y) {
        const int32_t imaginary = top - y * step_y;
        int32_t real = left;
        const uint32_t row = static_cast<uint32_t>(y * WIDTH);

        for (int x = 0; x < WIDTH; ++x) {
            int32_t zr = 0;
            int32_t zi = 0;
            int32_t magnitude_squared = 0;
            int iteration = 0;

            for (; iteration < MAX_ITERATIONS; ++iteration) {
                /* If one component is already above sqrt(INT32_MAX), the
                 * point has escaped and its square would overflow a 32-bit
                 * product. */
                if (zr > COMPONENT_LIMIT || zr < -COMPONENT_LIMIT ||
                    zi > COMPONENT_LIMIT || zi < -COMPONENT_LIMIT) {
                    magnitude_squared = ESCAPE_RADIUS_SQUARED;
                    break;
                }

                const int32_t zr_squared = (zr * zr) >> FRACTION_BITS;
                const int32_t zi_squared = (zi * zi) >> FRACTION_BITS;
                magnitude_squared = zr_squared + zi_squared;
                if (magnitude_squared > ESCAPE_RADIUS_SQUARED)
                    break;

                /* Multiplication is safe before the shift because both
                 * operands are bounded by COMPONENT_LIMIT. */
                zi = (zr * zi >> (FRACTION_BITS - 1)) + imaginary;
                zr = zr_squared - zi_squared + real;
            }

            frame[row + static_cast<uint32_t>(x)] =
                color_for_escape(iteration, magnitude_squared);
            real += step_x;
        }
    }
}
