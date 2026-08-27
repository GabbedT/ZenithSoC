# ZenithSoC – Verilator top-level testbench

Standalone Verilator testbench that verilates the **whole** `ZenithSoC` (no
external DDR model), loads a firmware ELF into the on-chip DDR/boot ROM, and
runs the core freely while printing an execution trace. The SD card uses the
same protocol model as `vp/` and accepts a binary or byte-oriented hex image.

## Quick start

Example: 

```bash
make run DDR=../../../cosim/out/firmware.elf BOOT=../../../cosim/out/boot.elf
```

SD boot example (the application ELF supplies symbols but is not preloaded in
DDR when `SD` is present):

```bash
make run \
  DDR=../../sw/benchmark/CoreMark/out/coremark_app.elf \
  BOOT=../../sw/benchmark/CoreMark/out/bootloader.elf \
  SD=../../sw/benchmark/CoreMark/out/coremark_app.bin \
  SD_BLOCK=0x2000
```

## Make options

| Variable | Meaning | Default |
|----------|---------|---------|
| `DDR=path.elf` | firmware ELF; with `SD`, used for symbols and verification but not preloaded | – |
| `BOOT=path.elf` | optional boot-stub ELF (loaded into ROM `0x0`) | – |
| `SD=path.bin\|hex` | SD contents loaded at `SD_BLOCK`; `.hex` is byte-oriented | – |
| `SD_BLOCK=N` | first block for the SD image | `0x2000` |
| `WAVE=1` | dump `out/zenith.fst` | `0` |
| `TRACE=0` | disable the per-instruction trace | trace on |
| `TRACE_START=N` | start the instruction trace after cycle N | `0` |
| `MAX_CYCLES=N` | stop after N cycles (`0` = run until `tohost`) | `0` |
| `ISA=...` | ISA string for the disassembler (match the firmware toolchain) | `rv32im_zfinx_zba_zbs_zicsr` |

Other targets: `make build`, `make wave` (open the latest FST in GTKWave),
`make info`, `make clean`.

## Trace format

```
0x80000000 : auipc   sp,0x10         x 2 <= 0x80010000
0x8000001c : sw      zero,0(t0)      | ST.w @0x80003cc0 data 0x00000000
```

PC · disassembled instruction · `rd <= value` · memory access (`LD/ST.<b|h|w> @addr [data]`).

## Performance counters

At the end of the run (after `tohost`, or at `MAX_CYCLES`) the simulation prints
a performance summary that identifies the **#1 bottleneck** of the ApogeoRV
pipeline and cache system.

```
======================================================================
  ZENITHSOC PERFORMANCE SUMMARY
======================================================================
  -- Global ------------------------------------------------
  total_cycles           :        788   IPC=0.183  CPI=5.462
  ...
  BOTTLENECK: st_raw_ldu (load-use RAW)
  (519 cycles, 65.9% of total)
======================================================================
```

Everything is counted **testbench-side** in `perf_counters.sv` by tapping DUT
signals through hierarchical references — no RTL changes are required. The
module is instantiated in `zenith_tb_top.sv` and its `report()` is exported to
C++ via the DPI function `zenith_perf_dump()` (called from `sim_main.cpp`).

Reported sections:

| Section | Contents |
|---------|----------|
| Global | total cycles, retired/issued instructions, IPC, CPI |
| Issue slots | issue / empty / stall slot breakdown (% of total cycles) |
| Stall causes | 19 priority-encoded buckets: backend, ROB full, flush busy, FENCE/CSR serialization, structural (DIV/LDU/STU busy), store blocked, per-unit RAW (ALU/MUL/DIV/LDU/STU), per-unit latency conflicts |
| Flushes | branch mispredictions + rate, branch/full flushes, FENCE events, exceptions |
| Caches | I$/D$ accesses, hits, hit rates; flush and refill stall cycles |
| Front-end | fetch stall, ibuffer full, icache conflict, fetch invalidations, backend bubbles |
| Memory units | LDU wait/lbuf-full, store-buffer full/wait, STU FSM state occupancy |
| DDR arbitration | data/fetch priority stalls, controller hold, store-idle, load-empty |
| Self-checks | invariant assertions (issue+empty+stall == total, retired ≤ issued, …) |
| BOTTLENECK | single largest contributor among empty slots and all stall buckets |

