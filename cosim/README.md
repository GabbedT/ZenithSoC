# CPU co-simulation

`cosim/` runs the ZenithSoC CPU complex in Verilator and compares each retired instruction with the Spike ISA model. It checks the committed PC, register result, and memory access, then performs architectural register and final-memory checks when the run ends.

## Prerequisites

- Verilator 5 or newer
- A C++20 host compiler
- `riscv32-unknown-elf-g++` and `riscv32-unknown-elf-objdump`
- A Spike build providing `libriscv` and `libfesvr`
- Python 3
- GTKWave and `fst2vcd` only when inspecting waveforms

Source the environment before building. It assumes Spike is installed under `$HOME/riscv-isa-sim`; override `SPIKE_SRC`, `SPIKE_BUILD`, `SPIKE_INC`, or `SPIKE_LIB` after sourcing when needed.

```bash
source cosim/setenv.sh
cd cosim
make info
```

The ISA and privilege configuration is centralized in [`config.mk`](config.mk). Change it there so GCC, the generator, and the RTL build remain aligned.

## Main targets

```bash
make build                         # Verilate and compile the lockstep simulator
make gen SEED=7 N=2000             # Generate tests/prog.c
make firmware boot                 # Build user firmware and boot ROM
make run-notrace SEED=7            # Full run without waveform tracing
make run SEED=7 MAX_RETIRE=5000    # Full run with tracing and a retire limit
make regress N=100 JOBS=4          # Run seeds 0..99 in parallel
make genseed                       # Start a completely new test campaign
make coverage-report               # Summarize generator opcode coverage
make wave                          # Convert/open the latest FST waveform
make clean                         # Remove generated build and test output
```

`SEED` is deterministic within the current generator campaign. This makes a
failure from `make regress` reproducible with `make run SEED=<failing_seed>`.
The campaign identifier is stored in `.genseed`; `make genseed` replaces it so
that the following regression generates a completely different population.
An older population can still be reproduced by passing its identifier
explicitly, for example `make run SEED=7 GEN_SEED=1234`.

Regression coverage is accumulated into `out/cov/regress.cov`, with one summed
line per mnemonic; no coverage file is created for each individual seed.

`CLASS` selects generated instruction groups, `PROG_LEN` controls regression program length, and `COVERAGE=1 make build` enables Verilator structural coverage. A successful run ends with `COSIM PASS`; a mismatch prints the first divergent instruction and recent PCs.

Generator-only checks can be run without rebuilding the simulator:

```bash
python3 -m unittest discover -s gen/tests -v
```

## Layout

- `rtl/`: co-simulation wrapper, ROM, DDR, and I/O models.
- `sim/`: C++ harness, ELF loader, Spike stepping, and comparisons.
- `sw/`: startup code and linker scripts for user firmware and boot ROM.
- `gen/rvgen.py`: CLI, instruction selection, coverage counting, and C output.
- `gen/instructions/`: separate integer, memory, branch/control-flow, and Zfinx
  floating-point generators. Each family separates basic instruction forms from
  deliberate corner cases.
- `tests/`, `out/`, `obj_dir/`, and `logs/`: generated artifacts.
