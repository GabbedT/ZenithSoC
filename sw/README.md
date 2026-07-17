# Bare-metal software

`sw/` contains the RV32 bare-metal support layer for ZenithSoC. It has no hosted runtime: applications use the linker/startup code supplied by their example or simulation flow and access peripherals through the MMIO drivers.

- `lib/`: common MMIO helpers, platform definitions, and driver headers.
- `src/`: driver implementations for UART, SPI, SD, Ethernet, GPIO, timer, PRNG, tracing, and audio.
- `examples/`: standalone firmware examples, each with its own linker/startup files and usually a `compile.sh` script.
- `benchmark/`: CoreMark and Embench-IoT ports with Makefiles for simulation and SD-card images.
- `tests/`: SoC level tests

## Toolchain and examples

The examples expect the `riscv32-unknown-elf-` GNU toolchain and the project’s current RV32 ISA configuration. From an example directory:

```bash
cd sw/examples/debug_print
bash compile.sh
```

The scripts generate ELF, dump, binary, and hex artifacts as appropriate. Paths and library versions in the legacy scripts may need adjustment for a new workstation; the `cosim` and `vp` Makefiles are the more reproducible build entry points.

## Benchmarks

```bash
cd sw/benchmark/CoreMark
make sim

cd ../Embench-IoT
make sim
```

The resulting firmware is intended for the Verilator testbench; see each benchmark directory for SD-image targets and parameters.
