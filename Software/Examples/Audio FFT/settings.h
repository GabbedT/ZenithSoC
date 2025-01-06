#ifndef SETTINGS_H
#define SETTINGS_H

#include <inttypes.h>

/* Audio sampling frequency */
#define SAMPLING_FREQUENCY 48000  /* KHz */

/* PDM interface clock frequency */
#define PDM_CLK_FREQUENCY 3000000 /* Hz  */

/* Decimator value for CIC filter */
#define DECIMATOR_VALUE (PDM_CLK_FREQUENCY / SAMPLING_FREQUENCY)

/* To define buffer width */
#define MAX_RECORDING_SECOND 20

/* In a second there are 48000 PCM16 samples (96 KB of data), this value must not exceed
 * the DDR2 size of the platform */
#define AUDIO_BUFFER_SIZE (2 * SAMPLING_FREQUENCY * MAX_RECORDING_SECOND)


/* Change the pointer value to an unused DDR2 memory area */
uint8_t *audioSamples = (uint8_t *) 0x80002000;

/* Microphone recording time, choosen by user trough command */
volatile uint32_t recordingTime = 0;

/* Number of samples saved into the buffer */
volatile uint32_t numberOfSamples = 0;

/* Fixed point gain in Q1.15 format */
volatile uint16_t gain = 0x8000;

/* Commands */
volatile bool sendAudio_FFT = false;
volatile bool sendAudio = false;
volatile bool startProcessing = false;

#endif