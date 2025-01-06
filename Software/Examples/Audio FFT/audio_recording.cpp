#include "settings.h"

#include "../../Library/Driver/PDM2PCM.h"
#include "../../Library/Driver/Ethernet.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Serial_IO.h"

void recordAudio(uint8_t *audioSamples, uint32_t bufferSize, uint32_t recordTime, Timer &timer, PDM2PCM &microphone);
bool initMicrophone(Timer &timer, PDM2PCM &microphone);


extern "C" void audio_recording() {
    Serial_IO::init();

    Serial_IO::write("Start\n");

    /* Ethernet driver to send audio samples */
    Ethernet ethernet;
    ethernet.init(Ethernet::MBPS100, Ethernet::FULL_DUPLEX, false, Ethernet::IEEE_8023);

    /* Microphone that converts PDM stream into PCM samples */
    PDM2PCM microphone;

    /* Timer to keep track of the amount of time the microphone is recording */
    Timer timer(0);
    
    Serial_IO::write("Init mic\n");

    if (!initMicrophone(timer, microphone)) {
        return;
    }

    Serial_IO::write("Done\n");


    recordAudio(audioSamples, AUDIO_BUFFER_SIZE, 5, timer, microphone);

    Serial_IO::write("Sending audio samples...\n");


    struct Ethernet::macAddr_s macDestination;

    timer.delay(1000);
    Serial_IO::write("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::write("[LINK STATUS] Enstablished!\n\n");

    // for (int i = 0; i < numberOfSamples; ++i) {
    //     Serial_IO::printf("%x\n\r", audioSamples[i]);
    // }

    macDestination.byte[0] = 0xBB;
    macDestination.byte[1] = 0xE0;
    macDestination.byte[2] = 0x3A;
    macDestination.byte[3] = 0xC3;
    macDestination.byte[4] = 0x8F;
    macDestination.byte[5] = 0x08;

    /* Calculate the total number of 1.5kB chunks in the audio buffer (consi) */
    ethernet.sendFrame(audioSamples, 1400, macDestination, nullptr);

    while (ethernet.isSending()) {  }

    if (!ethernet.isEmptyPayloadTX()) {
        Serial_IO::write("Payload not empty");
    }

    if (!ethernet.isEmptyPacketTX()) {
        Serial_IO::write("Packet not empty");
    }

    while(1) {  }

    return;
};

bool initMicrophone(Timer &timer, PDM2PCM &microphone) {
    PDM2PCM::error_e error = PDM2PCM::NO_ERROR;

    microphone.init(PDM2PCM::LEFT, false, PDM_CLK_FREQUENCY, SAMPLING_FREQUENCY, &error);

    if (error == PDM2PCM::ILLEGAL_DECIMATION_RATE) {
        Serial_IO::write("[ERROR] Illegal decimation rate!\n");

        return false;
    } else if (error == PDM2PCM::ILLEGAL_CLOCK) {
        Serial_IO::write("[ERROR] Illegal clock!\n");

        return false;
    }


    timer.init(-1, Timer::ONE_SHOT);

    return true;
};


void recordAudio(uint8_t *audioSamples, uint32_t bufferSize, uint32_t recordTime, Timer &timer, PDM2PCM &microphone) {
    PDM2PCM::error_e error = PDM2PCM::NO_ERROR;

    microphone.startRecording();

    /* Set amount of counting and reset timer */
    timer.setThreshold(10'000 * recordTime);
    timer.setTime(0);

    /* Start timer (non blocking function) */
    timer.start();

    Serial_IO::write("Entering\n");


    uint32_t samplesRead = 0;
    uint32_t index = 0;

    while (!timer.isHalted()) {
        if (!microphone.isEmpty()) {
            uint16_t sample = microphone.readSample();

            audioSamples[index] = sample & 0x00FF;
            audioSamples[index + 1] = (sample & 0xFF00) >> 8;

            Serial_IO::printf("%xb%xb\n", audioSamples[index + 1], audioSamples[index]);
            
            index += 2;
            ++samplesRead;
        }
    }

    Serial_IO::printf("Done\n", samplesRead);

    microphone.stopRecording();

    Serial_IO::printf("Done reading %d samples\n", samplesRead);
};