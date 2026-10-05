# CPU co-simulation

`cosim/` compares ZenithSoC retirement with Spike, including PC, destination
register, register value, memory-operation presence, addresses, store widths and
store data. It checks the architectural register file between batches and the
whole generated data area after committed stores have drained. RTL assertions
remain enabled by default. CPU/cache RTL and FPGA constraints are unchanged.

## Setup

Requires Verilator 5+, a C++20 compiler, Python 3, the RISC-V GCC/binutils
configured in `config.mk`, and a matching Spike executable plus `libriscv` and
`libfesvr`. `fst2vcd` and GTKWave are optional.

```bash
source setenv.sh                    # from the repository root
export PATH=/opt/riscv/bin:$PATH    # if your cross toolchain is installed here
cd cosim
make info
make test SPIKE="$SPIKE_LIB/spike"
make regress N=32 PROG_LEN=2000 JOBS=4
```

`make test` covers deterministic generation, ISA gating, memory-address bounds
for both cache geometries, coverage accounting, and injected runner failures.
Assembler/standalone-Spike tests run when `RISCV_GCC` and `SPIKE` are available;
the Make target forwards their resolved paths to Python. For example:

```bash
SPIKE="$SPIKE_LIB/spike" make test
```

The stronger tests currently expose existing RTL defects; see
[known findings and minimal reproducers](FINDINGS.md). A failing campaign is not
silently converted to a successful regression.

## Stimulus generation

Programs combine independently randomized legal instructions with directed
sequences. The planner first visits each enabled generator once (if the block
budget permits), then samples weighted families. Operands alternate between
broad register selection and small, randomly chosen register windows to create
RAW, WAR, WAW and bypass pressure. `x30` remains the bounded-loop counter and
`x31` the protected data base; `x0` is included in ordinary arithmetic and memory
operands. Compiler-owned stack/link registers are preserved.

The generated body includes:

- Immediate/shift extremes, AUIPC, destination/source aliases, divide-by-zero,
  signed division overflow and dependent arithmetic chains.
- Aligned accesses throughout 32 KiB of seeded, nonzero initial memory; positive
  and negative displacements; overlapping byte/halfword/word stores; signed and
  unsigned reloads; load-to-address dependencies; store-buffer pressure; dirty
  eviction sweeps over multiple tags of one cache index; fences.
- Branch diamonds with distinct register/memory effects, bounded backward loops,
  indirect jumps with aliased target/link registers and odd targets, and taken
  branches that must squash stores and illegal instructions.
- Explicit RV32C instructions/hints and 32-bit instructions at halfword fetch
  boundaries. Random ordinary instructions are kept 32-bit instead of relying
  entirely on assembler compression.
- Zfinx zeros, subnormals, finite extremes, infinities, NaNs and conversion
  boundaries, with all five static rounding modes. FMA, FDIV and FSQRT remain
  excluded because they are not implemented by this core.

`PROFILE=auto` chooses a deterministic per-seed workload from `mixed`, `memory`,
`dependencies`, `control`, and `float`. Set one explicitly for a focused campaign.
`CLASS` gates instruction families as before. `N`/`PROG_LEN` count generator
**blocks**, not emitted or retired instructions; directed blocks can expand into
many instructions, and loops execute repeatedly.

```bash
make gen SEED=7 N=1000 PROFILE=memory
make run-notrace SEED=7 N=1000 PROFILE=memory
make run SEED=7 N=1000 PROFILE=memory        # waveform and instruction trace
make regress N=32 START_SEED=100 PROFILE=control CLASS=arith,branch,ctrl
make regress SOC_CONFIG=1 N=32 JOBS=4
```

`SOC_CONFIG=1` uses the FPGA CPU/cache parameters. Generator defaults are 8192
cache bytes / 32-byte lines in this mode, versus 4096 / 16 otherwise. If those
RTL parameters change, update or override `CACHE_BYTES` and `LINE_BYTES` too.
`BUILD_JOBS` controls compiler parallelism separately from regression `JOBS`.

## Reproducibility and failures

`GEN_SEED` identifies a campaign, and `SEED` identifies a program in it.
`make genseed` replaces `.genseed`; explicit `GEN_SEED=...` restores a campaign.
Seeds are reproducible for a given generator version and configuration, not
across generator changes.

