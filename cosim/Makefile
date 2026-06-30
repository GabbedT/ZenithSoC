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

# --- Program generation / run parameters ------------------------------------
SEED        ?= 0
N           ?= 2000      			 # gen/run: instructions per program. regress: seed count
CLASS       ?= arith,mem,branch,ctrl # arith,mem,branch,ctrl,float
MAX_RETIRE  ?= 0         			 # 0 = unlimited

# Instructions per generated program during a regression sweep. Kept separate
# from N so `make regress N=64` means "64 seeds", each a PROG_LEN-instr program.
PROG_LEN    ?= 2000


# --- Tools -------------------------------------------------------------------
VERILATOR     ?= verilator
RISCV_GCC     ?= riscv32-unknown-elf-g++
RISCV_OBJDUMP ?= riscv32-unknown-elf-objdump

SPIKE_INC ?= /home/gabriele/riscv-isa-sim
SPIKE_LIB ?= /home/gabriele/riscv-isa-sim/build
SIM_DIR   := $(abspath sim)

# Passed to verilator C++ simulator
COSIM_DEFS = -DCOSIM_ISA=\\\"$(ISA)\\\" \
             -DCOSIM_PRIV=\\\"$(s_PRIV)\\\"

# ---- Verilator flags ---------------------------------------------------------
VFLAGS = --cc --exe --trace-fst --trace-structs --timing \
         --top-module cosim_top \
         -Wno-fatal \
         -Wno-ENUMVALUE \
         -y $(ZENITH_HW)/hw \
         -I$(ZENITH_HW)/hw \
         -Mdir obj_dir \
         -CFLAGS "-std=c++20 -O2 -I$(SPIKE_INC) -I$(SIM_DIR) $(COSIM_DEFS)" \
         -LDFLAGS "-L$(SPIKE_LIB) -lriscv -lfesvr -lpthread -ldl"


# Structural (line + toggle) coverage is opt-in: --coverage slows the model and
# is incompatible with the fast default flow, so gather it with a dedicated
# `make build COVERAGE=1` + one long run, never in the regression path.
COVERAGE ?= 0
ifeq ($(strip $(COVERAGE)),1)
    VFLAGS += --coverage
endif

# Parallel regression fan-out width (one verilator run per seed, JOBS at a time).
JOBS ?= $(shell nproc)

.PHONY: all build gen firmware boot run run-notrace regress regress-one \
        coverage-report verilator-coverage info wave clean

all: build


# ===========================================================================
# --- Verilate + compile ----------------------------------------------------
# ===========================================================================

build:
	@mkdir -p $(LOGDIR)
	@echo "=== [COSIM] Verilating (ISA=$(ISA) priv=$(s_PRIV)) ==="
	$(VERILATOR) $(VFLAGS) rtl/cosim_public.vlt -F $(COSIM_F) $(SIM_SRC) 2>&1 | tee $(VERILATOR_LOG)
	$(MAKE) -C obj_dir -f Vcosim_top.mk -j$$(nproc) 2>&1 | tee $(BUILD_LOG)
	@echo "=== [COSIM] build done -> obj_dir/Vcosim_top ==="
	@echo "=== [COSIM] verilator log -> $(VERILATOR_LOG) ==="
	@echo "=== [COSIM] build log     -> $(BUILD_LOG) ==="


# ==========================================================================
# --- Generate random program ----------------------------------------------
# ==========================================================================

gen:
	@mkdir -p tests
	$(PYTHON) gen/rvgen.py --seed $(SEED) --n $(N) --class $(CLASS) \
		$(GEN_FLAGS) --out $(PROG)


# ==========================================================================
# --- Compile user firmware + ROM stub -------------------------------------
# ==========================================================================

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


# ==========================================================================
# --- Run lockstep execution on a program ----------------------------------
# ==========================================================================

run: build gen firmware boot
	@echo "=== [COSIM] Running lockstep (SEED=$(SEED)) ==="
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),)

run-notrace: build gen firmware boot
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf +notrace \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),)

run-notrace: build gen firmware boot


# ==============================================================================
# ---- Regression: N programs with seeds 0..N-1, JOBS at a time ----------------
# ==============================================================================

