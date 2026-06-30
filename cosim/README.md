# ZenithSoC Co-simulation

Instruction-by-instruction co-simulation that runs the **CPU Complex** RTL with ApogeoRV core
in lockstep against the **Spike** golden ISA model. For
every instruction the DUT retires, the simulator steps Spike once and compares
PC, destination register value, and memory access address/data. 

In addition:

* A full achitectural GPR comparison is done once the commit queue drains
* End of run memory diff to spot caching problems

The simulation stops at the first mismatch (printing diagnostics) or on HTIF termination.

> CSR comparison is intentionally excluded (`mcycle`, `minstret`, ... are expected
> to diverge). There are no interrupts in either privilege configuration.

---

## 1. Directory layout

cosim/
├── Makefile          # build / gen / firmware / boot / run / regress targets
├── config.mk         # SINGLE source of truth: ISA, extensions, priv, tools
├── setenv.sh         # environment bootstrap (source it before building)
├── rtl/              # SystemVerilog co-sim wrapper
│   ├── cosim.f           # filelist: pulls in CPU + caches + this wrapper
│   ├── cosim_top.sv      # top module: CPU complex + memories + RVFI tap (DPI)
│   ├── cosim_ddr.sv      # DDR model with random latency + DPI preload
│   ├── cosim_rom.sv      # boot ROM model + DPI preload
│   ├── cosim_io.sv       # IO stub (loads return 0, stores accepted)
│   └── cosim_public.vlt  # verilator: expose internal backend signals to C++
├── sim/              # C++ harness (verilated --exe)
│   ├── cosim.cpp         # main lockstep loop, Spike driver, comparison
│   └── elf_loader.h      # minimal ELF32 reader (segments + tohost symbol)
├── sw/               # firmware / boot software
│   ├── crt0_user.s       # user startup: sp init, .bss clear, main(), HTIF exit
│   ├── link_user.ld      # user linker script (DDR @ 0x8000_0000)
│   ├── boot_rom.s        # boot ROM stub
│   └── boot_rom.ld       # boot ROM linker script
├── gen/
│   └── rvgen.py          # random RISC-V program generator (seeded)
└── tests/
    └── prog.c            # generated test program (overwritten by `make gen`)

### How it works

1. Verilator builds `cosim_top` (CPU complex + ROM/DDR/IO models) together with
   `sim/cosim.cpp` into `obj_dir/Vcosim_top`.

2. The firmware ELF is loaded into both:
   - Spike RAM at `0x8000_0000` via `memif().write`
   - the DUT DDR/ROM through DPI (`ddr_preload_word` / `rom_preload_word`)

3. The clock advances. Each committed RTL instruction calls `rvfi_commit()`
   through DPI; the harness runs `spike.step(1)` per event and compares.

4. Termination is a store to the `tohost` symbol (HTIF). `tohost == 1` -> PASS.

---

## 2. Prerequisites

You need three external toolchains plus Spike:

| Tool | Required | Notes |
| Verilator | 5.0+ | Builds the DUT + C++ harness. |
| Host C++ compiler | GCC 14.2.0 | Must match the compiler that built Spike's `libriscv.so` |
| RISC-V cross compiler | GCC 13+ | Must support `zfinx` |
| Spike | riscv-isa-sim | built `libriscv` / `libfesvr` + headers |


## 3. Configuration 

`config.mk` is the configuration file. The ISA string is built once and
consumed by three places: Verilator (RTL), GCC (`-march`), and `rvgen.py`
(`--ext`). Edit it there, not in the Makefile.

ISA_BASE   := rv32i
EXT_M      := m       # mul/div
EXT_C      :=         # compressed (set "c" to enable)
EXT_ZICSR  := zicsr   # CSR (required for zfinx / traps)
EXT_ZBA    := zba     # sh1add/sh2add/sh3add ...
EXT_ZBS    := zbs     # bset/bclr/binv/bext ...
EXT_ZBB    :=         # basic bit manip (set "zbb" to enable)
EXT_ZFINX  := zfinx   # floating-point in GPRs (no F registers)

