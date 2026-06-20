# SPI Program

This program mainly tests the SPI module and the written drivers. Other modules and drivers are also tested.

After boot setup, the program initialize a timer to `ONE_SHOT` mode, which means that once the timer reaches the threshold, it stops until the programmer resets it. 

Then the Serial port is also initialized to enable debugging.

SPI module is then initialized in:

* SCLK at 1MHz
* MODE 0
* MSB First

Since an SPI transaction can excange more that one 8 bit word, two array of three bytes are created to send commands to the SPI slave, the write command used to initialize and reset the slave is composed by the fields:

* Write Register Command (0x0A)
* Register Address
* Data to Write

Once the device has been initialized, a read command is issued to read all the available registers in the device. This is done by a loop. The command is composed by the fields:

* Read Register Command (0x0B)
* Register Address
* End Byte (0x00)

After, the burst capablilites of the SPI link are tested, the X, Y, Z axis and temperature registers of the accelerometer are read in an infinite loop, the values are packed into 16 bit variables which are then printed to the terminal.

# Compilation

To compile, ensure that the script `compile.sh` can be executed, then in the terminal: `./compile.sh`. Change the file paths in `ZenithSoC.sv` in the boot memory (the final output which is boot + program compiled is found in output.hex). If you're using `basic_system.sv` then change the file paths from the main memory (your_abs_path/program.hex) and boot memory (your_abs_path/boot.hex).