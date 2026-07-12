# ZenithSoC virtual platform

The virtual platform runs small firmware tests against individual peripheral blocks using Verilator. It is useful for fast, software-driven checks without requiring a complete board-level environment.

## Prerequisites

Install Verilator and the RV32 cross-compiler. Spike include/library paths can be supplied with `SPIKE_DIR` when a block test needs them.

## Run a block

Available checked-in blocks include `uart` and `sd`; `_template` is the starting point for new blocks.

```bash
cd vp
make info BLOCK=uart
make run-notrace BLOCK=uart
make run BLOCK=uart TRACE_START=0 TRACE_END=1000
make wave BLOCK=uart
```

The Makefile builds `blocks/$(BLOCK)/sw/firmware.cpp`, its RTL wrapper, and the shared SoC filelist. Use `make run-timeout BLOCK=uart TIMEOUT=10s` when debugging a firmware hang.

## Add a block

```bash
make new-block NAME=my_peripheral
```

Then edit the generated `block.mk`, RTL wrapper, firmware, and MMIO base mapping in `Makefile`. `make clean` removes generated `obj_dir/` and `out/` contents.
