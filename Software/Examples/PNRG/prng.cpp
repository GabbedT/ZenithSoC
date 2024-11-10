#include "../../Library/SerialOut.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Driver/PRNG.h"

#include <inttypes.h>

extern "C" void prng() {
    SerialOut::init();
    PRNG::init();

    Timer timer(0);
    timer.init(-1, Timer::ONE_SHOT);

    while (1) {
        SerialOut::printf("Random number: 0x%xd ", PRNG::random());
        // timer.delay(1'000);

        /* Cancel current line */
        // SerialOut::write("\x1b[2K"); 
        // SerialOut::write("\x1b[1A");
    }
}