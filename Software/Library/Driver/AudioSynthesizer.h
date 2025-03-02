#ifndef AUDIO_SYNTHESIZER_H
#define AUDIO_SYNTHESIZER_H

#include <inttypes.h>

class AudioSynthesizer {

public:

    typedef enum {ILLEGAL_DUTY_CYCLE, ILLEGAL_TIME, ILLEGAL_FREQUENCY, ILLEGAL_INDEX} error_e;

    struct controlRegister_s {
        /* Restart wave */
        unsigned int waveStart : 1;

        /* Enable wave output */
        unsigned int waveEnable : 1;
        
        /* Restart ADSR modulation */
        unsigned int adsrStart : 1;

        /* Enable ADSR modulation, gain is now independent */
        unsigned int adsrEnable : 1;

        /* ADSR FSM is idle */
        unsigned int adsrIdle : 1;

        /* Gain without ADSR modulation */
        unsigned int gain : 16;

        unsigned int padding : 11;
    };

    /* Type of wave */
    typedef enum {CUSTOM, SINE, TRIANGLE, SQUARE} wave_e;

private:

    /* Selected wave */
    uint32_t selectedWave;

    /* Base MMIO address */
    uint32_t* const baseAddress;

    /* Custom wave */
    volatile uint32_t* const customTable;
    volatile struct controlRegister_s* const customControl;
    volatile uint32_t* const customIncrement;
    volatile uint32_t* const customAttackStep;
    volatile uint32_t* const customDecayStep;
    volatile uint32_t* const customReleaseStep;
    volatile uint32_t* const customSustainTime;
    volatile uint32_t* const customAttackLevel;
    volatile uint32_t* const customSustainLevel;

    /* Sine wave */
    volatile struct controlRegister_s* const sineControl;
    volatile uint32_t* const sineIncrement;
    volatile uint32_t* const sineAttackStep;
    volatile uint32_t* const sineDecayStep;
    volatile uint32_t* const sineReleaseStep;
    volatile uint32_t* const sineSustainTime;
    volatile uint32_t* const sineAttackLevel;
    volatile uint32_t* const sineSustainLevel;

    /* Triangle wave */
    volatile struct controlRegister_s* const triangleControl;
    volatile uint32_t* const triangleIncrement;
    volatile uint32_t* const triangleAttackStep;
    volatile uint32_t* const triangleDecayStep;
    volatile uint32_t* const triangleReleaseStep;
    volatile uint32_t* const triangleSustainTime;
    volatile uint32_t* const triangleAttackLevel;
    volatile uint32_t* const triangleSustainLevel;

    /* Square wave */
    volatile struct controlRegister_s* const squareControl;
    volatile uint32_t* const squareIncrement;
    volatile uint32_t* const squareAttackStep;
    volatile uint32_t* const squareDecayStep;
    volatile uint32_t* const squareReleaseStep;
    volatile uint32_t* const squareSustainTime;
    volatile uint32_t* const squareAttackLevel;
    volatile uint32_t* const squareSustainLevel;
    volatile uint32_t* const squareDutyCycle;


public:

    /* Constructor */
    AudioSynthesizer();


//====================================================================================
//      WAVE CONTROL FUNCTIONS
//====================================================================================

    AudioSynthesizer& selectWave(wave_e type);

    /**
     * @brief Enable or disable a certain wave
     * 
     * @param type Type of wave 
     * @param enable Enable or Disable
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setWave(bool enable);

    /**
     * @brief Restart the wave phase register
     * 
     * @param type Type of wave 
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& restartWave();

    /**
     * @brief Set the wave frequency
     * 
     * @param type Type of wave 
     * @param frequency Frequency of the wave
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setFrequency(uint32_t frequency);

    /**
     * @brief Set the wave gain
     * 
     * @param type Type of wave 
     * @param gain Gain of the wave, a Q1.15 value (unsigned)
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setGain(uint16_t gain);

    /**
     * @brief Set the duty cycle of the square wave
     * 
     * @param dutyCycle A floating point value between 1.0 and 0.0
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setSquareDutyCycle(float dutyCycle, error_e *error);

    /**
     * @brief Write custom wave table, if the size of the table is
     * bigger than 1024, then only 1024 samples will be written
     * 
     * @param table An array of samples
     * @param size Size of the table
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setCustomWave(uint16_t *table, uint32_t size);

    /**
     * @brief Write a single sample in the custom wave table
     * 
     * @param table An array of samples
     * @param size Size of the table
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setCustomWave(uint16_t sample, uint32_t index, error_e *error);


//====================================================================================
//      ADSR CONTROL FUNCTIONS
//==================================================================================== 

    /**
     * @brief Enable or disable ADSR modulation
     * 
     * @param enable Boolean value to enable or disable ADSR modulation
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setADSR(bool enable);

    /**
     * @brief Set ADSR Modulation parameters to modulate 
     * the wave with attack, decay, sustain and release
     * modulation. It does not block until the modulation ends!
     * 
     * @param aTime Time to arrive to the attack level starting from 0 (in `ms`)
     * @param dTime Time to arrive to the sustain level starting from the attack level (in `ms`)
     * @param sTime Sustain time (in `ms`)
     * @param rTime Time to arrive to 0 gain from the sustain level (in `ms`)
     * @param aLevel Attack wave gain level, a `Q1.15` number.
     * @param sLevel Sustain wave gain level, a `Q1.15 number.
     * 
     * @return Pointer to the object to chain functions
     */
    AudioSynthesizer& setModulation(uint32_t aTime, uint32_t dTime, uint32_t sTime, uint32_t rTime, uint16_t aLevel, uint16_t sLevel);
    
    /**
     * @brief Check idle status
     * 
     * @return Boolean value rapresenting the idle status
     */
    bool isIdleADSR();

};

#endif 