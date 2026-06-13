#ifndef PRNG_CPP
#define PRNG_CPP

#include "../Library/Driver/PRNG.h"
#include "../Library/mmio.h"

#include <inttypes.h>

/****************************************************************/
/*                             DATA                             */
/****************************************************************/

volatile uint64_t* const PRNG::lfsr = (volatile uint64_t*) PRNG_BASE;

uint64_t PRNG::currentSeed;


/****************************************************************/
/*                         INITIALIZER                          */
/****************************************************************/

void PRNG::init(uint64_t seed) {
    PRNG::currentSeed = seed;

    *lfsr = seed;
};


/****************************************************************/
/*                       RANDOM GENERATORS                      */
/****************************************************************/

uint64_t PRNG::random() {
    return *lfsr;
};


uint64_t PRNG::randomBool() {
    return *lfsr & 1;
};


uint64_t PRNG::random(uint64_t min, uint64_t max) {
    if (min > max) {
        uint tmp = min;

        min = max;
        max = tmp;
    }

    return min + (*lfsr % (max - min + 1));
};

#endif 