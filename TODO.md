## UART

* Verify CTS/RTS flow control

* Docs: add warnings: 
  * Create MIG on vivado or modify the design for the target FPGA/Software tool in ddr_memory_interface.sv
  * Modify clocking wizard for the target FPGA/Software in system_clocking.sv
  * Modify multiplier for the target latency or FPGA/Software in integer_multiplier.sv
  * Modify multiplier for the target latency or FPGA/Software in float_multiplier.sv
  * Modify the path of the boot memory in ZenithSoC.sv

* Verify FPU
* Build I2C device
* Integrate VGA

SPI add disable RX