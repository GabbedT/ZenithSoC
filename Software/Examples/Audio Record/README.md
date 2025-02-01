# Audio

This program tests the audio input module and its ability to convert a microphone's PDM data stream into 16-bit unsigned PCM samples. The audio is sampled at a rate of 48 kHz, and the resulting samples are stored in an array.

During boot, the ROM program is copied to the main memory (off-chip DDR) to verify the capabilities of the memory system. Afterwards, the main program initializes the Serial_IO, Ethernet, and the PDM2PCM converter.

Recording starts immediately and captures approximately 10 seconds of audio. Once the recording is complete, the program sends multiple Ethernet packets (each 1500 bytes) to the computer.

To retrieve these packets, execute the Python program `packet_processing.py`. This script extracts the payload from the packets, converts the data into a .wav file, and saves the resulting audio waveform.

# Compilation

To compile, ensure that the script `compile.sh` can be executed, then in the terminal: `./compile.sh`. Change the file paths in `ZenithSoC.sv` in the boot memory (the final output which is boot + program compiled is found in output.hex). If you're using `basic_system.sv` then change the file paths from the main memory (your_abs_path/program.hex) and boot memory (your_abs_path/boot.hex).