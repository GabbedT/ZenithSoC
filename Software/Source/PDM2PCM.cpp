#ifndef PDM2PCM_CPP
#define PDM2PCM_CPP

#include "../Library/Driver/PDM2PCM.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <inttypes.h>

PDM2PCM::PDM2PCM() :
    baseAddress( (uint32_t *) PDM2PCM_BASE ),

    /* Initialize register addresses based on the base address */
    status         ( (struct statusRegister_s *) (baseAddress)      ),
    control        ( (struct controlRegister_s *) (baseAddress + 1) ),
    gain           ( (uint16_t *) (baseAddress + 2)                 ),
    decimationRate ( (uint8_t *) (baseAddress + 3)                  ),
    sampleBuffer   ( (uint16_t *) (baseAddress + 4)                 ),
    threshold      ( (uint16_t *) (baseAddress + 5)                 ),
    event          ( (uint8_t *) (baseAddress + 6)                  ) {
    
    control->interruptEnable = 0;
};

PDM2PCM::~PDM2PCM() {}

/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

PDM2PCM& PDM2PCM::init(channel_e channel, bool dualChannel, uint32_t frequency, uint32_t sampleRate, error_e* error) {
    setChannel(channel);
    setDualChannel(dualChannel);

    if (frequency > 3'000'000 || frequency < 1'000'000) {
        *error = ILLEGAL_CLOCK;

        return *this;
    }

    /* Find the clock divisor value */
    setClockDivisor(SYSTEM_FREQUENCY / frequency, nullptr);

    /* Find the decimation rate */
    uint32_t decimationRate = frequency / sampleRate;

    if (decimationRate > UINT8_MAX) {
        *error = ILLEGAL_DECIMATION_RATE;

        return *this;
    }

    setDecimationRate(decimationRate);

    return *this;
}

PDM2PCM& PDM2PCM::setGain(uint16_t gain, error_e* error) {
    if (gain > 0x8000) {
        *error = ILLEGAL_GAIN;

        return *this;
    }

    *PDM2PCM::gain = gain;

    return *this;
}

PDM2PCM& PDM2PCM::setDecimationRate(uint8_t decimationRate) {
    *PDM2PCM::decimationRate = decimationRate;

    return *this;
}

PDM2PCM& PDM2PCM::setThreshold(uint16_t threshold) {
    *PDM2PCM::threshold = threshold;

    return *this;
}

PDM2PCM& PDM2PCM::setChannel(channel_e channel) {
    PDM2PCM::control->channel = channel;

    return *this;
}

PDM2PCM& PDM2PCM::setDualChannel(bool dualChannel) {
    PDM2PCM::control->dualChannel = dualChannel;

    return *this;
}

PDM2PCM& PDM2PCM::enableInterface(bool enable) {
    PDM2PCM::control->interfaceEnable = enable;

    return *this;
}

PDM2PCM& PDM2PCM::enableBuffer(bool enable) {
    PDM2PCM::control->bufferEnable = enable;

    return *this;
}

PDM2PCM& PDM2PCM::setClockDivisor(uint8_t value, error_e* error) {
    uint32_t interfaceFrequency = SYSTEM_FREQUENCY / value;

    /* Check value validity */
    if (interfaceFrequency > 3'000'000 || interfaceFrequency < 1'000'000) {
        *error = ILLEGAL_CLOCK;

        return *this;
    }

    PDM2PCM::control->clockDivisor = value;

    return *this;
}

bool PDM2PCM::isFull() {
    return PDM2PCM::status->bufferFull;
}

bool PDM2PCM::isEmpty() {
    return PDM2PCM::status->bufferEmpty;
}

/*****************************************************************/
/*                         SAMPLES READ                          */
/*****************************************************************/

PDM2PCM& PDM2PCM::startRecording() {
    PDM2PCM::control->interfaceEnable = true;
    PDM2PCM::control->bufferEnable = true;
};

PDM2PCM& PDM2PCM::stopRecording() {
    PDM2PCM::control->interfaceEnable = false;
    PDM2PCM::control->bufferEnable = false;
};

uint16_t PDM2PCM::readSample() {
    /* Read sample buffer 1 time */
    return *PDM2PCM::sampleBuffer;
};

uint32_t PDM2PCM::readAudioStream(uint16_t* buffer, uint32_t size, error_e* error) {
    if (size == 0 || !buffer) {
        *error = ILLEGAL_ARGUMENTS;
    }

    /* Index of the buffer */
    uint32_t samplesRead = 0;

    while (samplesRead < size && !isEmpty()) {
        /* Read until the end of the array or the sample buffer become empty */
        buffer[samplesRead++] = *PDM2PCM::sampleBuffer;
    }

    /* Return the last index */
    return samplesRead;
};

#endif