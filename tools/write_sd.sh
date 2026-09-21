#!/usr/bin/env bash

# Usage:
#   write_sd.sh <device_or_image> <app.bin> [start_block]
#
# Examples:
#   sudo write_sd.sh /dev/sdb out/coremark_app.bin
#   sudo write_sd.sh /dev/sdb out/program.bin 0x2000

set -euo pipefail

DEV=${1:-}
APP=${2:-}
BLOCK_ARG=${3:-${SD_BLOCK:-0x2000}}
BLOCK=$((BLOCK_ARG))

if [ -z "$DEV" ] || [ -z "$APP" ]; then
    echo "Usage: $0 <device_or_image> <app.bin> [start_block]" >&2
    exit 1
fi

if [ ! -f "$APP" ]; then
    echo "ERROR: $APP not found" >&2
    exit 1
fi

# Paths below /dev must already exist and identify a block device.  This also
# catches an unexpanded documentation placeholder such as /dev/sdX before dd
# creates a regular file with that name.  Ordinary files elsewhere remain
# supported for building/testing disk images.
if [[ "$DEV" == /dev/* ]] && [ ! -b "$DEV" ]; then
    echo "ERROR: $DEV is not a block device; replace /dev/sdX with the actual SD device" >&2
    exit 1
fi

# The bootloader addresses absolute sectors on the card.  Writing to a
# partition (for example /dev/sdb1) would place the image relative to the
# partition start and leave the old payload at absolute block 0x2000.
if [ -b "$DEV" ] && command -v lsblk >/dev/null; then
    DEV_TYPE=$(lsblk -ndo TYPE "$DEV" 2>/dev/null || true)
    if [ "$DEV_TYPE" = "part" ]; then
        PARENT=$(lsblk -ndo PKNAME "$DEV" 2>/dev/null || true)
        echo "ERROR: $DEV is a partition; use the whole device /dev/$PARENT" >&2
        exit 1
    fi
fi

APP_SIZE=$(stat -c%s "$APP")
APP_BLOCKS=$(((APP_SIZE + 511) / 512))

echo "Writing application to block $BLOCK_ARG ($BLOCK) of $DEV ..."
dd if="$APP" of="$DEV" bs=512 seek="$BLOCK" conv=notrunc status=progress
sync "$DEV"

echo "Verifying $APP_SIZE bytes ($APP_BLOCKS blocks) from $DEV ..."
if ! dd if="$DEV" bs=512 skip="$BLOCK" count="$APP_BLOCKS" status=none |
    cmp -n "$APP_SIZE" "$APP" -; then
    echo "ERROR: SD read-back differs from $APP" >&2
    exit 1
fi

echo "Done; SD read-back matches $APP."
