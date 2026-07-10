#!/bin/bash

set -e 

usage() {
    echo "Error, usage example: ./run_coremark.sh [N iters] [Wave 0/1] [Trace 0/1]"
    echo ""
    echo "Example: ./run_coremark.sh 1000 0 1"
    exit 1
}

# Check number of arguments
if [ "$#" -ne 3 ]; then
    usage
fi

SIM_ITERS="$1"
WAVE="$2"
TRACE="$3"

# Check SIM_ITERS is a positive integer
if ! [[ "$SIM_ITERS" =~ ^[0-9]+$ ]]; then
    echo "Error: N iters must be an integer."
    usage
fi

# Check WAVE is 0 or 1
if ! [[ "$WAVE" =~ ^[01]$ ]]; then
    echo "Error: Wave must be 0 or 1."
    usage
fi

# Check TRACE is 0 or 1
if ! [[ "$TRACE" =~ ^[01]$ ]]; then
    echo "Error: Trace must be 0 or 1."
    usage
fi


# Move to coremark directory
cd ../../../sw/benchmark/CoreMark

make clean
make sim SIM_ITERATIONS=$SIM_ITERS

# Back to simulation environment
cd ../../../tb/verilator

make run DDR=../../sw/benchmark/CoreMark/out/coremark_sim.elf BOOT=../../sw/benchmark/CoreMark/out/boot_rom.elf WAVE=$WAVE TRACE=$TRACE

echo "[DONE]"