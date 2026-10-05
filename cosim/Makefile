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
TRACE     ?= $(OUT)/cosim.fst

# --- Program generation / run parameters ------------------------------------
SEED        ?= 0
N           ?= 2000      # gen/run: blocks per program. regress: seed count
CLASS       ?= arith,mem,branch,ctrl,float # arith,mem,branch,ctrl,float,fence
MAX_RETIRE  ?= 1000000   # 0 = unlimited

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
SPIKE ?= $(SPIKE_LIB)/spike
CHECK_REFERENCE ?= 1
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

ASSERTIONS ?= 1
ifeq ($(strip $(ASSERTIONS)),1)
    VFLAGS += --assert -DSV_ASSERTION
endif

# Match the FPGA CPU/cache parameters when requested; retain the legacy test
# configuration by default. build always re-verilates when switching modes.
SOC_CONFIG ?= 0
ifeq ($(strip $(SOC_CONFIG)),1)
    VFLAGS += -DCOSIM_SOC_CONFIG
endif


# Structural coverage is optional; each regression worker writes its own file.
COVERAGE ?= 0
ifeq ($(strip $(COVERAGE)),1)
    VFLAGS += --coverage
endif

# Parallel regression fan-out width (one verilator run per seed, JOBS at a time).
JOBS ?= 4
BUILD_JOBS ?= 4
PROFILE ?= auto
CACHE_BYTES ?= $(if $(filter 1,$(SOC_CONFIG)),8192,4096)
LINE_BYTES ?= $(if $(filter 1,$(SOC_CONFIG)),32,16)
MEM_RANDOM ?= 1
MEM_SEED ?= $(shell $(PYTHON) regress.py --print-memory-seed --start-seed $(SEED) --gen-seed $(GEN_SEED))
MEM_LATENCY_MIN ?= 2
MEM_LATENCY_MAX ?= 16
MEM_STALL_PERCENT ?= 25
MAX_CYCLES ?= 20000000
TIMEOUT ?= 120
START_SEED ?= 0
GEN_OPTIONS = $(GEN_FLAGS) --profile $(PROFILE) --cache-bytes $(CACHE_BYTES) --line-bytes $(LINE_BYTES)
SIM_OPTIONS = +max_retire=$(MAX_RETIRE) +max_cycles=$(MAX_CYCLES) +line_bytes=$(LINE_BYTES) \
              +mem_random=$(MEM_RANDOM) +mem_latency_min=$(MEM_LATENCY_MIN) \
              +mem_latency_max=$(MEM_LATENCY_MAX) +mem_stall_percent=$(MEM_STALL_PERCENT)

# Firmware consumes generated source; serialize these prerequisites under make -j.
.NOTPARALLEL: run run-notrace run-case regress


.PHONY: all build gen genseed firmware boot run run-notrace regress regress-one \
        coverage-report verilator-coverage info wave clean replay test run-case

all: build


# ===========================================================================
# --- Verilate + compile ----------------------------------------------------
# ===========================================================================

build:
	@mkdir -p $(LOGDIR)
	@echo "=== [COSIM] Verilating (ISA=$(ISA) priv=$(s_PRIV)) ==="
	$(VERILATOR) $(VFLAGS) rtl/cosim_public.vlt -F $(COSIM_F) $(SIM_SRC) 2>&1 | tee $(VERILATOR_LOG)
	@rm -f obj_dir/cosim.o
	$(MAKE) -C obj_dir -f Vcosim_top.mk -j$(BUILD_JOBS) 2>&1 | tee $(BUILD_LOG)
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
		$(GEN_OPTIONS) --out $(PROG) --manifest tests/generator.json

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
		+trace=$(TRACE) +trace_insns \
		$(SIM_OPTIONS) +mem_seed=$(MEM_SEED) +coverage=$(OUT)/executed.cov 2>&1 | tee $(RUN_LOG)
	@test -s $(TRACE)
	@echo "=== [COSIM] run log -> $(RUN_LOG) ==="

run-notrace: build gen firmware boot
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf +notrace \
		$(SIM_OPTIONS) +mem_seed=$(MEM_SEED) +coverage=$(OUT)/executed.cov 2>&1 | tee $(RUN_LOG)
	@echo "=== [COSIM] run log -> $(RUN_LOG) ==="

CASE ?= load_x0
run-case: PROG = cases/$(CASE).c
run-case: build firmware boot
	./obj_dir/Vcosim_top +firmware=$(OUT)/firmware.elf +boot=$(OUT)/boot.elf +notrace \
		$(SIM_OPTIONS) +mem_seed=$(MEM_SEED) +coverage=$(OUT)/executed.cov 2>&1 | tee $(RUN_LOG)


# ==============================================================================
# ---- Regression: N programs with seeds 0..N-1, JOBS at a time ----------------
# ==============================================================================

COVDIR = $(OUT)/cov
COVFILE = $(COVDIR)/regress.cov
RTDIR = $(OUT)/rtt
REGRESS_ARGS = --gen-seed $(GEN_SEED) --length $(PROG_LEN) --classes $(CLASS) \
               --jobs $(JOBS) --timeout $(TIMEOUT) --cc "$(RISCV_GCC)" \
               --objdump "$(RISCV_OBJDUMP)" --cflags="$(MARCH_FLAGS)" \
               --gen-flags="$(GEN_OPTIONS)" --sim-flags="$(SIM_OPTIONS)" \
               $(if $(filter 1,$(CHECK_REFERENCE)),--spike "$(SPIKE)",) \
               --boot $(OUT)/boot.elf --out $(RTDIR) --coverage-dir $(COVDIR)

regress-one:
	$(PYTHON) regress.py $(REGRESS_ARGS) --seeds 1 --start-seed $(SEED)

regress: build boot
	$(PYTHON) regress.py $(REGRESS_ARGS) --seeds $(N) --start-seed $(START_SEED) 2>&1 | tee $(OUT)/regress.log

replay:
	$(PYTHON) regress.py --replay "$(RUN_DIR)" --timeout $(TIMEOUT) $(if $(filter 1,$(REPLAY_TRACE)),--trace,)

coverage-report:
	$(PYTHON) coverage_report.py $(COVFILE)

test:
	SPIKE="$(SPIKE)" RISCV_GCC="$(RISCV_GCC)" $(PYTHON) -m unittest discover -s gen/tests -v
	$(PYTHON) -m unittest discover -s sim/tests -v

# Independent FPU result oracle, including pipeline stalls and cancellation.
.PHONY: fpu-test
fpu-test:
	$(VERILATOR) --cc --exe --build -j $(BUILD_JOBS) --top-module fpu_test_top \
		-Wno-fatal -DFPU -Mdir obj_fpu_test -F rtl/fpu_test.f \
		-CFLAGS "-std=c++20 -I$(SPIKE_INC)/softfloat" \
		-LDFLAGS "$(SPIKE_LIB)/libsoftfloat.a" sim/tests/fpu_test.cpp
	./obj_fpu_test/Vfpu_test_top

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
	fst2vcd $(TRACE) > $(OUT)/cosim.vcd
	env -u LD_LIBRARY_PATH -u GTK_PATH -u GIO_EXTRA_MODULES -u LD_PRELOAD \
		gtkwave $(OUT)/cosim.vcd

clean:
	rm -rf obj_dir $(OUT) tests/prog.c
