#!/bin/bash

WAVE=$1
TRACE=$2

# Move to Embench-IoT directory
cd ../../../sw/benchmark/Embench-IoT

make clean
make sim

# Collect each benchmark ELF: out/sim/benchname/benchname.elf
ELF_PATHS=( $(find out/sim -mindepth 2 -maxdepth 2 -name "*.elf" | sort) )

# Back to simulation environment
cd ../../../tb/verilator

mkdir -p logs/embench

printf "\n%-20s %-15s %-8s\n" "BENCHMARK" "CYCLES" "RESULT"
printf "%-20s %-15s %-8s\n" "---------" "------" "------"

# Do a run for each benchmark
for ELF in "${ELF_PATHS[@]}"; do
    BENCH_NAME=$(basename "$ELF" .elf)
    LOG_FILE="logs/embench/${BENCH_NAME}.log"

    make run \
        DDR=../../sw/benchmark/Embench-IoT/$ELF \
        BOOT=../../sw/benchmark/Embench-IoT/out/sim/boot_rom.elf \
        WAVE=$WAVE \
        TRACE=$TRACE \
        > "$LOG_FILE" 2>&1

    CYCLES=$(grep -o "EMBENCH_CYCLES=[0-9]*" "$LOG_FILE" | tail -1 | cut -d= -f2)

    if grep -q "\[ZTB\] PASS" "$LOG_FILE"; then
        RESULT="PASS"
    else
        RESULT="FAIL"
    fi

    if [ -z "$CYCLES" ]; then
        CYCLES="N/A"
    fi

    printf "%-20s %-15s %-8s\n" "$BENCH_NAME" "$CYCLES" "$RESULT"
done

echo ""
echo "[DONE]"
echo "Full logs saved in tb/verilator/logs/embench/"