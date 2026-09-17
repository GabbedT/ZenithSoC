#include <stddef.h>
#include <stdint.h>

extern "C" void* memcpy(void* destination, const void* source, size_t size) {
    auto* dst = static_cast<uint8_t*>(destination);
    const auto* src = static_cast<const uint8_t*>(source);
    for (size_t index = 0; index < size; ++index)
        dst[index] = src[index];
    return destination;
}

extern "C" void* memset(void* destination, int value, size_t size) {
    auto* dst = static_cast<uint8_t*>(destination);
    for (size_t index = 0; index < size; ++index)
        dst[index] = static_cast<uint8_t>(value);
    return destination;
}
