# ZenithSoC Roadmap 

The project will be developed using an incremental approach, structured into four distinct phases. Each phase builds upon the functionality established in the preceding step, progressively enhancing the platform capabilities. Notably, these phases are designed to be modular, allowing for any configuration to operate independently without dependency on subsequent additions. Here’s the phased development plan:

## Roadmap 

-> Delete relative path and substitute with .f file ✅
-> Add CPU cosimulation with Spike ✅
-> Run multiple regressions and fix bugs in Cache Complex
-> Run multiple regressions and fix C extension bugs
-> Run multiple regressions and fix Zfinx extension bugs

-> Add an easy way to sync SoC Memory Map and Parameters to VP - SW - TB
-> Add a verilator testbench for fast software testing

-> Coremark porting for SoC
-> Run Coremark on verilator testbench and extract performance

-> Verify again every block extensively with SW driven tests thanks to Virtual Platform
-> Trace Unit design and verification

-> Port SW driven test on VP to firmware for test to do a regression suite

-> WFI instruction with verification
-> Fix every interrupt interface in blocks (with C1W pending)
-> Modify interrupt unit (add level / edges detection)
-> CPU level test interrupts verification
-> Add external interrupts
-> SoC level interrupt verification

-> I2C design and verification

-> GPU design

-> ADC System