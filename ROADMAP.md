# ZenithSoC Roadmap 

The project will be developed using an incremental approach, structured into four distinct phases. Each phase builds upon the functionality established in the preceding step, progressively enhancing the platform capabilities. Notably, these phases are designed to be modular, allowing for any configuration to operate independently without dependency on subsequent additions. Here’s the phased development plan:

## Roadmap 

-> WFI instruction
-> Rename directory
-> Delete relative path and substitute with .f file
-> Add wrapper to Vivado files
-> Modify interrupt unit
-> Add external interrupts
-> Trace Unit design and verification
-> I2C design and verification
-> Interrupt verification
-> Verify again every block extensively with SW driven tests thanks to Virtual Platform
-> Verilator SoC level testbench with simplified models and Spike Cosimulation
-> Add firmware for test to do a regression
-> Coremark porting for SoC
-> GPU design
-> ADC System