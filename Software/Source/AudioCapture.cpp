#ifndef AudioCapture_CPP
#define AudioCapture_CPP

#include "../Library/Driver/AudioCapture.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <inttypes.h>

AudioCapture::AudioCapture() :
    baseAddress( (uint32_t *) ACU_BASE ),

    /* Initialize register addresses based on the base address */
    status         ( (struct statusRegister_s *) (baseAddress)      ),
    control        ( (struct controlRegister_s *) (baseAddress + 1) ),
    gain           ( (uint16_t *) (baseAddress + 2)                 ),
    decimationRate ( (uint8_t *) (baseAddress + 3)                  ),
    normalizer     ( (uint32_t *) (baseAddress + 4)                  ),
    sampleBuffer   ( (uint16_t *) (baseAddress + 5)                 ),
    threshold      ( (uint16_t *) (baseAddress + 6)                 ),
    event          ( (uint8_t *) (baseAddress + 7)                  ) {
    
    control->interruptEnable = 0;
};

AudioCapture::~AudioCapture() {};

/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

AudioCapture& AudioCapture::init(channel_e channel, bool dualChannel, uint32_t frequency, uint32_t sampleRate, error_e* error) {
    setChannel(channel);
    setDualChannel(dualChannel);

    if (frequency > 3'000'000 || frequency < 1'000'000) {
        *error = ILLEGAL_CLOCK;

        return *this;
    }

    /* Find the clock divisor value */
    setFrequency(frequency, nullptr);

    /* Find the decimation rate */
    uint32_t decimationRate = frequency / sampleRate;

    if (decimationRate > UINT8_MAX) {
        *error = ILLEGAL_DECIMATION_RATE;

        return *this;
    }

    setDecimationRate(decimationRate);

    AudioCapture::control->interfaceEnable = true;

    uint32_t normalizerFactor = 1;

    /* CIC filter will output a maximum sample value of DECIMATION_RATE ^ (FILTER_ORDER - 1) */
    for (int i = 0; i < 4; ++i) {
        normalizerFactor *= decimationRate;
    }

    *AudioCapture::normalizer = normalizerFactor;

    return *this;
};

AudioCapture& AudioCapture::setGain(uint16_t gain, error_e* error) {
    if (gain > 0x8000) {
        *error = ILLEGAL_GAIN;

        return *this;
    }

    *AudioCapture::gain = gain;

    return *this;
};

AudioCapture& AudioCapture::setDecimationRate(uint8_t decimationRate) {
    *AudioCapture::decimationRate = decimationRate;

    return *this;
};

AudioCapture& AudioCapture::setThreshold(uint16_t threshold) {
    *AudioCapture::threshold = threshold;

    return *this;
};

AudioCapture& AudioCapture::setChannel(channel_e channel) {
    AudioCapture::control->channel = channel;

    return *this;
};

AudioCapture& AudioCapture::setDualChannel(bool dualChannel) {
    AudioCapture::control->dualChannel = dualChannel;

    return *this;
};

AudioCapture& AudioCapture::enableInterface(bool enable) {
    AudioCapture::control->interfaceEnable = enable;

    return *this;
};

AudioCapture& AudioCapture::enableBuffer(bool enable) {
    AudioCapture::control->bufferEnable = enable;

    return *this;
};

AudioCapture& AudioCapture::setFrequency(uint32_t frequency, error_e* error) {
    /* Check value validity */
    if (frequency > 3'000'000 || frequency < 1'000'000) {
        *error = ILLEGAL_CLOCK;

        return *this;
    }

    AudioCapture::control->clockDivisor = SYSTEM_FREQUENCY / (frequency * 2);

    return *this;
};

/*****************************************************************/
/*                         SAMPLES READ                          */
/*****************************************************************/


AudioCapture& AudioCapture::startRecording() {
    AudioCapture::control->bufferEnable = true;

    return *this;
};

AudioCapture& AudioCapture::stopRecording() {
    AudioCapture::control->bufferEnable = false;

    return *this;
};

uint16_t AudioCapture::readSample() {
    /* Read sample buffer 1 time */
    return *AudioCapture::sampleBuffer;
};

uint32_t AudioCapture::readAudioStream(uint16_t* buffer, uint32_t size, error_e* error) {
    if (size == 0 || !buffer) {
        *error = ILLEGAL_ARGUMENTS;
    }

    /* Index of the buffer */
    uint32_t samplesRead = 0;

    while (samplesRead < size && !isEmpty()) {
        /* Read until the end of the array or the sample buffer become empty */
        buffer[samplesRead++] = *AudioCapture::sampleBuffer;
    }

    /* Return the last index */
    return samplesRead;
};

#endif