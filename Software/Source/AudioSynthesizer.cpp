#ifndef AUDIO_SYNTHESIZER_CPP
#define AUDIO_SYNTHESIZER_CPP

#include "../Library/Driver/AudioSynthesizer.h"
#include "../Library/mmio.h"
#include "../Library/platform.h"

#include <inttypes.h>

//====================================================================================
//                         CONSTRUCTOR                          
//====================================================================================

AudioSynthesizer::AudioSynthesizer() : 
    /* Base MMIO address */
    baseAddress ( (uint32_t *) (APU_BASE + 32) ),

    /* Custom wave */
    customTable        ( (uint32_t *) baseAddress                  ),
    customControl      ( (controlRegister_s*) (customTable + 1024) ),
    customIncrement    ( (uint32_t *) (customControl + 1)          ),
    customAttackStep   ( (uint32_t *) (customControl + 2)          ),
    customDecayStep    ( (uint32_t *) (customControl + 3)          ),
    customReleaseStep  ( (uint32_t *) (customControl + 4)          ),
    customSustainTime  ( (uint32_t *) (customControl + 5)          ),
    customAttackLevel  ( (uint32_t *) (customControl + 6)          ),
    customSustainLevel ( (uint32_t *) (customControl + 7)          ),

    /* Sine wave */
    sineControl      ( (controlRegister_s*) (customSustainLevel + 1) ),
    sineIncrement    ( (uint32_t *) (customSustainLevel + 2)         ),
    sineAttackStep   ( (uint32_t *) (customSustainLevel + 3)         ),
    sineDecayStep    ( (uint32_t *) (customSustainLevel + 4)         ),
    sineReleaseStep  ( (uint32_t *) (customSustainLevel + 5)         ),
    sineSustainTime  ( (uint32_t *) (customSustainLevel + 6)         ),
    sineAttackLevel  ( (uint32_t *) (customSustainLevel + 7)         ),
    sineSustainLevel ( (uint32_t *) (customSustainLevel + 8)         ),

    /* Triangle wave */
    triangleControl      ( (controlRegister_s*) (sineSustainLevel + 1) ),
    triangleIncrement    ( (uint32_t *) (sineSustainLevel + 2)         ),
    triangleAttackStep   ( (uint32_t *) (sineSustainLevel + 3)         ),
    triangleDecayStep    ( (uint32_t *) (sineSustainLevel + 4)         ),
    triangleReleaseStep  ( (uint32_t *) (sineSustainLevel + 5)         ),
    triangleSustainTime  ( (uint32_t *) (sineSustainLevel + 6)         ),
    triangleAttackLevel  ( (uint32_t *) (sineSustainLevel + 7)         ),
    triangleSustainLevel ( (uint32_t *) (sineSustainLevel + 8)         ),

    /* Square wave */
    squareControl      ( (controlRegister_s*) (triangleSustainLevel + 1) ),
    squareIncrement    ( (uint32_t *) (triangleSustainLevel + 2)         ),
    squareAttackStep   ( (uint32_t *) (triangleSustainLevel + 3)         ),
    squareDecayStep    ( (uint32_t *) (triangleSustainLevel + 4)         ),
    squareReleaseStep  ( (uint32_t *) (triangleSustainLevel + 5)         ),
    squareSustainTime  ( (uint32_t *) (triangleSustainLevel + 6)         ),
    squareAttackLevel  ( (uint32_t *) (triangleSustainLevel + 7)         ),
    squareSustainLevel ( (uint32_t *) (triangleSustainLevel + 8)         ),
    squareDutyCycle    ( (uint32_t *) (triangleSustainLevel + 9)         )
{
    customControl->adsrEnable = false;
    customControl->waveEnable = false;

    sineControl->adsrEnable = false;
    sineControl->waveEnable = false;

    triangleControl->adsrEnable = false;
    triangleControl->waveEnable = false;

    squareControl->adsrEnable = false;
    squareControl->waveEnable = false;

    selectedWave = SINE;
};

//====================================================================================
//                       WAVE CONTROL FUNCTIONS                 
//====================================================================================

