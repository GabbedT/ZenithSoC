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

constexpr int Q28_BITS = 28;
constexpr int Q14_BITS = 14;
constexpr int32_t Q14_ONE = 1 << Q14_BITS;
constexpr int32_t Q14_ESCAPE = 4 << Q14_BITS;
constexpr int32_t Q14_COMPONENT_LIMIT = 46340;
constexpr int32_t CENTER_X_Q14 = -12184;
constexpr int32_t CENTER_Y_Q14 = 2159;
constexpr int32_t Q28_ONE = 1 << Q28_BITS;
constexpr int32_t Q28_ESCAPE = 4 << Q28_BITS;
constexpr int32_t CENTER_X_Q28 = -199620386;
constexpr int32_t CENTER_Y_Q28 = 35386747;
constexpr int64_t CENTER_X_Q60 = -857363029134556279ll;
constexpr int64_t CENTER_Y_Q60 = 151984919822567140ll;
#if VGA_HIGH_RES
constexpr uint64_t INITIAL_PIXEL_STEP_Q60 = 5764607523034235ull;
#else
constexpr uint64_t INITIAL_PIXEL_STEP_Q60 = 11529215046068470ull;
#endif
constexpr uint64_t DEEP_PIXEL_STEP_Q60 = 32ull << 32;
constexpr uint64_t FAST_PIXEL_STEP_Q60 = 16ull << 46;
constexpr int64_t Q60_ESCAPE = 4ll << 60;
constexpr int MAX_DEEP_ITERATIONS = 512;
int deep_iteration_limit;

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

inline int32_t multiply_q28(int32_t a, int32_t b) {
    return static_cast<int32_t>((static_cast<int64_t>(a) * b) >> Q28_BITS);
}

inline int32_t multiply_q14(int32_t a, int32_t b) {
    return (a * b) >> Q14_BITS;
}

__attribute__((noinline)) int64_t multiply_q60(int64_t a, int64_t b) {
    const bool negative = (a < 0) != (b < 0);
    const uint64_t ua = a < 0 ? 0ull - static_cast<uint64_t>(a) : static_cast<uint64_t>(a);
    const uint64_t ub = b < 0 ? 0ull - static_cast<uint64_t>(b) : static_cast<uint64_t>(b);
    const uint32_t a0 = static_cast<uint32_t>(ua);
    const uint32_t a1 = static_cast<uint32_t>(ua >> 32);
    const uint32_t b0 = static_cast<uint32_t>(ub);
    const uint32_t b1 = static_cast<uint32_t>(ub >> 32);
    const uint64_t p00 = static_cast<uint64_t>(a0) * b0;
    const uint64_t p01 = static_cast<uint64_t>(a0) * b1;
    const uint64_t p10 = static_cast<uint64_t>(a1) * b0;
    const uint64_t p11 = static_cast<uint64_t>(a1) * b1;
    uint64_t low = p00;
    const uint64_t add01 = p01 << 32;
    low += add01;
    uint64_t high = p11 + (p01 >> 32) + static_cast<uint64_t>(low < add01);
    const uint64_t add10 = p10 << 32;
    const uint64_t before10 = low;
    low += add10;
    high += (p10 >> 32) + static_cast<uint64_t>(low < before10);
    const uint64_t magnitude = (high << 4) | (low >> 60);
    if (!negative)
        return static_cast<int64_t>(magnitude);
    return -static_cast<int64_t>(magnitude) - static_cast<int64_t>((low & ((1ull << 60) - 1)) != 0);
}

inline bool definitely_inside_q28(int32_t real, int32_t imaginary) {
    constexpr int32_t bulb_y_limit = Q28_ONE >> 2;
    if (real >= -(Q28_ONE + (Q28_ONE >> 2)) && real <= -(Q28_ONE - (Q28_ONE >> 2)) && imaginary >= -bulb_y_limit && imaginary <= bulb_y_limit) {
        const int32_t dx = real + Q28_ONE;
        const int64_t distance_squared = static_cast<int64_t>(dx) * dx + static_cast<int64_t>(imaginary) * imaginary;
        if (distance_squared <= (1ll << 52))
            return true;
    }

    constexpr int32_t cardioid_y_limit = 174483047;
    if (real >= -(Q28_ONE * 3 / 4) && real <= (Q28_ONE >> 1) && imaginary >= -cardioid_y_limit && imaginary <= cardioid_y_limit) {
        const int32_t xm = real - (Q28_ONE >> 2);
        const int32_t y_squared = multiply_q28(imaginary, imaginary);
        const int32_t q = multiply_q28(xm, xm) + y_squared;
        if (multiply_q28(q, q + xm) <= (y_squared >> 2))
            return true;
    }

    return false;
}

