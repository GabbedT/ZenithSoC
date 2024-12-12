# Ethernet Program

This program mainly tests the Ethernet MAC module and the written drivers of the class `Ethernet`. Other modules and drivers are also tested. 

After boot setup, the program initialize a timer to `ONE_SHOT` mode, which means that once the timer reaches the threshold, it stops until the programmer resets it. 

Then the GPIO is initialized as input, the switches are used to change Ethernet mode. The Serial port is also initialized to enable debugging.

Ethernet is last to be initialized, the following setup is used:
* 100Mbps
* Full Duplex
* No Auto Negotiation
* IEEE_8023

The programmer needs to fill the MAC address of the destination (the PC probably). 10 seconds are waited then the PHY register contents are printed to check correctness.

The most important function of this program is `packetLoop`, it enters in a loop that goes until the GPIO is deasserted. If it detects that the MAC is receiving a packet, immediately send another packet to respond and test the full duplex capability of the link. Once the packet has been received, the packet is fetched from the MAC buffer and its informations are printed.

The program consists in 4 sections

* 100Mbps IEEE 802.3 Mode
* 10Mbps IEEE 802.3 Mode
* 100Mbps Ethernet II Mode
* 10Mbps Ethernet II Mode

For each test case, the content of the control and status register is printed, the program then try to enstablish a link between the SoC and the machine. After the link has been enstablished, the function `packetLoop` is called.

# Compilation

To compile, ensure that the script `compile.sh` can be executed, then in the terminal: `./compile.sh`. Change the file paths in `ZenithSoC.sv` in the boot memory (the final output which is boot + program compiled is found in output.hex). If you're using `basic_system.sv` then change the file paths from the main memory (your_abs_path/program.hex) and boot memory (your_abs_path/boot.hex).