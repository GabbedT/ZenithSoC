#include <stdint.h>

/*
 * The target starts with an ADDI that returns OLD_VALUE.  Keeping the target
 * on a cache-line boundary makes the test independent from adjacent code.
 */
asm(
    ".section .text.flush_cache_target, \"ax\"\n"
    ".balign 16\n"
    ".global flush_cache_target\n"
    ".type flush_cache_target, @function\n"
    "flush_cache_target:\n"
    "addi a0, zero, 17\n"
    "ret\n"
    ".size flush_cache_target, .-flush_cache_target\n"
    ".previous\n");

extern "C" uint32_t flush_cache_target();

namespace {

    constexpr uint32_t OLD_VALUE = 17;
    constexpr uint32_t NEW_VALUE = 42;

    /* Encoding of `addi a0, zero, 42`. */
    constexpr uint32_t NEW_TARGET_INSTRUCTION = 0x02A00513;

}


extern "C" int main() {
    /* Fill the instruction-cache line with the original target. */
    if (flush_cache_target() != OLD_VALUE) {
        return 1;
    }

    /* Dirty the corresponding data-cache line without updating DDR yet. */
    uint32_t* target = reinterpret_cast<uint32_t*>(&flush_cache_target);
    uint32_t originalInstruction;

    /* Allocate the target line in D$ so the following store is a dirty hit,
     * rather than a write-through store miss. */
    asm volatile ("lw %0, 0(%1)"
                  : "=r" (originalInstruction)
                  : "r" (target)
                  : "memory");

    if (originalInstruction != 0x01100513) {
        return 3;
    }

    asm volatile ("sw %0, 0(%1)"
                  :: "r" (NEW_TARGET_INSTRUCTION), "r" (target)
                  : "memory");

    /* ZenithSoC defines FENCE as a complete D$ writeback/invalidate followed
     * by I$ invalidation.  Retirement must wait until both operations finish. */
    asm volatile ("fence rw, rw" ::: "memory");

    /* This returns NEW_VALUE only if the D$ update reached DDR and the stale
     * I$ line was discarded and refilled from that updated DDR contents. */
    if (flush_cache_target() != NEW_VALUE) {
        return 2;
    }

    return 0;
}
