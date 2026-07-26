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