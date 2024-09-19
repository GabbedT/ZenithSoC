#ifndef PRNG_H
#define PRNG_H

#include <inttypes.h>

#include "../mmio.h"
#include "../platform.h"

class PRNG {

public: 

    static uint64_t currentSeed;


private: 

    /* Pointer to LFSR register */
    static volatile uint64_t* const lfsr;


public:

    static void init(uint64_t seed = 0xDEADBEEFBEEFDEAD);

    static uint64_t random();

    static uint64_t random(uint64_t min, uint64_t max);
};

#endif 