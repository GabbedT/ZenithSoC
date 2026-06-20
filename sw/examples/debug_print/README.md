# Debug Print Program

This program simply tests the print functions (and the UART module) of the static class Serial_IO used for debugging purpouses. 

# Compilation

To compile, ensure that the script `compile.sh` can be executed, then in the terminal: `./compile.sh`. Change the file paths in `ZenithSoC.sv` in the boot memory (the final output which is boot + program compiled is found in output.hex). If you're using `basic_system.sv` then change the file paths from the main memory (your_abs_path/program.hex) and boot memory (your_abs_path/boot.hex).