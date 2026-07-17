# =============================================================================
#  ZenithSoC Virtual Platform — Makefile
#  Usage: make BLOCK=uart run
# =============================================================================

BLOCK       ?= _template
TIMEOUT		?= 10s

# --- Paths ---
BLOCK_DIR    = blocks/$(BLOCK)
BLOCK_FW     = $(BLOCK_DIR)/sw/firmware.cpp

TB_TOP 		 = common/tb/tb_top.sv
ZENITH_F     = ../hw/_zenithSoC.f
SIM_SRC      = common/sim/sim_main.cpp
FW_STARTUP   = common/sw/startup.S
FW_LINK      = common/sw/link.ld

TRACE_START ?= 0
TRACE_END   ?= 18446744073709551615

# Driver paths
FW_INC_DIRS = common/sw \
              ../sw/lib \
              ../sw/lib/driver

FW_INCS = $(addprefix -I,$(FW_INC_DIRS))

FW_SRC_DIRS = ../sw/src

# ZenithSoC hw source tree (for includes)
ZENITH_HW    = ..

# --- Tools ---
VERILATOR     ?= verilator
RISCV_GCC     ?= riscv32-unknown-elf-g++
RISCV_OBJDUMP ?= riscv32-unknown-elf-objdump
SPIKE_DIR     ?= 

# --- Block-specific MMIO addresses ---
# Each block has its own base address in the ZenithSoC memory map.
# The Makefile maps BLOCK name to the correct IO_BASE for Spike.
IO_SIZE      = 0x2000

ifeq ($(BLOCK),uart)
    IO_BASE  = 0x00004000
else ifeq ($(BLOCK),timer)
    IO_BASE  = 0x00006000
else ifeq ($(BLOCK),gpio)
    IO_BASE  = 0x00008000
else ifeq ($(BLOCK),spi)
    IO_BASE  = 0x0000A000
else ifeq ($(BLOCK),ethernet)
    IO_BASE  = 0x0000C000
else ifeq ($(BLOCK),prng)
    IO_BASE  = 0x0000E000
else ifeq ($(BLOCK),apu)
    IO_BASE  = 0x00010000
    IO_SIZE  = 0x2400
else ifeq ($(BLOCK),sd)
    IO_BASE  = 0x00014400
else
    # Default: use UART base for template and unknown blocks
    IO_BASE  = 0x00004000
endif

# --- Verilator flags ---
VFLAGS       = --cc --exe --trace-fst --trace-structs \
			   --timing \
               --top-module tb_top \
               -Wno-fatal \
			   -Wno-BLKANDNBLK \
               -y $(ZENITH_HW)/hw \
               -I$(ZENITH_HW)/hw \
			   -I$(BLOCK_DIR)/rtl \
               -CFLAGS "-std=c++17 -O2 -I$(SPIKE_DIR)/include" \
               -LDFLAGS "-L$(SPIKE_DIR)/lib -lriscv -lfesvr -lpthread -ldl"

-include $(BLOCK_DIR)/block.mk

# =============================================================================
#  Targets
# =============================================================================

.PHONY: all verilate firmware run run-notrace wave clean info new-block

all: verilate firmware

# --- Verilate + compile C++ model ---
verilate:
	@echo "=== Verilating block: $(BLOCK) ==="
	$(VERILATOR) $(VFLAGS) -F $(ZENITH_F) $(TB_TOP) $(SIM_SRC)
	$(MAKE) -C obj_dir -f Vtb_top.mk -j$$(nproc)
	@echo "=== Verilator build done ==="

# --- Compile firmware ---
firmware:
	@mkdir -p out
	@echo "=== Compiling firmware for block: $(BLOCK) ==="
	$(RISCV_GCC) -march=rv32im_zfinx_zba_zbs_zicsr -fno-use-cxa-atexit -fno-exceptions -mno-fdiv -mabi=ilp32 -nostartfiles \
	    -T $(FW_LINK) $(FW_STARTUP) $(BLOCK_FW) $(FW_SRCS) \
	    $(FW_INCS) \
	    -O2 -o out/firmware.elf
	$(RISCV_OBJDUMP) -d out/firmware.elf > out/firmware.dis
	@echo "=== Firmware built: out/firmware.elf ==="

# --- Run simulation with waveform ---
run: all
	@echo "=== Running VP: BLOCK=$(BLOCK) IO_BASE=$(IO_BASE) ==="
	@mkdir -p out
	./obj_dir/Vtb_top +firmware=out/firmware.elf \
		+io_base=$(IO_BASE) +io_size=$(IO_SIZE) \
		+trace_start=$(TRACE_START) +trace_end=$(TRACE_END) \
		> out/stdout.txt 2>&1
	@echo "=== Simulation done ==="

# --- Run simulator with waveform and a timeout when the FW get stuck (easy debug) ---
run-timeout: all
	@echo "=== Running VP with timeout $(TIMEOUT): BLOCK=$(BLOCK) IO_BASE=$(IO_BASE) ==="
	-timeout -s TERM $(TIMEOUT) ./obj_dir/Vtb_top +firmware=out/firmware.elf \
	    +io_base=$(IO_BASE) +io_size=$(IO_SIZE)

# --- Run without waveform (faster) ---
run-notrace: all
	@echo "=== Running VP (no trace): BLOCK=$(BLOCK) ==="
	./obj_dir/Vtb_top +firmware=out/firmware.elf \
	    +io_base=$(IO_BASE) +io_size=$(IO_SIZE) +notrace
	@echo "=== Simulation done ==="

# --- Open waveform ---
wave:
	fst2vcd out/waveform.fst > out/waveform.vcd
	env -u LD_LIBRARY_PATH -u GTK_PATH -u GIO_EXTRA_MODULES -u LD_PRELOAD gtkwave out/waveform.vcd

# --- Print block info ---
info:
	@echo "Block:    $(BLOCK)"
	@echo "RTL:      $(BLOCK_RTL)"
	@echo "Firmware: $(BLOCK_FW)"
	@echo "IO_BASE:  $(IO_BASE)"
	@echo "IO_SIZE:  $(IO_SIZE)"
	@echo "Zenith:   $(ZENITH_HW)"

# --- Create new block from template ---
new-block:
	@if [ -z "$(NAME)" ]; then \
	    echo "Usage: make new-block NAME=my_block"; exit 1; \
	fi
	@cp -r blocks/_template blocks/$(NAME)
	@echo "Created blocks/$(NAME)/ — edit dut_wrapper.sv and firmware.cpp"

# --- Clean ---
clean:
	rm -rf obj_dir out