Every regression gets a new directory under `out/rtt/campaign-.../`. Existing
failure directories are retained. Each seed records:

- `generator.json`: resolved profile, ISA, cache geometry, scenario/opcode
  counts, and source SHA-256.
- `result.json`: exact subprocess arguments, memory seed, stage and exit status.
- Separate generation, compilation, standalone-reference and simulation logs.
- `generated.cov` and `executed.cov`; `coverage.dat` for instrumented builds.
- On failure: `prog.c`, `fw.elf`, `boot.elf` and `firmware.dis`.

The campaign also saves its simulator binary, configuration and `summary.json`.
Successful sources/ELFs are removed by default; direct `regress.py --keep-pass`
retains them. Replay uses the retained ELF and simulator, without regenerating:

```bash
make replay RUN_DIR=out/rtt/campaign-.../seed7
make replay RUN_DIR=out/rtt/campaign-.../seed7 REPLAY_TRACE=1
```

Replay still requires the compatible shared Spike libraries. To regenerate a
failure, pass the original `SEED`, `GEN_SEED`, length, profile, classes, geometry
and memory settings to `make run`; the default memory seed is derived identically
in single runs and campaigns. `MEM_SEED` can override it in single runs.

The runner stops a seed at its first failed stage. By default it first checks
that the program terminates on standalone Spike; `CHECK_REFERENCE=0` skips this
extra stage, while lockstep still uses Spike. It requires both zero exit status
and an explicit completion PASS. Instruction/cycle limits and timeouts fail:

- `MAX_RETIRE=1000000` by default; `0` disables this limit.
- `MAX_CYCLES=20000000`, including boot and memory drain.
- `TIMEOUT=120` seconds per subprocess in a campaign/replay.
- A separate no-retirement watchdog detects deadlocks.

## Memory timing

`MEM_RANDOM=1` enables deterministic DDR backpressure and variable response
latency. `MEM_RANDOM=0` uses fixed timing for isolating functional failures.
Responses remain ordered, byte strobes are honored, and randomized read returns
are spaced by at least four cycles because the adapter serializes 128-bit
responses into four words without response-side backpressure.

```bash
make regress N=16 MEM_LATENCY_MIN=1 MEM_LATENCY_MAX=40 MEM_STALL_PERCENT=60
make run-notrace SEED=4 MEM_SEED=2700688153 MEM_RANDOM=1
```

Defaults: latency 2..16 cycles and 25% request stall probability. Queueing can
increase total response latency. `MEM_LATENCY_MAX` accepts up to 10000 and
`MEM_STALL_PERCENT` up to 95. This is a digital protocol stress model, not a DDR
PHY timing model.

## Coverage

```bash
make coverage-report
make regress N=8 COVERAGE=1
verilator_coverage --write out/merged.dat out/rtt/campaign-.../seed*/coverage.dat
verilator_coverage --annotate out/annotated out/merged.dat
```

`out/cov/regress.cov` aggregates **successfully compared generated-body events
from passing runs**. Startup/initialization are excluded using ELF symbols.
`failed.cov` holds the available compared prefixes of failed runs separately;
assertion aborts may not flush coverage. Counts are not branch coverage from the
compiler, nor a claim of full architectural coverage.

Bins include executed mnemonics (Spike aliases included), instruction lengths,
fetch boundaries, destination registers, result edges, x0, operand aliases,
RAW distances for 32-bit source instructions, each branch condition/outcome,
indirect jumps, memory widths/lanes, data pages, line ends and overlap with the
latest store. Compressed writes update dependency history, but compressed source
operands and branch outcomes are not yet decoded into those bins.

`generated.cov` and `scenarios.cov` describe static generation for passing runs;
they do not prove that every emitted instruction executed. The report lists
unhit targets explicitly. `COVERAGE=1` additionally writes Verilator structural
coverage per worker without file collisions; `make verilator-coverage` annotates
a single run's `out/coverage.dat`.

## Current boundaries

Expected synchronous trap/interrupt campaigns, privilege transitions, atomics,
CSR-state/FP-flag comparisons and self-modifying code are not implemented.
Unexpected DUT exceptions fail immediately. The bundled two-instruction boot
ROM is validated explicitly; arbitrary boot ROMs require extending that model.
Aligned memory accesses are intentionally legal; illegal opcodes occur only on
paths that must be squashed. These boundaries are not counted as covered.
