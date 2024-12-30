#ifndef PDM2PCM_H
#define PDM2PCM_H

#include <inttypes.h>

#include "../mmio.h"

class PDM2PCM {

public: 

    enum error_e { ILLEGAL_CLOCK, 
                   ILLEGAL_GAIN, 
                   ILLEGAL_DIVISOR, 
                   ILLEGAL_DECIMATION_RATE,
                   ILLEGAL_ARGUMENTS };

    enum channel_e { LEFT, RIGHT };

    struct controlRegister_s {
        /* Divide main clock to obtain the output clock frequency desired */
        unsigned int clockDivisor : 7;

        /* Enable or disable certain interrupt event */
        unsigned int interruptEnable : 5;
        
        /* Channel configuration */
        unsigned int channel : 1;
        unsigned int dualChannel : 1;

        /* Save samples into buffer */
        unsigned int bufferEnable : 1;

        /* Enable interface and clock generation */
        unsigned int interfaceEnable : 1;
    };

    struct statusRegister_s {
        unsigned int bufferEmpty : 1;
        unsigned int bufferFull : 1;

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

    /* Sample buffer */
    volatile uint16_t* const sampleBuffer;

    /* Threshold register */
    volatile uint16_t* const threshold;

    /* Event register */
    volatile uint8_t* const event;

public: 

    PDM2PCM();

    /* Deconstructor */
    ~PDM2PCM();


/*****************************************************************/
/*                         CONFIGURATION                         */
/*****************************************************************/

    PDM2PCM& init(channel_e channel, bool dualChannel, uint32_t frequency, uint32_t sampleRate, error_e* error);

    PDM2PCM& setGain(uint16_t gain, error_e* error);

    PDM2PCM& setDecimationRate(uint8_t decimationRate);

    PDM2PCM& setThreshold(uint16_t threshold);

    PDM2PCM& setChannel(channel_e channel);

    PDM2PCM& setDualChannel(bool dualChannel);

    PDM2PCM& enableInterface(bool enable);

    PDM2PCM& enableBuffer(bool enable);

    PDM2PCM& setClockDivisor(uint8_t value, error_e* error);

    inline bool isFull();

    inline bool isEmpty();


/*****************************************************************/
/*                           SAMPLES                             */
/*****************************************************************/

    PDM2PCM& startRecording();

    PDM2PCM& stopRecording();

    uint16_t readSample();

    uint32_t readAudioStream(uint16_t* buffer, uint32_t size, error_e* error); 

};

#endif