The bottleneck line distinguishes hazard classes (e.g. `st_raw_ldu` load-use RAW
vs `st_raw_alu` ALU RAW vs `st_struct_ldu` load-unit-full structural) so you can
see exactly what limits the core on a given workload. The same `perf_counters.sv`
is reused by the XSim testbench in `tb/top/` (see `tb/top/README.md`).

## Embench-IoT

The full-SoC Verilator testbench can run the 19 Embench-IoT benchmarks with the
same direct ELF-loading flow used by the other firmware tests. From the
repository root, run:

```bash
cd tb/verilator/script
./run_embench.sh 0 0
```

The first argument disables waveform generation and the second disables the
instruction trace. The script stores the complete logs in
`tb/verilator/logs/embench/` and reports the cycle count measured between
`start_trigger()` and `stop_trigger()`.

### Current Results

The following results are taken from the current logs. All benchmarks completed
successfully (`PASS`). The reported value is the number of SoC clock cycles at
which `EMBENCH_CYCLES` was printed; it is not host execution time.

| Benchmark | Cycles | Result |
| :-- | --: | :--: |
| `aha-mont64` | 5,492,019 | PASS |
| `crc32` | 4,182,180 | PASS |
| `depthconv` | 2,840,515 | PASS |
| `edn` | 3,771,791 | PASS |
| `huffbench` | 2,913,567 | PASS |
| `matmult-int` | 2,861,235 | PASS |
| `md5sum` | 2,590,511 | PASS |
| `nettle-aes` | 4,141,649 | PASS |
| `nettle-sha256` | 4,924,451 | PASS |
| `nsichneu` | 5,604,382 | PASS |
| `picojpeg` | 3,954,375 | PASS |
| `qrduino` | 3,573,520 | PASS |
| `sglib-combined` | 3,605,251 | PASS |
| `slre` | 3,250,644 | PASS |
| `statemate` | 5,152,525 | PASS |
| `tarfind` | 1,910,220 | PASS |
| `ud` | 2,613,106 | PASS |
| `wikisort` | 970,164 | PASS |
| `xgboost` | 5,338,860 | PASS |

Using `CPU_MHZ=100`, `GLOBAL_SCALE_FACTOR=1`, and the Embench reference values
in `sw/benchmark/Embench-IoT/embench-iot/baseline-data/speed.json`, the relative
score for each benchmark is computed as:

```text
relative score = reference time / (EMBENCH_CYCLES / (CPU_MHZ * 1000))
Embench score = geometric mean of the 19 relative scores = 118.67
Embench score/MHz = 1.19
```

## CoreMark

CoreMark can be run on the same full-SoC Verilator testbench. To build and run
the 3,000-iteration simulation:

```bash
cd tb/verilator/script
./run_coremark.sh 3000 0 0
```

### Risultato corrente

The result below is taken from `tb/verilator/logs/run.log`. The run completed
the standard 10-second measurement interval and validated correctly. CoreMark
prints `Total time (secs): 10` and `Iterations/Sec: 300` using integer seconds;
the precise score below is calculated from the cycle counter instead.

| Metric | Result |
| :-- | --: |
| Iterations | 3,000 |
| Clock | 100 MHz |
| Total cycles (`CYCLES`) | 1,012,098,368 |
| Retired instructions (`INSTRET`) | 831,249,814 |
| CPI | 1.217 |
| Measured time | 10.120983680 s |
| Iterations/sec | 296.413875850 |
| CoreMark/MHz | **2.96** |
| Result | **PASS** |
