#!/bin/bash
# Create a new block from the template
# Usage: ./scripts/new_block.sh my_block

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <block_name>"
    echo "Example: $0 my_adc"
    exit 1
fi

BLOCK_NAME=$1
BLOCK_DIR="blocks/$BLOCK_NAME"

if [ -d "$BLOCK_DIR" ]; then
    echo "Error: block '$BLOCK_NAME' already exists at $BLOCK_DIR"
    exit 1
fi

cp -r blocks/_template "$BLOCK_DIR"

echo "Created $BLOCK_DIR/"
echo ""
echo "Next steps:"
echo "  1. Edit $BLOCK_DIR/rtl/dut_wrapper.sv — instantiate your peripheral"
echo "  2. Edit $BLOCK_DIR/sw/firmware.c — write your test"
echo "  3. Add IO_BASE mapping in Makefile for '$BLOCK_NAME'"
echo "  4. Run: make BLOCK=$BLOCK_NAME run"
