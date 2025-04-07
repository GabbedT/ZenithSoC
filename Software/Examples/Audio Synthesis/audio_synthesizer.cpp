#include "../../Library/Driver/AudioSynthesizer.h"
#include "../../Library/Driver/AudioCapture.h"
#include "../../Library/Driver/PRNG.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Serial_IO.h"

/* Audio record parameters */
#define PDM_CLK_FREQUENCY 3000000
#define SAMPLING_FREQUENCY 48000

void testWave(AudioSynthesizer::wave_e wave, AudioSynthesizer& synth, Timer& timer);

/* NES audio test */
void nesCoin(AudioSynthesizer& synth, Timer& timer);
void nesJump(AudioSynthesizer& synth, Timer& timer);
void nesBass(AudioSynthesizer& synth, Timer& timer);
void nesLaser(AudioSynthesizer& synth, Timer& timer);
void nesPowerUp(AudioSynthesizer& synth, Timer& timer);
void nesGameOver(AudioSynthesizer& synth, Timer& timer);

extern "C" void audio_synthesizer() {
    Timer timer(0);
    Serial_IO::init();
    AudioSynthesizer synth;

    timer.init(-1, Timer::ONE_SHOT);

    // Serial_IO::write("[TEST] Sine Wave\n");
    // testWave(AudioSynthesizer::SINE, synth, timer);

    // Serial_IO::write("[TEST] Triangle Wave\n");
    // testWave(AudioSynthesizer::TRIANGLE, synth, timer);

    // Serial_IO::write("[TEST] Square Wave\n");
    // testWave(AudioSynthesizer::SQUARE, synth, timer);

    // /* Sawtooth */
    // for (int i = 0; i < 1024; ++i) {
    //     synth.setCustomWave((0x8000 / 1024) * i, i, nullptr);
    // }

    // Serial_IO::write("[TEST] Custom Wave\n");
    // testWave(AudioSynthesizer::CUSTOM, synth, timer);
  
    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Coin\n");
    //     nesCoin(synth, timer); timer.delay(1000);
    // }

    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Jump\n");
    //     nesJump(synth, timer); timer.delay(1000);
    // }

    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Bass\n");
    //     nesBass(synth, timer); timer.delay(1000);
    // }
 
    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Laser\n");
    //     nesLaser(synth, timer); timer.delay(1000);
    // }

    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Power Up\n");
    //     nesPowerUp(synth, timer); timer.delay(1000);
    // }
 
    // for (uint32_t i = 0; i < 5; ++i) {
    //     Serial_IO::write("[TEST] Game Over\n");
    //     nesGameOver(synth, timer); timer.delay(1000);
    // }

    /* Test microphone and replay the recorded audio */
    AudioCapture mic; mic.init(AudioCapture::LEFT, false, PDM_CLK_FREQUENCY, SAMPLING_FREQUENCY, nullptr);
    Serial_IO::write("[TEST] Recording 10 seconds...\n");

    /* Set timer threshold and reset timer */
    timer.setThreshold(100'000 * 10'000);
    timer.setTime(0);

    /* Start timer (non blocking function) */
    timer.start();

    mic.startRecording();

    /* Save samples */
    uint16_t *audioSamples = (uint16_t *) 0x80004000;

    /* Sample index */
    int index = 0; 

    int strange = 0;

    while (!timer.isHalted()) {
        if (!mic.isEmpty()) {
            audioSamples[index] = mic.readSample();

            if (audioSamples[index] == 0 || audioSamples[index] == 0xFFFF) {
                ++strange;
            }

            ++index;
        }
    }

    Serial_IO::printf("[TEST] Strange samples: %d\n[TEST] Index: %d\n", strange, index);

    Serial_IO::write("[TEST] Playback!\n");

    
    /**
     * Custom wave generator has 2 buffers of 1024 samples each. This is to enable double buffering.
     * The frequency in this case is related to the entire buffer, not a single sample. That means
     * that if we simply write 48kHz in setFrequency, the buffer will be played at 48kHz. On the other hand
     * each sample is going to be read at 1024 * 48kHz. To overcome this we can divide the frequency by the number
     * of samples in the buffer (1024).
     */
    synth.selectWave(AudioSynthesizer::CUSTOM)
         .setFrequency(SAMPLING_FREQUENCY / 1024)
         .setGain(0x8000);

    /* Fill the double buffer first to avoid glitches */
    synth.setCustomWave(audioSamples, 1024, 0, nullptr);
    synth.setCustomWave(&audioSamples[1024], 1024, 1, nullptr);

    /* Start */
    synth.setWave(true).restartWave();

    /* To enable double buffering is important to save the active table and writing 
     * it when it cease to be active */
    volatile uint32_t activeTable = synth.getActiveTable();

    /* Write table and wait, it's a simple implementation with a loop, instead of 
     * reading the activeTable bit, it's possible to just start a timer with an interrupt,
     * the timer value will be the inverse of the frequency. */
    for (int i = 2048; i < index; i += 1024) {
        /* Wait until the next table is being read */
        while (synth.getActiveTable() == activeTable) {  }

        /* Write to the other table */
        if (synth.getActiveTable() == 0) {
            synth.setCustomWave(&audioSamples[i], 1024, 1, nullptr);
        } else {
            synth.setCustomWave(&audioSamples[i], 1024, 0, nullptr);
        }

        /* Switch table */
        activeTable = synth.getActiveTable();
    }
};


