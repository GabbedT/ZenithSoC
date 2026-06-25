# ====================================================================
# Usage:
#   source cosim/env.sh
#   cd cosim && make build && make run SEED=42
# ====================================================================

# --- Host GCC 14.2.0 (Verilated harness compiler + libstdc++ runtime) ---
export HOST_GCC=
export PATH=
export LD_LIBRARY_PATH=
export CC=gcc
export CXX=g++

# --- riscv32 cross toolchain (firmware target) -------------------------------
export PATH=

# --- Verilator 5.026 (ADI module) --------------------------------------------
export VERILATOR_ROOT=
export PATH=$VERILATOR_ROOT/bin:$PATH

# --- dtc (only needed when rebuilding Spike) ---------------------------------
export PATH=

# --- Spike as a library ------------------------------------------------------
export SPIKE_DIR=
export LD_LIBRARY_PATH=

echo "[cosim/env] verilator=$(which verilator) riscv-g++=$(which riscv32-unknown-elf-g++)"
echo "[cosim/env] host g++=$(g++ --version | head -1) SPIKE_DIR=$SPIKE_DIR"