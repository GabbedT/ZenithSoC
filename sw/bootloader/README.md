# Shared ZenithSoC SD bootloader

This directory contains the generic boot program used by applications that
are deployed as an image on the SD card. It initializes the card, copies a
contiguous application image to DDR, publishes the writes, and jumps to the
configured entry address.

The boot ROM is deliberately kept independent of the application. The image
size is derived from `APP_BIN`, while the deployment-specific values can be
overridden on the command line:

```bash
make -C sw/bootloader \
  APP_BIN=/absolute/path/to/program.bin \
  BOOT_ELF=/absolute/path/to/boot.elf \
  BOOT_HEX=/absolute/path/to/boot.hex \
  BOOT_IMAGE_BLOCK=0x2000 \
  BOOT_LOAD_ADDRESS=0x80000000
```

The same block value can be used when preparing the card with
`tools/write_sd.sh`:

```bash
sudo tools/write_sd.sh /dev/sdX /absolute/path/to/program.bin 0x2000
```

The main configuration variables are `BOOT_IMAGE_BLOCK`,
`BOOT_IMAGE_BLOCKS`, `BOOT_LOAD_ADDRESS`, `BOOT_ENTRY_ADDRESS`,
`BOOT_UART_INDEX`, `BOOT_BAUD_RATE`, `BOOT_DDR_WAIT_MS`, `BOOT_SD_CLOCK`,
`BOOT_SD_BUS`, `BOOT_LOGGING`, and `BOOT_DUMP_WORDS`. `BOOT_DDR_WAIT_MS`
defaults to 10 ms and delays SD initialization so MIG can finish DDR2
calibration. `BOOT_DUMP_WORDS` controls the post-writeback DDR readback dump
and defaults to zero; the VGA example enables a 16-word dump. Defaults are
also documented in `boot_config.h`.

The boot entry installs a machine-mode trap handler in the boot ROM before
calling C code. If the loaded application traps, the handler reports `mcause`
and `mepc` over the boot UART and halts instead of falling through the reset
vector and apparently rebooting. This core does not implement `mtval`, so the
handler deliberately does not access it.

The first SD block is expressed in 512-byte blocks. The loader supports both
SDHC/SDXC block addressing and SDSC byte addressing, matching the shared SD
driver. The boot RAM layout is the fixed 16 KiB ZenithSoC layout: the linker
reserves its upper 2 KiB for the pre-DDR stack.

The bootloader also supplies the freestanding `memcpy` and `memset` routines
needed by the SD driver, so an application does not need to contribute its
own runtime object when using this component.
