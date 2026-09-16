#include "platform.h"
#include "test_cases.h"

extern "C" int main(void) {
    if (!displayHighResolutionTest()) {
        TEST_FAIL();
    }

    TEST_PASS();
}
