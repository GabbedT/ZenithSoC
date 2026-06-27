# ============================================================================
# Everything is derived from config.mk (ISA/extensions/priv/tool).
# Change settings there, not here.
#
# Targets:
#   make build      verilates cosim_top + sim_cosim.cpp, links libriscv/libfesvr
#   make gen        generates a random program (SEED, N, CLASS)
#   make run        build + gen + firmware + lockstep execution on one program
#   make regress    generates and executes N programs (SEED 0..N-1)
#   make info       prints resolved ISA/priv/tool configuration
#   make wave       opens the waveform from the latest run
#   make clean
# ============================================================================

include config.mk

SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

LOGDIR = logs
VERILATOR_LOG = $(LOGDIR)/verilator.log
BUILD_LOG = $(LOGDIR)/build.log
RUN_LOG = $(LOGDIR)/run.log

# --- Sources ---------------------------------------------------------------
ZENITH_HW = ..
COSIM_F   = rtl/cosim.f
SIM_SRC   = sim/cosim.cpp

# --- Software --------------------------------------------------------------
CRT0      = sw/crt0_user.s
LINK_USER = sw/link_user.ld
BOOT_S    = sw/boot_rom.s
BOOT_LD   = sw/boot_rom.ld

OUT       = out
PROG      ?= tests/prog.c

# --- Program generation / run parameters ----------------------------------
SEED       ?= 0
N          ?= 2000
CLASS      ?= arith,mem,branch
MAX_RETIRE ?= 0          # 0 = unlimited

# --- Tools ---
VERILATOR     ?= verilator
RISCV_GCC     ?= riscv32-unknown-elf-g++
RISCV_OBJDUMP ?= riscv32-unknown-elf-objdump

SPIKE_INC ?= /home/gabriele/riscv-isa-sim
SPIKE_LIB ?= /home/gabriele/riscv-isa-sim/build
SIM_DIR   := $(abspath sim)

# Passed to verilator C++ simulator
COSIM_DEFS = -DCOSIM_ISA=\\\"$(ISA)\\\" \
             -DCOSIM_PRIV=\\\"$(s_PRIV)\\\"

# --- Verilator flags -------------------------------------------------------
VFLAGS = --cc --exe --trace-fst --trace-structs --timing \
          --top-module cosim_top \
          -Wno-fatal \
          -Wno-ENUMVALUE \
          -y $(ZENITH_HW)/hw \
          -I$(ZENITH_HW)/hw \
          -Mdir obj_dir \
		  -CFLAGS "-std=c++20 -O2 -I$(SPIKE_INC) -I$(SIM_DIR) $(COSIM_DEFS)" \
		  -LDFLAGS "-L$(SPIKE_LIB) -lriscv -lfesvr -lpthread -ldl" \

.PHONY: all build gen firmware boot run regress info wave clean

all: build

# --- Verilate + compile ----------------------------------------------------
build:
	@mkdir -p $(LOGDIR)
	@echo "=== [COSIM] Verilating (ISA=$(ISA) priv=$(s_PRIV)) ==="
	$(VERILATOR) $(VFLAGS) rtl/cosim_public.vlt -F $(COSIM_F) $(SIM_SRC) 2>&1 | tee $(VERILATOR_LOG)
	$(MAKE) -C obj_dir -f Vcosim_top.mk -j$$(nproc) 2>&1 | tee $(BUILD_LOG)
	@echo "=== [COSIM] build done -> obj_dir/Vcosim_top ==="
	@echo "=== [COSIM] verilator log -> $(VERILATOR_LOG) ==="
	@echo "=== [COSIM] build log     -> $(BUILD_LOG) ==="

# --- Generate random program ----------------------------------------------
gen:
	@mkdir -p tests
	$(PYTHON) gen/rvgen.py --seed $(SEED) --n $(N) --class $(CLASS) \
		$(GEN_FLAGS) --out $(PROG)

# --- Compile user firmware + ROM stub -------------------------------------
firmware:
	@mkdir -p $(OUT)
	@echo "=== [COSIM] Compiling user firmware ($(MARCH_FLAGS)) ==="
	$(RISCV_GCC) $(MARCH_FLAGS) -fno-use-cxa-atexit -fno-exceptions -nostartfiles \
		-O2 -ffreestanding \
		-T $(LINK_USER) $(CRT0) $(PROG) \
		-o $(OUT)/firmware.elf
	$(RISCV_OBJDUMP) -d $(OUT)/firmware.elf > $(OUT)/firmware.dis
	@echo "=== [COSIM] firmware -> $(OUT)/firmware.elf ==="

boot:
	@mkdir -p $(OUT)
	$(RISCV_GCC) $(MARCH_FLAGS) -nostartfiles -O2 \
		-T $(BOOT_LD) $(BOOT_S) -o $(OUT)/boot.elf
	@echo "=== [COSIM] boot stub -> $(OUT)/boot.elf ==="

# --- Run lockstep execution on a program ----------------------------------
run: build gen firmware boot
	@echo "=== [COSIM] Running lockstep (SEED=$(SEED)) ==="
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),)

run-notrace: build gen firmware boot
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf +notrace \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),)

# --- Regression: N programs with seeds 0..N-1 -----------------------------
regress: build boot
	@pass=0; fail=0; \
	for s in $$(seq 0 $$(($(N)-1))); do \
		$(PYTHON) gen/rvgen.py --seed $$s --n 2000 --class $(CLASS) $(GEN_FLAGS) --out $(PROG) >/dev/null; \
		$(RISCV_GCC) $(MARCH_FLAGS) -fno-use-cxa-atexit -fno-exceptions -nostartfiles -O2 -ffreestanding \
			-T $(LINK_USER) $(CRT0) $(PROG) -o $(OUT)/firmware.elf 2>/dev/null; \
		if ./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf +notrace >/dev/null 2>&1; then \
			pass=$$((pass+1)); echo "seed $$s : PASS"; \
		else \
			fail=$$((fail+1)); echo "seed $$s : FAIL (rerun: make run SEED=$$s)"; \
		fi; \
	done; \
	echo "=== [COSIM] regress: $$pass PASS, $$fail FAIL ==="

# --- Configuration info ----------------------------------------------------
info:
	@echo "ISA         : $(ISA)"
	@echo "PRIV        : $(s_PRIV)"
	@echo "MARCH_FLAGS : $(MARCH_FLAGS)"
	@echo "GEN_FLAGS   : $(GEN_FLAGS)"
	@echo "VERILATOR   : $(VERILATOR)"
	@echo "RISCV_GCC   : $(RISCV_GCC)"
	@echo "SPIKE_DIR   : $(SPIKE_DIR)"
	@echo "PYTHON      : $(PYTHON)"

# --- Waveform --------------------------------------------------------------
wave:
	fst2vcd $(OUT)/cosim.fst > $(OUT)/cosim.vcd
	env -u LD_LIBRARY_PATH -u GTK_PATH -u GIO_EXTRA_MODULES -u LD_PRELOAD \
		gtkwave $(OUT)/cosim.vcd

clean:
	rm -rf obj_dir $(OUT) tests/prog.c