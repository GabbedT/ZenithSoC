#include <stddef.h>
#include <stdint.h>

/* The boot image is freestanding and cannot rely on a hosted libc. */
extern "C" void* memcpy(void* destination, const void* source, size_t size) {
    uint8_t* dst = (uint8_t*) destination;
    const uint8_t* src = (const uint8_t*) source;

    for (size_t index = 0; index < size; ++index) {
        dst[index] = src[index];
    }

    return destination;
}

extern "C" void* memset(void* destination, int value, size_t size) {
    uint8_t* dst = (uint8_t*) destination;

    for (size_t index = 0; index < size; ++index) {
        dst[index] = (uint8_t) value;
    }

    return destination;
}
