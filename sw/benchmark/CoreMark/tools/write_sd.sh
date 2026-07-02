#!/usr/bin/env bash

# Usage:
#   write_sd.sh <device_or_image> <bootloader.bin> <app.bin>
#
# Example:
#   sudo write_sd.sh /dev/sdb out/bootloader.bin out/coremark_app.bin
#
# The layout expected by boot_sd.cpp:
#   Sector 0x0000 (byte 0):          Boot ROM image (bootloader.bin)
#   Sector 0x2000 (byte 0x400000):   CoreMark app (coremark_app.bin)

set -e

DEV=$1
BOOT=$2
APP=$3

if [ -z "$DEV" ] || [ -z "$BOOT" ] || [ -z "$APP" ]; then
    echo "Usage: $0 <device_or_image> <bootloader.bin> <coremark_app.bin>" >&2
    exit 1
fi

if [ ! -f "$BOOT" ]; then
    echo "ERROR: $BOOT not found" >&2
    exit 1
fi

if [ ! -f "$APP" ]; then
    echo "ERROR: $APP not found" >&2
    exit 1
fi

echo "Writing bootloader to sector 0 of $DEV ..."
dd if="$BOOT" of="$DEV" bs=512 seek=0 conv=notrunc

echo "Writing CoreMark app to sector 0x2000 (8192) of $DEV ..."
dd if="$APP" of="$DEV" bs=512 seek=8192 conv=notrunc

echo "Done."