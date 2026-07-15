# ============================================================================
# Everything is derived from config.mk (ISA/extensions/priv/tool).
# Change settings there, not here.
#
# Targets:
#   make build      verilates cosim_top + sim_cosim.cpp, links libriscv/libfesvr
#   make gen        generates a random program (SEED, N, CLASS)
#   make run        build + gen + firmware + lockstep execution on one program
#   make regress    generates and executes N programs (SEED 0..N-1)
#   make genseed    starts a new generator campaign
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

# A regression seed identifies one test inside a generator campaign. Keeping
# the campaign seed in a file makes repeated `make run SEED=N` deterministic;
# `make genseed` changes the campaign and therefore the whole test population.
GEN_SEED_FILE ?= .genseed
GEN_SEED      ?= $(shell test -f $(GEN_SEED_FILE) && cat $(GEN_SEED_FILE) || echo 0)

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

.PHONY: all build gen genseed firmware boot run run-notrace regress regress-one \
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
	$(PYTHON) gen/rvgen.py --seed $(SEED) --gen-seed $(GEN_SEED) \
		--n $(N) --class $(CLASS) \
		$(GEN_FLAGS) --out $(PROG)

# Start a new deterministic generator campaign. The selected value remains in
# effect until this target is called again and is intentionally not cleaned.
genseed:
	@new_seed=$$(od -An -N8 -tu8 /dev/urandom | tr -d ' '); \
	printf '%s\n' "$$new_seed" > $(GEN_SEED_FILE).tmp; \
	mv $(GEN_SEED_FILE).tmp $(GEN_SEED_FILE); \
	echo "=== [COSIM] new generator campaign: GEN_SEED=$$new_seed ==="


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
	@echo "=== [COSIM] Running lockstep (GEN_SEED=$(GEN_SEED) SEED=$(SEED)) ==="
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
COVFILE = $(COVDIR)/regress.cov
COVLOCK = $(COVDIR)/.regress.cov.lock
RTDIR  = $(OUT)/rtt

# Run ONE seed in an isolated workdir
regress-one:
	@d=$(RTDIR)/seed$(SEED); mkdir -p $$d $(COVDIR); \
	$(PYTHON) gen/rvgen.py --seed $(SEED) --gen-seed $(GEN_SEED) \
		--n $(PROG_LEN) --class $(CLASS) $(GEN_FLAGS) \
		--out $$d/prog.c --cov-out - --quiet \
		| flock $(COVLOCK) sh -c 'awk '\''{ total[$$1] += $$2 } END { for (op in total) print op, total[op] }'\'' $(COVFILE) - | sort > $(COVFILE).tmp && mv $(COVFILE).tmp $(COVFILE)'; \
	$(RISCV_GCC) $(MARCH_FLAGS) -fno-use-cxa-atexit -fno-exceptions -nostartfiles \
		-O2 -ffreestanding -T $(LINK_USER) $(CRT0) $$d/prog.c -o $$d/fw.elf 2>/dev/null; \
	if ./obj_dir/Vcosim_top +firmware=$$d/fw.elf +boot=$(OUT)/boot.elf +notrace \
		$(if $(filter-out 0,$(MAX_RETIRE)),+max_retire=$(MAX_RETIRE),) \
		>$$d/log 2>&1; then \
		echo "seed $(SEED) : PASS"; \
		rm -rf $$d; \
	else \
		echo "seed $(SEED) : FAIL (log: $$d/log, rerun: make run SEED=$(SEED) GEN_SEED=$(GEN_SEED))"; \
		exit 1; \
	fi

# Parallel sweep of seeds 0..N-1. Each seed reuses the same N for its program
# length; override per-seed length is not needed since N controls both here.
regress: build boot
	@echo "=== [COSIM] regression campaign GEN_SEED=$(GEN_SEED) ==="; \
	rm -rf $(RTDIR) $(COVDIR); mkdir -p $(RTDIR) $(COVDIR); : > $(COVFILE); \
	seq 0 $$(($(N)-1)) \
	| xargs -P $(JOBS) -I{} $(MAKE) --no-print-directory regress-one SEED={} \
		GEN_SEED=$(GEN_SEED) PROG_LEN=$(PROG_LEN) \
	| tee $(OUT)/regress.log; \
	pass=$$(grep -c ': PASS' $(OUT)/regress.log); \
	fail=$$(grep -c ': FAIL' $(OUT)/regress.log); \
	echo "=== [COSIM] regress: $$pass PASS, $$fail FAIL (JOBS=$(JOBS)) ==="; \
	grep ': FAIL' $(OUT)/regress.log || true; \
	test $$fail -eq 0


# ==============================================================================
# ---- Coverage ----------------------------------------------------------------
# ==============================================================================

# Layer A: aggregate the generator opcode histograms produced by a regression
# sweep into one union histogram. `regress` merges under a lock, so it does
# not create one coverage sidecar per seed.
coverage-report:
	@cat $(COVFILE) 2>/dev/null \
	| awk '{c[$$1]+=$$2} END{for(o in c) printf "%-12s %d\n", o, c[o]}' \
	| sort -k2 -nr
	@echo "=== distinct mnemonics exercised: $$(cat $(COVFILE) 2>/dev/null | awk '{print $$1}' | sort -u | wc -l) ==="

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
	@echo "GEN_SEED    : $(GEN_SEED)"
	@echo "GEN_SEED_FILE: $(GEN_SEED_FILE)"
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