COVDIR = $(OUT)/cov
RTDIR  = $(OUT)/rtt

# Run ONE seed in an isolated workdir
regress-one:
	@d=$(RTDIR)/seed$(SEED); mkdir -p $$d $(COVDIR); \
	$(PYTHON) gen/rvgen.py --seed $(SEED) --n $(PROG_LEN) --class $(CLASS) $(GEN_FLAGS) \
		--out $$d/prog.c --cov-out $(COVDIR)/seed$(SEED).cov >/dev/null; \
	$(RISCV_GCC) $(MARCH_FLAGS) -fno-use-cxa-atexit -fno-exceptions -nostartfiles \
		-O2 -ffreestanding -T $(LINK_USER) $(CRT0) $$d/prog.c -o $$d/fw.elf 2>/dev/null; \
	if ./obj_dir/Vcosim_top +firmware=$$d/fw.elf +boot=$(OUT)/boot.elf +notrace \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),) \
		>$$d/log 2>&1; then \
		echo "seed $(SEED) : PASS"; \
	else \
		echo "seed $(SEED) : FAIL (rerun: make run SEED=$(SEED))"; exit 1; \
	fi

# Parallel sweep of seeds 0..N-1. Each seed reuses the same N for its program
# length; override per-seed length is not needed since N controls both here.
regress: build boot
	@rm -rf $(RTDIR) $(COVDIR); mkdir -p $(RTDIR) $(COVDIR); \
	seq 0 $$(($(N)-1)) \
	| xargs -P $(JOBS) -I{} $(MAKE) --no-print-directory regress-one SEED={} PROG_LEN=$(PROG_LEN) \
	| tee $(OUT)/regress.log; \
	pass=$$(grep -c ': PASS' $(OUT)/regress.log); \
	fail=$$(grep -c ': FAIL' $(OUT)/regress.log); \
	echo "=== [COSIM] regress: $$pass PASS, $$fail FAIL (JOBS=$(JOBS)) ==="; \
	grep ': FAIL' $(OUT)/regress.log || true; \
	test $$fail -eq 0


# ==============================================================================
# ---- Coverage ----------------------------------------------------------------
# ==============================================================================

# Layer A: aggregate the generator opcode histograms (--cov-out sidecars)
# produced by a regression sweep into a single union histogram.
coverage-report:
	@cat $(COVDIR)/*.cov 2>/dev/null \
	| awk '{c[$$1]+=$$2} END{for(o in c) printf "%-12s %d\n", o, c[o]}' \
	| sort -k2 -nr
	@echo "=== distinct mnemonics exercised: $$(cat $(COVDIR)/*.cov 2>/dev/null | awk '{print $$1}' | sort -u | wc -l) ==="

# Layer B: annotate Verilator structural coverage. Requires a COVERAGE=1 build
# followed by a run (which writes coverage.dat in the run cwd).
verilator-coverage:
	verilator_coverage --annotate $(OUT)/cov_annotated $(OUT)/coverage.dat \
	|| verilator_coverage --annotate $(OUT)/cov_annotated coverage.dat
	@echo "=== annotated coverage -> $(OUT)/cov_annotated ==="


# ===========================================================================
# --- Configuration info ----------------------------------------------------
# ===========================================================================

info:
	@echo "ISA         : $(ISA)"
	@echo "PRIV        : $(s_PRIV)"
	@echo "MARCH_FLAGS : $(MARCH_FLAGS)"
	@echo "GEN_FLAGS   : $(GEN_FLAGS)"
	@echo "VERILATOR   : $(VERILATOR)"
	@echo "RISCV_GCC   : $(RISCV_GCC)"
	@echo "SPIKE_DIR   : $(SPIKE_DIR)"
	@echo "PYTHON      : $(PYTHON)"


# ===========================================================================
# --- Waveform --------------------------------------------------------------
# ===========================================================================

wave:
	fst2vcd $(OUT)/cosim.fst > $(OUT)/cosim.vcd
	env -u LD_LIBRARY_PATH -u GTK_PATH -u GIO_EXTRA_MODULES -u LD_PRELOAD \
		gtkwave $(OUT)/cosim.vcd

clean:
	rm -rf obj_dir $(OUT) tests/prog.c