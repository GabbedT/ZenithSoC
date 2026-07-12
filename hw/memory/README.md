# Memory subsystem

`hw/memory/` provides on-chip storage, instruction/data cache complexes, and the DDR interface used by the SoC. The top-level memory pieces are included by [`../_zenithSoC.f`](../_zenithSoC.f).

- `cache/`: instruction and data cache implementations and banks.
- `ddr/`: DDR memory and cache-facing interfaces.
- `on_chip_memory.sv` and `memory_bank.sv`: local memory structures.

Memory behavior is exercised by the full Verilator testbench and the Spike co-simulation flow. Keep address and timing assumptions aligned with `hw/utils/pkg/soc_parameters.sv` and the documentation.
