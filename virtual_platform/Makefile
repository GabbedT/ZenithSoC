# =============================================================================
#  ZenithSoC Virtual Platform — Makefile
#  Usage: make BLOCK=uart run
# =============================================================================

BLOCK       ?= _template

# --- Paths ---
BLOCK_DIR    = blocks/$(BLOCK)
BLOCK_RTL    = $(wildcard $(BLOCK_DIR)/rtl/*.sv)
BLOCK_FW     = $(BLOCK_DIR)/sw/firmware.c

COMMON_RTL   = common/rtl/io_master_bfm.sv common/tb/tb_top.sv
SIM_SRC      = common/sim/sim_main.cpp
FW_STARTUP   = common/sw/startup.S
FW_LINK      = common/sw/link.ld

# ZenithSoC hardware source tree (for includes)
ZENITH_HW    = ../ZenithSoC/Hardware

# --- Tools ---
VERILATOR    ?= verilator
RISCV_GCC    ?= riscv32-unknown-elf-gcc
RISCV_OBJDUMP ?= riscv32-unknown-elf-objdump
SPIKE_DIR    ?= /opt/spike

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
               --top-module tb_top \
               -Wno-fatal \
               -I$(ZENITH_HW) \
               -CFLAGS "-std=c++17 -O2 -I$(SPIKE_DIR)/include" \
               -LDFLAGS "-L$(SPIKE_DIR)/lib -lriscv -lfesvr -lpthread -ldl"

# --- Optional: include block-specific config ---
-include $(BLOCK_DIR)/block.mk

# =============================================================================
#  Targets
# =============================================================================

.PHONY: all verilate firmware run run-notrace wave clean info new-block

all: verilate firmware

# --- Verilate + compile C++ model ---
verilate:
	@echo "=== Verilating block: $(BLOCK) ==="
	$(VERILATOR) $(VFLAGS) $(COMMON_RTL) $(BLOCK_RTL) $(SIM_SRC)
	$(MAKE) -C obj_dir -f Vtb_top.mk -j$$(nproc)
	@echo "=== Verilator build done ==="

# --- Compile firmware ---
firmware:
	@mkdir -p out
	@echo "=== Compiling firmware for block: $(BLOCK) ==="
	$(RISCV_GCC) -march=rv32im_zicsr -mabi=ilp32 -nostdlib \
	    -T $(FW_LINK) $(FW_STARTUP) $(BLOCK_FW) \
	    -I common/sw \
	    -O2 -o out/firmware.elf
	$(RISCV_OBJDUMP) -d out/firmware.elf > out/firmware.dis
	@echo "=== Firmware built: out/firmware.elf ==="

# --- Run simulation with waveform ---
run: all
	@echo "=== Running VP: BLOCK=$(BLOCK) IO_BASE=$(IO_BASE) ==="
	./obj_dir/Vtb_top +firmware=out/firmware.elf \
	    +io_base=$(IO_BASE) +io_size=$(IO_SIZE)
	@echo "=== Simulation done ==="

# --- Run without waveform (faster) ---
run-notrace: all
	@echo "=== Running VP (no trace): BLOCK=$(BLOCK) ==="
	./obj_dir/Vtb_top +firmware=out/firmware.elf \
	    +io_base=$(IO_BASE) +io_size=$(IO_SIZE) +notrace
	@echo "=== Simulation done ==="

# --- Open waveform ---
wave:
	gtkwave out/waveform.fst &

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
	@echo "Created blocks/$(NAME)/ — edit dut_wrapper.sv and firmware.c"

# --- Clean ---
clean:
	rm -rf obj_dir out
