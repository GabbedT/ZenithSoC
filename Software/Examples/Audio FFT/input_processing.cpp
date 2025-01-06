#include "../../Library/Serial_IO.h"

#include "settings.h"

#define MAX_COMMAND_LENGTH 25

/* Indexes */
#define SET_SAMPLING_TIME 0
#define GET_FFT 1
#define GET_SAMPLES 2
#define GET_STATUS 3
#define START_PROCESSING 4

#define MAX_TABLE_COMMAND_LENGTH 18

/* Command list, each sample is followed by 1 or 0 parameters */
char* commands[] {
    /* Set a recording time (in seconds) */
    "SET_SAMPLING_TIME",

    /* Send FFT samples to PC */
    "GET_FFT",

    /* Send Audio samples to PC */
    "GET_SAMPLES",

    /* Send processing status to PC */
    "GET_STATUS",

    /* Start FFT processing */
    "START_PROCESSING"
};

bool parseCommands(uint32_t &time);

/* Return the index to command table, if the command is malformed return -1 */
int32_t checkCommand(char* command);


bool parseCommands(uint32_t &time) {
    /* Initialize Serial_IO in the main function */
    Serial_IO::write("> ");

    char commandRead[MAX_COMMAND_LENGTH];

    /* Read command from user */
    Serial_IO::readString(commandRead, MAX_COMMAND_LENGTH);

    /* Variable is set to -1 if no space is found */
    int32_t spaceIndex = -1;

    /* Find space index */
    for (uint32_t i = 0; i < MAX_COMMAND_LENGTH; ++i) {
        if (commandRead[i] == ' ') {
            spaceIndex = i;

            break;
        }
    }

    /* Take the command table index */
    uint32_t commandIndex = checkCommand(commandRead);

    if (commandIndex == -1) {
        Serial_IO::write("[ERROR] Invalid Command!\n");

        return false;
    }


    switch (commandIndex) {
        case SET_SAMPLING_TIME: {
            for (int i = spaceIndex + 1; commandRead[i] != '\0'; ++i) {
                if (commandRead[i] >= '0' && commandRead[i] <= '9') {
                    /* Each iteration it shifts the most significant digit on the left, then
                     * it adds the new character as the least significand digit */
                    recordingTime = recordingTime * 10 + (commandRead[i] - '0');
                } else {
                    /* Error: illegal character */
                    return false;
                }

                Serial_IO::printf("[SET SAMPLING TIME] Time set to %d\n", recordingTime);

                if (recordingTime > MAX_RECORDING_SECOND) {
                    Serial_IO::write("[ERROR] Recording time exceeds the limit!\n");

                    return false;
                }
            }

            break;
        }

        case GET_FFT: {
            sendAudio_FFT = true;

            Serial_IO::printf("[GET FFT]\n");

            break;
        }

        case GET_SAMPLES: {
            sendAudio = true;

            Serial_IO::printf("[GET SAMPLES]\n");

            break;
        }

        case GET_STATUS: {
            /* PRINT STATUS */

            Serial_IO::printf("[GET STATUS]\n");

            break;
        }

        case START_PROCESSING: {
            startProcessing = true;

            Serial_IO::printf("[START PROCESSING]\n");

            break;
        }
        
        default:
        break;
    }

    return true;
};


int32_t checkCommand(char* command) {
    for (uint32_t i = 0; i < MAX_COMMAND_LENGTH; ++i) {
        uint32_t j = 0;

        /* Exit loop only if the commands are different or arrives at the end */
        while (commands[i][j] != '\0' && command[j] != '\0' && commands[i][j] == command[j]) {
            ++j;
        }

        if (commands[i][j] == '\0' && (command[j] != '\0' || command[j] != ' ')) {
            /* Return command index if the loop arrived to the end */
            return i;
        }
    }

    return -1;
};