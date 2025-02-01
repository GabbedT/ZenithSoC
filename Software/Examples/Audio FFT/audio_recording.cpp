#include "settings.h"

#include "../../Library/Driver/PDM2PCM.h"
#include "../../Library/Driver/Ethernet.h"
#include "../../Library/Driver/Timer.h"
#include "../../Library/Serial_IO.h"

uint32_t recordAudio(uint8_t *audioSamples, uint32_t bufferSize, uint32_t recordTime, Timer &timer, PDM2PCM &microphone);
bool initMicrophone(Timer &timer, PDM2PCM &microphone);
void getRegister(uint8_t addr, const char *name, Ethernet& ethernet);

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
    
    uint32_t samplesRead = recordAudio(audioSamples, AUDIO_BUFFER_SIZE, 10000, timer, microphone);

    Serial_IO::write("Sending audio samples...\n");


    struct Ethernet::macAddr_s macDestination;

    timer.delay(1000);
    Serial_IO::write("\n\n[LINK STATUS] Linking...\n");
    ethernet.waitLink();
    Serial_IO::write("[LINK STATUS] Enstablished!\n\n");
    timer.delay(1000);


    /* Print all PHY registers */
    getRegister(Ethernet::_BasicControl_, "\n[0] BASIC CONTROL REGISTER: ", ethernet);
    getRegister(Ethernet::_BasicStatus_, "\n[1] BASIC STATUS REGISTER: ", ethernet);
    getRegister(Ethernet::_PHYIdentifier1_, "\n[2] PHY IDENTIFIER 1 REGISTER: ", ethernet);
    getRegister(Ethernet::_PHYIdentifier2_, "\n[3] PHY IDENTIFIER 2 REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegAdvertisement_, "\n[4] AUTO NEGOTIATION ADVERTISEMENT REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegLinkAbility_, "\n[5] AUTO NEGOTIATION LINK PARTNER REGISTER: ", ethernet);
    getRegister(Ethernet::_AutoNegExpansion_, "\n[6] AUTO NEGOTIATION EXPANSION REGISTER: ", ethernet);
    getRegister(Ethernet::_ModeCtrlStatus_, "\n[17] MODE CONTROL/STATUS REGISTER: ", ethernet);
    getRegister(Ethernet::_SpecialModes_, "\n[18] SPECIAL MODES REGISTER: ", ethernet);
    getRegister(Ethernet::_SymbolErrorCnt_, "\n[26] SYMBOL ERROR COUNTER REGISTER: ", ethernet);
    getRegister(Ethernet::_CtrlStatusIndication_, "\n[27] SPECIAL CONTROL/STATUS INDICATIONS REGISTER: ", ethernet);
    getRegister(Ethernet::_InterruptSource_, "\n[29] INTERRUPT SOURCE REGISTER: ", ethernet);
    getRegister(Ethernet::_InterruptMask_, "\n[30] INTERRUPT MASK REGISTER: ", ethernet);
    getRegister(Ethernet::_SpecialCtrlStatus_, "\n[31] PHY SPECIAL CONTROL/STATUS REGISTER: ", ethernet);

    macDestination.byte[0] = 0xBB;
    macDestination.byte[1] = 0xE0;
    macDestination.byte[2] = 0x3A;
    macDestination.byte[3] = 0xC3;
    macDestination.byte[4] = 0x8F;
    macDestination.byte[5] = 0x08;

    uint32_t bytesSent = 0;

    /* Calculate the total number of 1.5kB chunks in the audio buffer (consi) */
    while(bytesSent != (samplesRead * 2)) { 
        uint32_t remainingBytes = (samplesRead * 2) - bytesSent;

        if (remainingBytes > ethernet.MAX_PAYLOAD_LENGTH) {
            ethernet.sendFrame(&audioSamples[bytesSent], ethernet.MAX_PAYLOAD_LENGTH, macDestination, nullptr);

            bytesSent += ethernet.MAX_PAYLOAD_LENGTH;
        } else {
            ethernet.sendFrame(&audioSamples[bytesSent], remainingBytes, macDestination, nullptr);

            bytesSent = samplesRead * 2;
        }
    }


    return;
};

void getRegister(uint8_t addr, const char *name, Ethernet& ethernet) {
    uint16_t reg = ethernet.readPHYRegister(addr);

    Serial_IO::printf(name);
    Serial_IO::writeB(reg);
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


uint32_t recordAudio(uint8_t *audioSamples, uint32_t bufferSize, uint32_t recordTime, Timer &timer, PDM2PCM &microphone) {
    PDM2PCM::error_e error = PDM2PCM::NO_ERROR;

    Serial_IO::write("Entering\n");

    microphone.startRecording();

    /* Set amount of counting and reset timer */
    timer.setThreshold(100'000 * recordTime);
    timer.setTime(0);

    /* Start timer (non blocking function) */
    timer.start();

    Serial_IO::write("Sampling\n");


    uint32_t samplesRead = 0;
    uint32_t strange = 0;
    uint32_t index = 0;

    while (!timer.isHalted()) {
        if (!microphone.isEmpty()) {
            volatile int16_t sample = microphone.readSample() - 32768;

            if (sample > 31000 || sample < -31000) {
                ++strange;
            }

            audioSamples[index] = (sample & 0xFF00) >> 8;
            audioSamples[index + 1] = sample & 0x00FF;

            index += 2;
            ++samplesRead;
        }
    }

    Serial_IO::printf("Done\n", samplesRead);

    microphone.stopRecording();

    Serial_IO::printf("Done reading %d samples\n", samplesRead);
    Serial_IO::printf("Sample sus %d samples\n", strange);

    return samplesRead;
};