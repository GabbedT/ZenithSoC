# CPU subsystem

The CPU subsystem connects the imported [ApogeoRV](ApogeoRV/) RISC-V core to ZenithSoC caches, interrupts, and tracing. [`cpu_complex.sv`](cpu_complex.sv) is the integration point and [`_cpu.f`](_cpu.f) includes the core and cache sources.

- `ApogeoRV/`: upstream CPU core and its own documentation/license.
- `trace_unit/`: packetization, serialization, registers, and trace integration.
- `interrupt_controller.sv`: SoC interrupt management.

The CPU and cache documentation is under [`docs/cpu_complex/`](../../docs/cpu_complex/). Changes inside `ApogeoRV/` should follow that component’s contribution guidance.
