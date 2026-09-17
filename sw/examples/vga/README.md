# VGA rotating-cube SD demo

This example boots through the CoreMark SD bootloader, copies the application
from SD block `0x2000` to DDR at `0x80000000`, and displays a rotating,
software-rasterized cube through the ZenithSoC VGA controller.

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

The outputs are:

- `out/boot.hex`: boot-ROM image built from CoreMark's `boot_sd.cpp`.
- `out/program.bin`: application image to write at SD block `0x2000`.
- `out/boot.elf` and `out/vga_cube.elf`: symbol-bearing versions used by the
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
make DEMO_FRAMES=3
make -C ../../../tb/verilator run \
  DDR=$PWD/out/vga_cube.elf BOOT=$PWD/out/boot.elf \
  SD=$PWD/out/program.bin SD_BLOCK=0x2000 TRACE=0 \
  VGA_DUMP=$PWD/out/vga_latest.png
```

To prepare a card or disk image manually:

```bash
dd if=out/program.bin of=/dev/sdX bs=512 seek=8192 conv=notrunc
```
