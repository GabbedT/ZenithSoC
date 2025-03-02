#include "../../Library/Driver/AudioSynthesizer.h"
#include "../../Library/Driver/PRNG.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Serial_IO.h"

void testWave(AudioSynthesizer::wave_e wave, AudioSynthesizer& synth, Timer& timer);

void nesCoin(AudioSynthesizer& synth, Timer& timer);
void nesJump(AudioSynthesizer& synth, Timer& timer);
void nesBass(AudioSynthesizer& synth, Timer& timer);
void nesExplosion(AudioSynthesizer& synth, Timer& timer);

extern "C" void audio_synthesizer() {
    Timer timer(0);
    Serial_IO::init();
    AudioSynthesizer synth;

    timer.init(-1, Timer::ONE_SHOT);

    Serial_IO::write("[TEST] Sine Wave\n");
    testWave(AudioSynthesizer::SINE, synth, timer);

    Serial_IO::write("[TEST] Triangle Wave\n");
    testWave(AudioSynthesizer::TRIANGLE, synth, timer);

    Serial_IO::write("[TEST] Square Wave\n");
    testWave(AudioSynthesizer::SQUARE, synth, timer);

    /* Sawtooth */
    for (int i = 0; i < 1024; ++i) {
        synth.setCustomWave((0x8000 / 1024) * i, i, nullptr);
    }

    Serial_IO::write("[TEST] Custom Wave\n");
    testWave(AudioSynthesizer::CUSTOM, synth, timer);
  
    for (uint32_t i = 0; i < 10; ++i) {
        Serial_IO::write("[TEST] Coin\n");
        nesCoin(synth, timer); timer.delay(1000);
    }

    for (uint32_t i = 0; i < 10; ++i) {
        Serial_IO::write("[TEST] Jump\n");
        nesJump(synth, timer); timer.delay(1000);
    }

    for (uint32_t i = 0; i < 10; ++i) {
        Serial_IO::write("[TEST] Bass\n");
        nesBass(synth, timer); timer.delay(1000);
    }
 
    for (uint32_t i = 0; i < 10; ++i) {
        Serial_IO::write("[TEST] Explosion\n");
        nesExplosion(synth, timer); timer.delay(1000);
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
            
            Serial_IO::printf("Duty Cycle: %.6f\n", dutyCycle);

            timer.delay(2);
        }
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


void nesExplosion(AudioSynthesizer &synth, Timer& timer) {
    const int ATTACK_TIME = 50;
    const int DECAY_TIME = 100;
    const int SUSTAIN_LEVEL = 1000;
    const int RELEASE_TIME = 200;
    const int PEAK_GAIN = 0x8000;
    const int SUSTAIN_GAIN = 0x6000;

    /* Noise */
    for (int i = 0; i < 1024; ++i) {
        synth.setCustomWave(PRNG::random() % 0x8000, i, nullptr);
    }

    synth.selectWave(AudioSynthesizer::CUSTOM);

    synth.setWave(true)
         .setGain(PEAK_GAIN)
         .setFrequency(1000);

    synth.setADSR(true).setModulation(ATTACK_TIME, DECAY_TIME, SUSTAIN_LEVEL, RELEASE_TIME, PEAK_GAIN, SUSTAIN_GAIN);
    while (!synth.isIdleADSR()) {  }
 
    synth.setADSR(false).setWave(false);
};