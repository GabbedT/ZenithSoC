#ifndef AudioCapture_H
#define AudioCapture_H

#include <inttypes.h>

#include "../mmio.h"

class AudioCapture {

public: 

    enum error_e { NO_ERROR,
                   ILLEGAL_CLOCK, 
                   ILLEGAL_GAIN, 
                   ILLEGAL_DIVISOR, 
                   ILLEGAL_DECIMATION_RATE,
                   ILLEGAL_ARGUMENTS };

    enum channel_e { LEFT, RIGHT };

    struct controlRegister_s {
        /* Divide main clock to obtain the output clock frequency desired */
        unsigned int clockDivisor : 7;

        /* Enable or disable certain interrupt event */
        unsigned int interruptEnable : 6;
        
        /* Channel configuration */
        unsigned int channel : 1;
        unsigned int dualChannel : 1;

        /* Save samples into buffer */
        unsigned int bufferEnable : 1;

        /* Enable interface and clock generation */
        unsigned int interfaceEnable : 1;
    };

    struct statusRegister_s {
        unsigned int bufferFull : 1;
        unsigned int bufferEmpty : 1;

        /* Pad to 8 bits */
        unsigned int padding : 6;
    };

    enum event_e {
        /* Sample buffer become full */
        BUFFER_FULL = 1 << 0,

        /* Left channel surpassed the threshold (low or high)*/
        LEFT_THRESHOLD_HIGH = 1 << 1,
        LEFT_THRESHOLD_LOW  = 1 << 2,

        /* Right channel surpassed the threshold (low or high)*/
        RIGHT_THRESHOLD_HIGH = 1 << 3,
        RIGHT_THRESHOLD_LOW  = 1 << 4,

        /* Configuration error */
        ERROR  = 1 << 5
    };

private: 

    /* Base MMIO address */
    uint32_t* const baseAddress;

    /* Status register address */
    volatile struct statusRegister_s* const status;

    /* Control register address */
    volatile struct controlRegister_s* const control;

    /* Gain register */
    volatile uint16_t* const gain;

    /* Decimation register */
    volatile uint8_t* const decimationRate;

    /* Normalization value */
    volatile uint32_t* const normalizer;

    /* Sample buffer */
    volatile uint16_t* const sampleBuffer;

    /* Threshold register */
    volatile uint16_t* const threshold;

    /* Event register */
    volatile uint8_t* const event;

public: 

    AudioCapture();

    /* Deconstructor */
    ~AudioCapture();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    AudioCapture& init(channel_e channel, bool dualChannel, uint32_t frequency, uint32_t sampleRate, error_e* error);

    AudioCapture& setGain(uint16_t gain, error_e* error);

    AudioCapture& setDecimationRate(uint8_t decimationRate);

    AudioCapture& setThreshold(uint16_t threshold);

    AudioCapture& setChannel(channel_e channel);

    AudioCapture& setDualChannel(bool dualChannel);

    AudioCapture& enableInterface(bool enable);

    AudioCapture& enableBuffer(bool enable);

    AudioCapture& setFrequency(uint32_t value, error_e* error);

    inline bool isFull() {
        return AudioCapture::status->bufferFull;
    };

    inline bool isEmpty() {
        return AudioCapture::status->bufferEmpty;
    };


/*****************************************************************/
/*                           SAMPLES                             */
/*****************************************************************/

    AudioCapture& startRecording();

    AudioCapture& stopRecording();

    uint16_t readSample();

    uint32_t readAudioStream(uint16_t* buffer, uint32_t size, error_e* error); 

};

#endif