inline bool definitely_inside_q14(int32_t real, int32_t imaginary) {
    constexpr int32_t bulb_y_limit = Q14_ONE >> 2;
    if (real >= -(Q14_ONE + (Q14_ONE >> 2)) && real <= -(Q14_ONE - (Q14_ONE >> 2)) && imaginary >= -bulb_y_limit && imaginary <= bulb_y_limit) {
        const int32_t dx = real + Q14_ONE;
        if (dx * dx + imaginary * imaginary <= (1 << 24))
            return true;
    }

    constexpr int32_t cardioid_y_limit = 10650;
    if (real >= -(Q14_ONE * 3 / 4) && real <= (Q14_ONE >> 1) && imaginary >= -cardioid_y_limit && imaginary <= cardioid_y_limit) {
        const int32_t xm = real - (Q14_ONE >> 2);
        const int32_t y_squared = multiply_q14(imaginary, imaginary);
        const int32_t q = multiply_q14(xm, xm) + y_squared;
        if (multiply_q14(q, q + xm) <= (y_squared >> 2))
            return true;
    }

    return false;
}

inline uint16_t color_for_escape(int iteration, int iteration_limit, uint32_t magnitude_band) {
    if (iteration >= iteration_limit)
        return 0x001;
    return PALETTE[(static_cast<uint32_t>(iteration) * 2u + magnitude_band) & 63u];
}

template <bool CHECK_INTERIOR>
inline uint16_t render_point_q28(int32_t real, int32_t imaginary) {
    if (CHECK_INTERIOR && definitely_inside_q28(real, imaginary))
        return 0x001;

    int32_t zr = real;
    int32_t zi = imaginary;
    int32_t magnitude_squared = 0;
    int iteration = 1;

    while (iteration < deep_iteration_limit) {
        if (zr > (Q28_ONE * 2) || zr < -(Q28_ONE * 2) || zi > (Q28_ONE * 2) || zi < -(Q28_ONE * 2)) {
            magnitude_squared = Q28_ESCAPE + 1;
            break;
        }
        const int32_t zr_squared = multiply_q28(zr, zr);
        const int32_t zi_squared = multiply_q28(zi, zi);
        magnitude_squared = zr_squared + zi_squared;
        if (magnitude_squared > Q28_ESCAPE)
            break;
        zi = multiply_q28(zr, zi) * 2 + imaginary;
        zr = zr_squared - zi_squared + real;
        ++iteration;
    }

    return color_for_escape(iteration, deep_iteration_limit, (static_cast<uint32_t>(magnitude_squared) >> 26) & 3u);
}

inline uint16_t render_point_q14(int32_t real, int32_t imaginary) {
    if (definitely_inside_q14(real, imaginary))
        return 0x001;

    int32_t zr = real;
    int32_t zi = imaginary;
    int32_t magnitude_squared = 0;
    int iteration = 1;

    while (iteration < MAX_ITERATIONS) {
        if (zr > Q14_COMPONENT_LIMIT || zr < -Q14_COMPONENT_LIMIT || zi > Q14_COMPONENT_LIMIT || zi < -Q14_COMPONENT_LIMIT) {
            magnitude_squared = Q14_ESCAPE + 1;
            break;
        }
        const int32_t zr_squared = multiply_q14(zr, zr);
        const int32_t zi_squared = multiply_q14(zi, zi);
        magnitude_squared = zr_squared + zi_squared;
        if (magnitude_squared > Q14_ESCAPE)
            break;
        zi = multiply_q14(zr, zi) * 2 + imaginary;
        zr = zr_squared - zi_squared + real;
        ++iteration;
    }

    return color_for_escape(iteration, MAX_ITERATIONS, (static_cast<uint32_t>(magnitude_squared) >> 12) & 3u);
}

__attribute__((noinline)) uint16_t render_point_q28_outer(int32_t real, int32_t imaginary) {
    return render_point_q28<true>(real, imaginary);
}

__attribute__((noinline)) uint16_t render_point_q28_deep(int32_t real, int32_t imaginary) {
    return render_point_q28<false>(real, imaginary);
}

