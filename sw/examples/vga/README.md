# VGA cube/fractal SD demo

This example boots through the shared ZenithSoC SD bootloader, copies the application
from SD block `0x2000` to DDR at `0x80000000`, and displays either a rotating,
software-rasterized cube or a continuous fixed-point Mandelbrot zoom through
the ZenithSoC VGA controller.

The fractal camera is expressed relative to the exact center pixel, so integer
rounding cannot make the zoom target drift. Its scale decreases monotonically
and never reverses. Three fixed-point paths are selected automatically: Q2.14
uses the native RV32 multiplier for the common fast path, Q4.28 preserves
detail after Q2.14 is exhausted, and Q4.60 extends the practical zoom range to
about `10^16:1`. The iteration budget grows only as deeper detail requires it.
The renderer also skips the first, known Mandelbrot iteration and rejects the
main cardioid and period-2 bulb analytically.

The renderer supports both 320x240 RGB444 with hardware 2x scaling and native
640x480 RGB444. It uses a depth buffer, fixed-point transforms, and two DDR
framebuffers switched at the VGA early-frame boundary. Before every swap, a
cache-index sweep followed by `FENCE` publishes dirty pixels to the
non-coherent VGA DDR master. High-resolution mode advances the angle twice per
rendered frame to compensate for its lower software-rendering frame rate.

Build both deployment artifacts:

```bash
make
```

Select native high resolution with:

```bash
make RESOLUTION=640x480
```

`RESOLUTION=320x240` is the default. Configuration-specific object directories
prevent stale objects when switching mode.

The cube is the default demo. Select the continuous fractal zoom with:

```bash
make VGA_DEMO=fractal
```

The demo selection is made at compile time in `vga.cpp`; both renderers are
kept in separate translation units (`cube.cpp` and `fractal.cpp`).

The outputs are:

- `out/boot.hex`: boot-ROM image built from `sw/bootloader`.
- `out/program.bin`: application image to write at SD block `0x2000`.
- `out/boot.elf` and `out/vga_<demo>.elf`: symbol-bearing versions used by the
  Verilator harness.

Run with the live Verilator VGA window:

```bash
make run
```

For a headless simulation that continuously updates a PNG:

```bash
make run-headless
```

For automated finite runs, rebuild with a frame limit before invoking the
testbench:

```bash
make clean
make VGA_DEMO=fractal DEMO_FRAMES=3
make -C ../../../tb/verilator run \
  DDR=$PWD/out/vga_fractal.elf BOOT=$PWD/out/boot.elf \
  SD=$PWD/out/program.bin SD_BLOCK=0x2000 TRACE=0 \
  VGA_DUMP=$PWD/out/vga_latest.png
```

To prepare a card or disk image manually:

```bash
../../../tools/write_sd.sh /dev/sdX out/program.bin 0x2000
```
