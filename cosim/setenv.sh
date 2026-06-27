#!/usr/bin/env bash
# ============================================================================
# ZenithSoC cosimulation environment
#
# Usage:
#   source cosim/setenv.sh
#   cd cosim
#   make clean
#   make build
#   make run-notrace SEED=0 MAX_RETIRE=1000
# ============================================================================

# Stop only if this script is executed directly, not when sourced
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    echo "Please source this file:"
    echo "  source cosim/env.sh"
    exit 1
fi

# Directory of this env.sh file
export COSIM_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PROJECT_ROOT="$(cd "$COSIM_DIR/.." && pwd)"

export CC="${CC:-gcc}"
export CXX="${CXX:-g++}"

export SPIKE_SRC="$HOME/riscv-isa-sim"
export SPIKE_BUILD="$HOME/riscv-isa-sim/build"

export SPIKE_INC="$SPIKE_SRC"
export SPIKE_LIB="$SPIKE_BUILD"

# Needed at runtime if linking against .so instead of .a
export LD_LIBRARY_PATH="$SPIKE_LIB:${LD_LIBRARY_PATH:-}"


echo "[cosim/env] PROJECT_ROOT=$PROJECT_ROOT"
echo "[cosim/env] COSIM_DIR=$COSIM_DIR"
echo "[cosim/env] verilator=$(command -v verilator || echo NOT_FOUND)"
echo "[cosim/env] riscv-g++=$(command -v riscv32-unknown-elf-g++ || echo NOT_FOUND)"
echo "[cosim/env] host g++=$(g++ --version | head -1)"
echo "[cosim/env] spike=$(command -v spike || echo NOT_FOUND)"
echo "[cosim/env] SPIKE_INC=$SPIKE_INC"
echo "[cosim/env] SPIKE_LIB=$SPIKE_LIB"