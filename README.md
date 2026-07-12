# ZenithSoC

ZenithSoC is a modular 32-bit RISC-V system-on-chip written in SystemVerilog and targeted primarily at FPGA-based embedded systems. The repository combines the RTL, software drivers and examples, simulation environments, a virtual platform, and Sphinx documentation in one development tree.

## What is included

- `hw/`: synthesizable SoC RTL, including the ApogeoRV CPU integration, AXI interconnect, caches, memories, audio processing unit, and peripherals.
- `sw/`: bare-metal drivers, examples, and benchmarks porting.
- `tb/`: simulation models and testbenches. The Verilator flow is the quickest way to run firmware against the full SoC.
- `vp/`: block-oriented virtual platform for software-driven peripheral tests.
- `cosim/`: lockstep RTL/Spike co-simulation for CPU and memory-system verification.
- `docs/`: Sphinx sources for the architecture, memory map, peripherals, and programming reference.
- `constraint/`: FPGA pin and memory-controller constraints.

The integrated design currently includes a RISC-V CPU complex, instruction and data caches, on-chip and DDR memory interfaces, AXI, UART, SPI, Ethernet, SD, GPIO, timer, PRNG, VGA, tracing, and audio capture/synthesis blocks. See [`ROADMAP.md`](ROADMAP.md) for status and planned work.

## Repository status

This is an active hardware project. Some flows depend on local FPGA tools, a RISC-V cross-compiler, Verilator, and Spike installations. Generated simulation output is intentionally kept out of version control.

## Quick start: Verilator

The full-SoC Verilator testbench loads an ELF into the simulated DDR and optionally a boot ELF into the boot ROM. Build the firmware first, for example with the co-simulation flow:

```bash
source setenv.sh
cd tb/verilator
make run DDR=../../cosim/out/firmware.elf BOOT=../../cosim/out/boot.elf
```

Useful options include `WAVE=1` to write a waveform, `TRACE=1` to write trace file, and `MAX_CYCLES=N` to bound a run. See [`tb/verilator/README.md`](tb/verilator/README.md).

## Quick start: Cosimulator

Run CPU lockstep co-simulation with Spike:

```bash
source setenv.sh
cd cosim
make info
make run-notrace SEED=0 N=2000
make regress N=100 JOBS=4
```

## Quick start: Virtual Platform

Run a peripheral block on the virtual platform:

```bash
cd vp
make info BLOCK=uart
make run-notrace BLOCK=uart
```

## Quick start: Documentation

Build the documentation:

```bash
cd docs
python3 -m pip install -r requirements.txt
make html
# Open docs/_build/html/index.html
```

## Working with the RTL

The top-level filelist is [`hw/_zenithSoC.f`](hw/_zenithSoC.f), and the top-level RTL module is [`hw/ZenithSoC.sv`](hw/ZenithSoC.sv). Component filelists such as `hw/io/uart/_uart.f` keep each subsystem’s source list close to its implementation. Address and interface packages live under `hw/utils/pkg/`.

## Contributing

When changing RTL, update the relevant documentation and add or extend a simulation or software-driven test. Keep generated files in the ignored output directories, use clear commits, and verify the narrowest applicable flow before opening a pull request. New peripherals should include their register map, software driver, and a documented test path.

## Licensing

The repository includes third-party components with their own license files, notably [`hw/cpu/ApogeoRV/LICENSE`](hw/cpu/ApogeoRV/LICENSE) and the imported benchmark trees. Add or consult the project-level license metadata before redistributing the complete repository, and preserve the terms of every included component.