AudioSynthesizer& AudioSynthesizer::selectWave(wave_e type) {
    selectedWave = type;

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setWave(bool enable) {
    switch (selectedWave) {
        case CUSTOM: {
            customControl->waveEnable = enable;
            break;
        }

        case SINE: {
            sineControl->waveEnable = enable;
            break;
        }

        case TRIANGLE: {
            triangleControl->waveEnable = enable;
            break;
        }

        case SQUARE: {
            squareControl->waveEnable = enable;
            break;
        }
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::restartWave() {
    switch (selectedWave) {
        case CUSTOM: {
            customControl->waveStart = true;
            break;
        }

        case SINE: {
            sineControl->waveStart = true;
            break;
        }

        case TRIANGLE: {
            triangleControl->waveStart = true;
            break;
        }

        case SQUARE: {
            squareControl->waveStart = true;
            break;
        }
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setFrequency(uint32_t frequency) {
    switch (selectedWave) {
        case CUSTOM: {
            /* The phase register needs to arrive to max to complete a cycle */
            uint64_t tmp = (uint64_t) UINT32_MAX * frequency;

            *customIncrement = tmp / SYSTEM_FREQUENCY;
            break;
        }

        case SINE: {
            /* The phase register needs to arrive to max to complete a cycle 
             * quarter cycle, that is because the table holds only a quarter
             * of a sine cycle */
            uint64_t tmp = (uint64_t) UINT32_MAX * frequency * 4;

            *sineIncrement = tmp / SYSTEM_FREQUENCY;
            break;
        }

        case TRIANGLE: {
            /* The phase register needs to arrive to and decrease
             * to minimum, so two times */
            uint64_t tmp = (uint64_t) UINT32_MAX * frequency * 2;

            *triangleIncrement = tmp / SYSTEM_FREQUENCY;
            break;
        }

        case SQUARE: {
            /* The phase register needs to arrive to max to complete a cycle */
            uint64_t tmp = (uint64_t) UINT32_MAX * frequency;

            *squareIncrement = tmp / SYSTEM_FREQUENCY;
            break;
        }
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setGain(uint16_t gain) {
    switch (selectedWave) {
        case CUSTOM: {
            customControl->gain = gain;
            break;
        }

        case SINE: {
            sineControl->gain = gain;
            break;
        }

        case TRIANGLE: {
            triangleControl->gain = gain;
            break;
        }

        case SQUARE: {
            squareControl->gain = gain;
            break;
        }
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setSquareDutyCycle(float dutyCycle, error_e *error) {
    if (dutyCycle > 1.0 && dutyCycle < 0.0) {
        *error = ILLEGAL_DUTY_CYCLE;
    } else {
        *squareDutyCycle = (uint32_t) (dutyCycle * UINT32_MAX);
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setCustomWave(uint16_t *table, uint32_t size) {
    uint32_t newSize = size;

    /* Custom table size is 1024 */
    if (size > 1024) {
        newSize = 1024;
    }

    for (int i = 0; i < newSize; ++i) {
        customTable[i] = table[i];
    }

    return *this;
};

AudioSynthesizer& AudioSynthesizer::setCustomWave(uint16_t sample, uint32_t index, error_e *error) {
    /* Custom table size is 1024 */
    if (index > 1024) {
        *error = ILLEGAL_INDEX;
    } else {
        customTable[index] = sample;
    }

    return *this;
};


//====================================================================================
//                       ADSR CONTROL FUNCTIONS                
//====================================================================================

AudioSynthesizer& AudioSynthesizer::setADSR(bool enable) {
    switch (selectedWave) {
        case CUSTOM: {
            customControl->adsrEnable = enable;
            break;
        }

        case SINE: {
            sineControl->adsrEnable = enable;
            break;
        }

        case TRIANGLE: {
            triangleControl->adsrEnable = enable;
            break;
        }

        case SQUARE: {
            squareControl->adsrEnable = enable;
            break;
        }
    }

    return *this;
};

bool AudioSynthesizer::isIdleADSR() {
    switch (selectedWave) {
        case CUSTOM: return customControl->adsrIdle;

        case SINE: return sineControl->adsrIdle;

        case TRIANGLE: return triangleControl->adsrIdle;

        case SQUARE: return squareControl->adsrIdle;
    }

    return false;
};

AudioSynthesizer& AudioSynthesizer::setModulation(uint32_t aTime, uint32_t dTime, uint32_t sTime, uint32_t rTime, uint16_t aLevel, uint16_t sLevel) {
    /* Number of clock cycles for a single millisecond */
    const uint32_t millis = SYSTEM_FREQUENCY / 1000;

    uint32_t attackLevel = aLevel << 16;
    uint32_t sustainLevel = sLevel << 16;

    /* Frequency increment */
    uint32_t attackStep = attackLevel / (millis * aTime);
    uint32_t decayStep = (attackLevel - sustainLevel) / (millis * dTime);
    uint32_t releaseStep = sustainLevel / (millis * rTime);

    /* Sustain time */
    uint32_t sustainDuration = sTime * millis;
    
    switch (selectedWave) {
        case CUSTOM: {
            *customAttackStep = attackStep;
            *customDecayStep = decayStep;
            *customReleaseStep = releaseStep;

            *customSustainTime = sustainDuration;

            *customAttackLevel = attackLevel;
            *customSustainLevel = sustainLevel;

            customControl->adsrStart = true;

            break;
        }

        case SINE: {
            *sineAttackStep = attackStep;
            *sineDecayStep = decayStep;
            *sineReleaseStep = releaseStep;

            *sineSustainTime = sustainDuration;

            *sineAttackLevel = attackLevel;
            *sineSustainLevel = sustainLevel;

            sineControl->adsrStart = true;

            break;
        }

        case TRIANGLE: {
            *triangleAttackStep = attackStep;
            *triangleDecayStep = decayStep;
            *triangleReleaseStep = releaseStep;

            *triangleSustainTime = sustainDuration;

            *triangleAttackLevel = attackLevel;
            *triangleSustainLevel = sustainLevel;

            triangleControl->adsrStart = true;

            break;
        }

        case SQUARE: {
            *squareAttackStep = attackStep;
            *squareDecayStep = decayStep;
            *squareReleaseStep = releaseStep;

            *squareSustainTime = sustainDuration;

            *squareAttackLevel = attackLevel;
            *squareSustainLevel = sustainLevel;

            squareControl->adsrStart = true;

            break;
        }
    }

    return *this;
};

#endif 
