# ======================================================================
# Standalone Verilator testbench for the full ZenithSoC.
#
# Prerequisite: source the cosim environment first (Verilator 5.026, GCC 14.2,
# Spike libs)
#     source ../../cosim/setenv.sh
#
# Targets:
#   make build                    verilate zenith_tb_top + sim_main.cpp
#   make run DDR=fw.elf [BOOT=b.elf] [SD=image.bin]   build + run
#   make wave                     open the latest waveform
#   make info                     print resolved configuration
#   make clean
#
# Run options:
#   DDR=path.elf        firmware ELF or symbol image (optional with SD=...)
#   BOOT=path.elf       optional boot-stub ELF loaded into the ROM
#   SD=path.bin|hex     optional image loaded into the simulated SD card
#   SD_BLOCK=N          first SD block used for the image (default 0x2000)
#   WAVE=1              dump out/zenith.fst                       (default 0)
#   TRACE=1             print the per-instruction trace           (default 1)
#   VGA=1               open a live VGA window                    (default 0)
#   VGA_DUMP=path.png   continuously save the latest complete frame
#   VGA_SCALE=1..4      live-window integer scaling               (default 1)
#   TRACE_START=N       start printing after N simulated cycles  (default 0)
#   MAX_CYCLES=N        stop after N cycles (0 = run until tohost) (default 0)
# ======================================================================

SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

# --- Sources -----------------------------------------------------------
# Absolute paths: the generated obj_dir/*.mk compiles from inside obj_dir, so
# relative -I/-y would resolve against the wrong directory.
ZENITH_HW := $(abspath ../../hw)
COSIM_SIM := $(abspath ../../cosim/sim)
SD_MODEL  := $(abspath ../sd_model)
TB_F      = zenith_tb.f
SIM_SRC   = sim_main.cpp vga_receiver.cpp

OUT    = out
LOGDIR = logs

# --- ISA (must match the toolchain used to build the firmware) ---------
# Mirrors cosim/config.mk; override on the command line if needed.
ISA ?= rv32im_zfinx_zba_zbs_zicsr

# --- Run parameters ----------------------------------------------------
DDR        ?=
BOOT       ?=
SD         ?=
SD_BLOCK   ?= 0x2000
WAVE       ?= 0
TRACE      ?= 1
TRACE_START ?= 0
MAX_CYCLES ?= 0
VGA        ?= 0
VGA_DUMP   ?=
VGA_SCALE  ?= 1

# --- Tools -------------------------------------------------------------
VERILATOR ?= verilator

SPIKE_INC ?= $(HOME)/riscv-isa-sim
SPIKE_LIB ?= $(HOME)/riscv-isa-sim/build
SIM_DIR   := $(abspath .)

VLATOR_DEFS = -DCOSIM_ISA=\\\"$(ISA)\\\"

# X11 is optional: headless VGA capture and PNG dumps do not need it.
X11_FOUND  := $(shell pkg-config --exists x11 2>/dev/null && echo 1)
ifeq ($(X11_FOUND),1)
VGA_CFLAGS  := $(shell pkg-config --cflags x11) -DZTB_HAVE_X11=1
VGA_LDFLAGS := $(shell pkg-config --libs x11)
endif

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
    -y $(SD_MODEL) \
    -I$(ZENITH_HW) \
    -I$(SD_MODEL) \
    -Mdir obj_dir \
    -CFLAGS "-std=c++20 -O2 -I$(SPIKE_INC) -I$(COSIM_SIM) -I$(SIM_DIR) $(VGA_CFLAGS) $(VLATOR_DEFS)" \
    -LDFLAGS "-L$(SPIKE_LIB) -lriscv -lfesvr -lpthread -ldl -lz $(VGA_LDFLAGS)"

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
	@test -n "$(DDR)$(SD)" || { echo "ERROR: pass DDR=fw.elf and/or SD=image.bin|hex"; exit 1; }
	@echo "=== [ZTB] Running (DDR=$(DDR) BOOT=$(BOOT)) ==="
	./obj_dir/Vzenith_tb_top \
		$(if $(DDR),+firmware=$(DDR),) \
		$(if $(BOOT),+boot=$(BOOT),) \
		$(if $(SD),+sd=$(SD) +sd_block=$(SD_BLOCK),) \
		$(if $(filter 1,$(WAVE)),+wave,) \
		$(if $(filter 0,$(TRACE)),+notrace,) \
		$(if $(filter 1,$(VGA)),+vga +vga_scale=$(VGA_SCALE),) \
		$(if $(VGA_DUMP),+vga_dump=$(VGA_DUMP),) \
		$(if $(filter-out 0,$(TRACE_START)),+trace_start=$(TRACE_START),) \
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
	@echo "SD         : $(SD)   SD_BLOCK=$(SD_BLOCK)"
	@echo "WAVE/TRACE : $(WAVE)/$(TRACE)   TRACE_START=$(TRACE_START)   MAX_CYCLES=$(MAX_CYCLES)"
	@echo "VGA         : $(VGA)   VGA_SCALE=$(VGA_SCALE)   VGA_DUMP=$(VGA_DUMP)"
	@echo "X11 viewer  : $(if $(X11_FOUND),available,not built; PNG capture only)"

clean:
	rm -rf obj_dir $(OUT) $(LOGDIR)