NO_FDIV    := 1       # 1 = pass -mno-fdiv and disable fdiv/fsqrt in the generator
PRIV       := mu      # "m" or "mu" (Machine + User). No interrupts either way.

Run `make info` to see the resolved configuration.

---

## 5. Make targets

| Target | Description |
| `make build` | Verilate `cosim_top` + `sim/cosim.cpp`. Add COVERAGE=1 to build with `--coverage` option |
| `make gen` | Generate a random program (`SEED`, `N`, `CLASS`) -> `tests/prog.c`. |
| `make firmware` | Compile `tests/prog.c` + `crt0_user.s` -> `out/firmware.elf`. |
| `make boot` | Compile the boot ROM stub -> `out/boot.elf`. |
| `make run` | `build + gen + firmware + boot`, then lockstep with FST waveform. |
| `make run-notrace` | Same as `run` but without waveform dump (faster). |
| `make regress` | Generate + run `N` programs in parallel (sweep `seeds 0..N-1`), report PASS/FAIL and exit non zero if seed failed. |
| `make regress-one SEED=<n>` | Run a single seed |
| `make coverage-report` | Aggregate the per seed generator opcode histogram (`out/cov/*.cov`). |
| `make verilator-report` | Annotate Verilator structural coverage (`out/cov_annotated`). |
| `make info` | Print the resolved ISA / priv / tool configuration. |
| `make wave` | Convert and open `out/cosim.fst` in GTKWave. |
| `make clean` | Remove `obj_dir`, `out`, `tests/prog.c`. |

### Run parameters (override on the command line)

| Variable | Default | Meaning |
| `SEED` | `0` | Random seed for the program generator. |
| `N` | `2000` | Number of generated instructions / program count for `regress`. |
| `PROG_LEN` | `2000` | Number of instructions per program during a `regress` sweep. |
| `CLASS` | `arith,mem,branch,ctrl,float` | Instruction classes to generate (CSV). |
| `JOBS` | `$(nprocs)` | Parallel seeds for `regress` |
| `COVERAGE` | `0` | Build verilator with `--coverage` |
| `MAX_RETIRE` | `0` | Stop after N retired instructions (`0` is infinite) |
| `PROG` | `tests/prog.c` | Program source to compile. |

---

## 6. Typical workflows

### One full run from scratch

source cosim/setenv.sh     # + the PATH/CXX exports from §3
cd cosim
make clean
make run-notrace SEED=0 N=2000 

Expected tail on success:

[COSIM] start lockstep (firmware=out/firmware.elf)
[COSIM] tohost write detected (value=0x1)
[COSIM] PASS - <N> instructions compared, memory verified, no mismatch.


### Run an existing binary directly

./obj_dir/Vcosim_top +firmware=out/firmware.elf +boot=out/boot.elf \
                     +notrace +max_retire=5000

Plusargs accepted by the harness: `+firmware=`, `+boot=`, `+notrace`,
`+max_retire=`.

## 7. Reading a mismatch

On the first divergence the harness prints the retire index, the disassembled
instruction, the DUT-vs-Spike PC / `rd` / memory values, and the last 8 PCs:

[COSIM][MISMATCH] @retire #1443 (store data)
instr : sb t3, 686(t6)
PC    : DUT 0x800002b4 | SPIKE 0x800002b4
mem   : DUT ST @0x800007ae data 0x018b0000
last PCs : 80000298, ..., 800002b4
[COSIM] FAIL

---

## 8. Memory map

| Region | Address | Model |
| Boot ROM | `0x0000_0000` - `0x0000_3FFF` | `cosim_rom.sv`, registered read, never stalls. |
| User DDR | `0x8000_0000` + | `cosim_ddr.sv`, random latency (`LAT_MIN..LAT_MAX`), burst model. |
| IO stub | (IO channel) | `cosim_io.sv`, reads return `0`, writes always accepted. |

The user program links and runs from DDR at `0x8000_0000`; HTIF `tohost`/`fromhost`
live in the `.htif` section (see `sw/link_user.ld`). The DUT starts from the boot
ROM stub; the harness discards ROM-stub retires (`PC < 0x8000_0000`) until the
first user instruction, aligning the comparison with Spike's entry point.