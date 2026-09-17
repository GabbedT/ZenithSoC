#include "platform.h"
#include "test_cases.h"

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

    TEST_PASS();
}
