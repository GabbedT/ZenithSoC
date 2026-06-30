# ZenithSoC – Verilator top-level testbench

Standalone Verilator testbench that verilates the **whole** `ZenithSoC` (no
external DDR/SD models), loads a firmware ELF into the on-chip DDR/boot ROM, and
runs the core freely while printing an execution trace.

## Quick start

Example: 

```bash
make run DDR=../../../cosim/out/firmware.elf BOOT=../../../cosim/out/boot.elf
```

## Make options

| Variable | Meaning | Default |
|----------|---------|---------|
| `DDR=path.elf` | firmware ELF (loaded into DDR `0x8000_0000` / ROM) — **required** | – |
| `BOOT=path.elf` | optional boot-stub ELF (loaded into ROM `0x0`) | – |
| `WAVE=1` | dump `out/zenith.fst` | `0` |
| `TRACE=0` | disable the per-instruction trace | trace on |
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

## Future: interactive manual control

The `Sim` class and the wrapper already expose the seams (pause between ticks,
DPI-driven `dut` access) to add manual commands — poke memory, force an
interrupt, inspect registers — then resume. See the scaffolding comments in
`zenith_tb_top.sv` and `sim_main.cpp`.