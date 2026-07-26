#!/usr/bin/env bash

# Usage:
#   write_sd.sh <device_or_image> <app.bin>
#
# Example:
#   sudo write_sd.sh /dev/sdb out/coremark_app.bin

set -e

DEV=$1
APP=$2

if [ -z "$DEV" ] || [ -z "$APP" ]; then
    echo "Usage: $0 <device_or_image> <coremark_app.bin>" >&2
    exit 1
fi

if [ ! -f "$APP" ]; then
    echo "ERROR: $APP not found" >&2
    exit 1
fi

echo "Writing CoreMark app to sector 0x2000 (8192) of $DEV ..."
dd if="$APP" of="$DEV" bs=512 seek=8192 conv=notrunc

echo "Done."
