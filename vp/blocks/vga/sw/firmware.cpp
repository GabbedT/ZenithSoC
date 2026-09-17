#include "platform.h"
#include "test_cases.h"

/* Run the complete VGA VP regression suite and report the first failure. */
extern "C" int main(void) {
    if (!displayHighResolutionTest()) {
        TEST_FAIL();
    }

    if (!displayHighResolutionSpriteTest()) {
        TEST_FAIL();
    }

    if (!displayLowResolutionTest()) {
        TEST_FAIL();
    }

    if (!displayLowResolutionSpriteTest()) {
        TEST_FAIL();
    }

    if (!doubleBufferingTest()) {
        TEST_FAIL();
    }

    TEST_PASS();
}