__attribute__((noinline)) uint16_t render_point_q60(int64_t real, int64_t imaginary) {
    int64_t zr = real;
    int64_t zi = imaginary;
    int64_t magnitude_squared = 0;
    int iteration = 1;

    while (iteration < deep_iteration_limit) {
        if (zr > (2ll << 60) || zr < -(2ll << 60) || zi > (2ll << 60) || zi < -(2ll << 60)) {
            magnitude_squared = Q60_ESCAPE + 1;
            break;
        }
        const int64_t zr_squared = multiply_q60(zr, zr);
        const int64_t zi_squared = multiply_q60(zi, zi);
        magnitude_squared = zr_squared + zi_squared;
        if (magnitude_squared > Q60_ESCAPE)
            break;
        zi = multiply_q60(zr, zi) * 2 + imaginary;
        zr = zr_squared - zi_squared + real;
        ++iteration;
    }

    return color_for_escape(iteration, deep_iteration_limit, (static_cast<uint64_t>(magnitude_squared) >> 58) & 3u);
}

template <typename Fixed, uint16_t (*RenderPoint)(Fixed, Fixed)>
void render_exact(uint16_t* frame, Fixed center_x, Fixed center_y, Fixed step) {
    const Fixed left = center_x - static_cast<Fixed>(WIDTH / 2) * step;
    const Fixed top = center_y + static_cast<Fixed>(HEIGHT / 2) * step;
    Fixed imaginary = top;
    uint16_t* pixel = frame;

    for (int y = 0; y < HEIGHT; ++y) {
        Fixed real = left;
        for (int x = 0; x < WIDTH; ++x) {
            *pixel++ = RenderPoint(real, imaginary);
            real += step;
        }
        imaginary -= step;
    }
}

inline bool viewport_can_contain_analytic_interior(int32_t step) {
    if (step < (Q28_ONE / (WIDTH * 16)))
        return false;
    const int32_t left = CENTER_X_Q28 - (WIDTH / 2) * step;
    const int32_t right = CENTER_X_Q28 + (WIDTH / 2) * step;
    const int32_t bottom = CENTER_Y_Q28 - (HEIGHT / 2) * step;
    const int32_t top = CENTER_Y_Q28 + (HEIGHT / 2) * step;
    return left <= (Q28_ONE >> 1) && right >= -(Q28_ONE + (Q28_ONE >> 2)) && bottom <= 174483047 && top >= -174483047;
}

} // namespace

void render_fractal(uint16_t* frame, uint32_t frame_number) {
    static uint64_t pixel_step_q60;

    if (frame_number == 0 || pixel_step_q60 == 0) {
        pixel_step_q60 = INITIAL_PIXEL_STEP_Q60;
    } else if (pixel_step_q60 > 1) {
        const uint64_t decrement = (pixel_step_q60 + 40) / 41;
        pixel_step_q60 = decrement < pixel_step_q60 ? pixel_step_q60 - decrement : 1;
    }

    const uint32_t requested_iterations = static_cast<uint32_t>(MAX_ITERATIONS) + (frame_number >> 2);
    deep_iteration_limit = requested_iterations < static_cast<uint32_t>(MAX_DEEP_ITERATIONS) ? static_cast<int>(requested_iterations) : MAX_DEEP_ITERATIONS;

    if (pixel_step_q60 >= FAST_PIXEL_STEP_Q60) {
        const int32_t step_q14 = static_cast<int32_t>((pixel_step_q60 + (1ull << 45)) >> 46);
        render_exact<int32_t, render_point_q14>(frame, CENTER_X_Q14, CENTER_Y_Q14, step_q14);
    } else if (pixel_step_q60 >= DEEP_PIXEL_STEP_Q60) {
        const int32_t step_q28 = static_cast<int32_t>((pixel_step_q60 + (1ull << 31)) >> 32);
        if (viewport_can_contain_analytic_interior(step_q28))
            render_exact<int32_t, render_point_q28_outer>(frame, CENTER_X_Q28, CENTER_Y_Q28, step_q28);
        else
            render_exact<int32_t, render_point_q28_deep>(frame, CENTER_X_Q28, CENTER_Y_Q28, step_q28);
    } else {
        render_exact<int64_t, render_point_q60>(frame, CENTER_X_Q60, CENTER_Y_Q60, static_cast<int64_t>(pixel_step_q60));
    }
}