void testWave(AudioSynthesizer::wave_e wave, AudioSynthesizer& synth, Timer& timer) {
    synth.selectWave(wave);

    synth.setFrequency(440).setGain(0x8000);
    synth.setWave(true).restartWave();

    if (wave == AudioSynthesizer::SQUARE) {
        synth.setSquareDutyCycle(0.5f, nullptr);
    }

    /* Wait 4 seconds */
    timer.delay(4000);

    /* Try frequency modulation */
    Serial_IO::write("[TEST] Frequency modulation\n");

    for (int i = 0; i < 1000; ++i) {
        synth.setFrequency(440 + (i * 10));
        timer.delay(2);
    }

    synth.setFrequency(440);

    if (wave == AudioSynthesizer::SQUARE) {
        /* Duty cycle modulation */
        Serial_IO::write("[TEST] Duty Cycle modulation\n");

        for (uint32_t i = 0; i < 1000; ++i) {
            float dutyCycle = i / 1000.0f;
            synth.setSquareDutyCycle(dutyCycle, nullptr);
            
            Serial_IO::printf("[TEST] Duty Cycle: %.6f\n", dutyCycle);

            timer.delay(2);
        }

        synth.setSquareDutyCycle(0.5f, nullptr);
    }

    /* Try amplitude modulation */
    Serial_IO::write("[TEST] Amplitude modulation\n");

    for (int i = 0; i < 1000; ++i) {
        synth.setGain(0x8000 - (0x10 * i));
        timer.delay(2);
    }

    /* Try ADSR modulation */
    Serial_IO::write("[TEST] ADSR modulation\n");

    synth.setFrequency(440)
         .setGain(0x8000)
         .setADSR(true);

    for (int i = 0; i < 2; ++i) {
        synth.setModulation(1000, 100, 3000, 5000, 0x8000, 0x6000);

        while (!synth.isIdleADSR()) {  }
    }

    synth.setWave(false).setADSR(false);
};


void nesCoin(AudioSynthesizer& synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::SQUARE);

    synth.setWave(true)
         .setSquareDutyCycle(0.5f, nullptr)
         .setFrequency(800)
         .setGain(0x8000);

    /* Quick “pitch drop” effect by decreasing frequency in steps */
    for (int f = 800; f >= 400; f -= 50) {
        synth.setFrequency(f);
        timer.delay(20);
    }

    synth.setWave(false);
};


void nesJump(AudioSynthesizer &synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::SQUARE);

    synth.setWave(true)
         .setSquareDutyCycle(0.25f, nullptr)
         .setGain(0x8000);

    /* Rising sweep */
    for (int f = 200; f <= 600; f += 25) {
        synth.setFrequency(f);
        timer.delay(20);
    }

    synth.setWave(false);
};


void nesBass(AudioSynthesizer &synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::TRIANGLE);

    synth.setWave(true)
         .setFrequency(110)
         .setGain(0x8000);

    timer.delay(500);

    synth.setWave(false);
};


void nesLaser(AudioSynthesizer& synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::SQUARE);
    synth.setWave(true)
         .setSquareDutyCycle(0.125f, nullptr)
         .setGain(0x8000);

    for (int f = 600; f <= 1200; f += 50) {
        synth.setFrequency(f);
        timer.delay(15);
    }

    for (int f = 1200; f >= 600; f -= 50) {
        synth.setFrequency(f);
        timer.delay(15);
    }

    synth.setWave(false);
};


void nesPowerUp(AudioSynthesizer& synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::SQUARE);
    synth.setWave(true)
         .setSquareDutyCycle(0.33f, nullptr)
         .setGain(0x8000);

    for (int f = 300; f <= 900; f += 30) {
        synth.setFrequency(f);
        timer.delay(25);
    }

    synth.setWave(false);
};


void nesGameOver(AudioSynthesizer& synth, Timer& timer) {
    synth.selectWave(AudioSynthesizer::SQUARE);
    synth.setWave(true)
         .setSquareDutyCycle(0.5f, nullptr)
         .setGain(0x8000);

    for (int f = 800; f >= 200; f -= 20) {
        synth.setFrequency(f);
        timer.delay(30);
    }

    synth.setWave(false);
};