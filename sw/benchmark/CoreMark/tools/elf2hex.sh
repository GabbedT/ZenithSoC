#!/usr/bin/env bash

# Convert an ELF to a raw binary hex dump (one byte per line, no address).
# Usage: elf2hex.sh <input.elf> <output.hex>

set -e

ELF=$1
HEX=$2

if [ -z "$ELF" ] || [ -z "$HEX" ]; then
    echo "Usage: $0 <input.elf> <output.hex>" >&2
    exit 1
fi

TMP=$(mktemp --suffix=.bin)

riscv32-unknown-elf-objcopy -O binary "$ELF" "$TMP"
xxd -p -c 1 -g 1 "$TMP" > "$HEX"
rm -f "$TMP"

echo "Wrote $HEX"