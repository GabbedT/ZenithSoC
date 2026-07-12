# Hardware RTL

`hw/` contains the synthesizable SystemVerilog implementation of ZenithSoC. The complete source set is assembled by [`_zenithSoC.f`](_zenithSoC.f), which is consumed by the simulation and integration flows.

## Top-level integration

- [`ZenithSoC.sv`](ZenithSoC.sv) is the SoC integration module.
- `_zenithSoC.f` is the canonical source filelist.
- `utils/pkg/soc_parameters.sv` and the other package files define shared interfaces, addresses, and parameters.

## Subsystems

- [`cpu/`](cpu/): ApogeoRV integration, caches, interrupts, and trace unit.
- [`memory/`](memory/): on-chip memory, cache complexes, and DDR interfaces.
- [`bus/`](bus/): AXI masters, slaves, routing, and network fabric.
- [`apu/`](apu/): audio capture and synthesis pipelines.
- [`io/`](io/): UART, SPI, Ethernet, SD, GPIO, timer, PRNG, and VGA peripherals.
- [`common/`](common/): clocks, reset, synchronizers, buffers, and edge detection.
- [`utils/`](utils/): packages, interfaces, and Vivado definitions.

For a full simulation, use [`tb/verilator`](../tb/verilator) or [`vp`](../vp). Hardware implementation projects should additionally include the appropriate files from [`constraint`](../constraint).
