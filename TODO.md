## UART

* Verify CTS/RTS flow control

* Docs: add warnings: 
  * Create MIG on vivado or modify the design for the target FPGA/Software tool in ddr_memory_interface.sv
  * Modify clocking wizard for the target FPGA/Software in system_clocking.sv
  * Modify multiplier for the target latency or FPGA/Software in integer_multiplier.sv
  * Modify multiplier for the target latency or FPGA/Software in float_multiplier.sv
  * Modify the path of the boot memory in ZenithSoC.sv

* Build I2C device
* Integrate VGA

Add write strobe to Eth registers

Change abs path to sine wave gen (user must run python script to generate the table)

Test interrupts

Test wait for interrupt instruction (sleep)


GESTIONE DATI PARZIALI NEL CASO DI INTERRUZIONE BURST DELEGATA AL SOFTWARE