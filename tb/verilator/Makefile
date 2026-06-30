# ======================================================================
# Standalone Verilator testbench for the full ZenithSoC.
#
# Prerequisite: source the cosim environment first (Verilator 5.026, GCC 14.2,
# Spike libs)
#     source ../../cosim/setenv.sh
#
# Targets:
#   make build                    verilate zenith_tb_top + sim_main.cpp
#   make run DDR=fw.elf [BOOT=b.elf]   build + run a firmware
#   make wave                     open the latest waveform
#   make info                     print resolved configuration
#   make clean
#
# Run options:
#   DDR=path.elf        firmware ELF loaded into DDR/ROM          (required)
#   BOOT=path.elf       optional boot-stub ELF loaded into the ROM
#   WAVE=1              dump out/zenith.fst                       (default 0)
#   TRACE=1             print the per-instruction trace           (default 1)
#   MAX_CYCLES=N        stop after N cycles (0 = run until tohost) (default 0)
# ======================================================================

SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

# --- Sources -----------------------------------------------------------
# Absolute paths: the generated obj_dir/*.mk compiles from inside obj_dir, so
# relative -I/-y would resolve against the wrong directory.
ZENITH_HW := $(abspath ../../hw)
COSIM_SIM := $(abspath ../../cosim/sim)
TB_F      = zenith_tb.f
SIM_SRC   = sim_main.cpp

OUT    = out
LOGDIR = logs

# --- ISA (must match the toolchain used to build the firmware) ---------
# Mirrors cosim/config.mk; override on the command line if needed.
ISA ?= rv32im_zfinx_zba_zbs_zicsr

# --- Run parameters ----------------------------------------------------
DDR        ?=
BOOT       ?=
WAVE       ?= 0
TRACE      ?= 1
MAX_CYCLES ?= 0

# --- Tools -------------------------------------------------------------
VERILATOR ?= verilator

SPIKE_INC ?= $(HOME)/riscv-isa-sim
SPIKE_LIB ?= $(HOME)/riscv-isa-sim/build
SIM_DIR   := $(abspath .)

VLATOR_DEFS = -DCOSIM_ISA=\\\"$(ISA)\\\"

# --- Verilator flags ---------------------------------------------------
VFLAGS = --cc --exe --trace-fst --trace-structs --timing \
    --top-module zenith_tb_top \
    -DVERILATOR \
    -D_DDR_BEHAVIORAL_ \
    -Wno-fatal \
    -Wno-ENUMVALUE \
    -Wno-BLKANDNBLK \
    -Wno-WIDTHTRUNC \
    -Wno-WIDTHEXPAND \
    -y $(ZENITH_HW) \
    -I$(ZENITH_HW) \
    -Mdir obj_dir \
    -CFLAGS "-std=c++20 -O2 -I$(SPIKE_INC) -I$(COSIM_SIM) -I$(SIM_DIR) $(VLATOR_DEFS)" \
    -LDFLAGS "-L$(SPIKE_LIB) -lriscv -lfesvr -lpthread -ldl"

.PHONY: all build run wave info clean

all: build

# --- Verilate + compile ------------------------------------------------
build:
	@mkdir -p $(LOGDIR) $(OUT)
	@echo "=== [ZTB] Verilating ZenithSoC (ISA=$(ISA)) ==="
	$(VERILATOR) $(VFLAGS) -f $(TB_F) $(SIM_SRC) 2>&1 | tee $(LOGDIR)/verilator.log
	$(MAKE) -C obj_dir -f Vzenith_tb_top.mk -j$$(nproc) 2>&1 | tee $(LOGDIR)/build.log
	@echo "=== [ZTB] build done -> obj_dir/Vzenith_tb_top ==="

# --- Run ---------------------------------------------------------------
run: build
	@mkdir -p $(OUT)
	@test -n "$(DDR)" || { echo "ERROR: pass a firmware, e.g. make run DDR=fw.elf"; exit 1; }
	@echo "=== [ZTB] Running (DDR=$(DDR) BOOT=$(BOOT)) ==="
	./obj_dir/Vzenith_tb_top \
		+firmware=$(DDR) \
		$(if $(BOOT),+boot=$(BOOT),) \
		$(if $(filter 1,$(WAVE)),+wave,) \
		$(if $(filter 0,$(TRACE)),+notrace,) \
		$(if $(filter-out 0,$(MAX_CYCLES)),+max_cycles=$(MAX_CYCLES),) \
		2>&1 | tee $(LOGDIR)/run.log

# --- Waveform ----------------------------------------------------------
wave:
	fst2vcd $(OUT)/zenith.fst > $(OUT)/zenith.vcd
	env -u LD_LIBRARY_PATH -u GTK_PATH -u GIO_EXTRA_MODULES -u LD_PRELOAD \
		gtkwave $(OUT)/zenith.vcd

# --- Info --------------------------------------------------------------
info:
	@echo "ISA         : $(ISA)"
	@echo "VERILATOR  : $(VERILATOR) ($$($(VERILATOR) --version 2>/dev/null))"
	@echo "SPIKE_INC  : $(SPIKE_INC)"
	@echo "SPIKE_LIB  : $(SPIKE_LIB)"
	@echo "DDR        : $(DDR)"
	@echo "BOOT       : $(BOOT)"
	@echo "WAVE/TRACE : $(WAVE)/$(TRACE)   MAX_CYCLES=$(MAX_CYCLES)"

clean:
	rm -rf obj_dir $(OUT) $(LOGDIR)