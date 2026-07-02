# ============================================================================
# Targets:
#   make sd     Build SD-boot image (bootloader.elf + coremark_app.elf)
#   make sim    Build Verilator-direct ELF (firmware only, DDR@0x8000_0000)
#   make clean
#
# Usage (sim):
#   make sim
#   # in tb/verilator:
#   make run DDR=../../sw/benchmark/CoreMark/out/coremark_sim.elf
#
# Usage (SD card real hardware):
#   make sd
#   sudo tools/write_sd.sh /dev/sdX out/bootloader.bin out/coremark_app.bin
# ============================================================================

SHELL := /bin/bash

# ----------------------------------------------------------------------------
# Paths
# ----------------------------------------------------------------------------

ROOT      := $(abspath ../..)
LIB_DIR   := $(abspath ../../lib)
SRC_DIR   := $(abspath ../../src)
CORE_DIR  := $(abspath coremark)
PORT_DIR  := $(abspath port)
APP_DIR   := $(abspath app)
BOOT_DIR  := $(abspath bootloader)
SIM_DIR   := $(abspath sim)
COSIM_SW  := $(abspath ../../../cosim/sw)
OUT       := $(abspath out)

# ----------------------------------------------------------------------------
# Toolchain
# ----------------------------------------------------------------------------

RISCV_PREFIX ?= riscv32-unknown-elf-

CC    := $(RISCV_PREFIX)gcc
CXX   := $(RISCV_PREFIX)g++
AS    := $(RISCV_PREFIX)as
LD    := $(RISCV_PREFIX)g++
DUMP  := $(RISCV_PREFIX)objdump
COPY  := $(RISCV_PREFIX)objcopy
SIZE  := $(RISCV_PREFIX)size

LIBGCC := $(shell $(CC) -march=rv32im_zfinx_zba_zbs_zicsr -mabi=ilp32 \
        -print-libgcc-file-name 2>/dev/null)

# ----------------------------------------------------------------------------
# ISA / ABI
# ----------------------------------------------------------------------------

MARCH := rv32im_zfinx_zba_zbs_zicsr
MABI  := ilp32

# ----------------------------------------------------------------------------
# CoreMark parameters
# ----------------------------------------------------------------------------

SD_ITERATIONS  ?= 2000      # standard score (real hardware)
SIM_ITERATIONS ?= 50        # quick functional check in simulation
DATA_SIZE      ?= 2000      # PERFORMANCE_RUN

# ----------------------------------------------------------------------------
# Common compiler/linker flags
# ----------------------------------------------------------------------------

CFLAGS_BASE := \
    -march=$(MARCH) -mabi=$(MABI) \
    -O3 -mno-fdiv -ffreestanding -fno-exceptions \
    -funroll-loops -finline-functions -fno-common \
    -nostartfiles -fno-use-cxa-atexit \
    -I$(PORT_DIR) -I$(CORE_DIR) -I$(LIB_DIR)

LFLAGS_BASE := \
    -march=$(MARCH) -mabi=$(MABI) \
    -nostartfiles -fno-exceptions -fno-use-cxa-atexit

SD_DEFS  := -DITERATIONS=$(SD_ITERATIONS)  -DTOTAL_DATA_SIZE=$(DATA_SIZE) -DPERFORMANCE_RUN=1
SIM_DEFS := -DITERATIONS=$(SIM_ITERATIONS) -DTOTAL_DATA_SIZE=$(DATA_SIZE) -DPERFORMANCE_RUN=1

# ============================================================================
# SD TARGET
# ============================================================================

SD_OUT    := $(OUT)/sd
APP_ELF   := $(OUT)/coremark_app.elf
APP_BIN   := $(OUT)/coremark_app.bin
BOOT_ELF  := $(OUT)/bootloader.elf
BOOT_BIN  := $(OUT)/bootloader.bin

APP_OBJS := \
    $(SD_OUT)/app_start.o \
    $(SD_OUT)/cm_core_list_join.o \
    $(SD_OUT)/cm_core_main.o \
    $(SD_OUT)/cm_core_matrix.o \
    $(SD_OUT)/cm_core_state.o \
    $(SD_OUT)/cm_core_util.o \
    $(SD_OUT)/cm_core_portme.o \
    $(SD_OUT)/cm_ee_printf_zenith.o \
    $(SD_OUT)/cm_uart.o

BOOT_OBJS := \
    $(SD_OUT)/boot_start.o \
    $(SD_OUT)/boot_sd.o \
    $(SD_OUT)/boot_uart.o \
    $(SD_OUT)/boot_sddrv.o

.PHONY: sd

sd: $(APP_BIN) $(BOOT_BIN)
	@echo "=== SD Image ready ==="
	@echo "  Bootloader: $(BOOT_BIN)"
	@echo "  App:        $(APP_BIN)"
	@echo "Flash: sudo tools/write_sd.sh /dev/sdX $(BOOT_BIN) $(APP_BIN)"

$(SD_OUT)/app_start.o: $(APP_DIR)/app_start.s | $(SD_OUT)
	$(AS) -march=$(MARCH) -mabi=$(MABI) $< -o $@

$(SD_OUT)/cm_core_list_join.o: $(CORE_DIR)/core_list_join.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_core_main.o: $(CORE_DIR)/core_main.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_core_matrix.o: $(CORE_DIR)/core_matrix.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_core_state.o: $(CORE_DIR)/core_state.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_core_util.o: $(CORE_DIR)/core_util.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_core_portme.o: $(PORT_DIR)/core_portme.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_ee_printf_zenith.o: $(PORT_DIR)/ee_printf_zenith.c | $(SD_OUT)
	$(CC) $(CFLAGS_BASE) $(SD_DEFS) -c $< -o $@

$(SD_OUT)/cm_uart.o: $(SRC_DIR)/UART.cpp | $(SD_OUT)
	$(CXX) $(CFLAGS_BASE) -c $< -o $@

$(APP_ELF): $(APP_OBJS) | $(OUT)
	$(LD) $(LFLAGS_BASE) -T $(APP_DIR)/coremark.ld -o $@ $^ $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(OUT)/coremark_app.dump

$(APP_BIN): $(APP_ELF)
	$(COPY) -O binary $< $@

$(SD_OUT)/boot_start.o: $(BOOT_DIR)/boot_start.s | $(SD_OUT)
	$(AS) -march=$(MARCH) -mabi=$(MABI) $< -o $@

$(SD_OUT)/boot_sd.o: $(BOOT_DIR)/boot_sd.cpp | $(SD_OUT)
	$(CXX) $(CFLAGS_BASE) -c $< -o $@

$(SD_OUT)/boot_uart.o: $(SRC_DIR)/UART.cpp | $(SD_OUT)
	$(CXX) $(CFLAGS_BASE) -c $< -o $@

$(SD_OUT)/boot_sddrv.o: $(SRC_DIR)/SD.cpp | $(SD_OUT)
	$(CXX) $(CFLAGS_BASE) -c $< -o $@

$(BOOT_ELF): $(BOOT_OBJS) | $(OUT)
	$(LD) $(LFLAGS_BASE) -T $(BOOT_DIR)/boot.ld -o $@ $^ $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(OUT)/bootloader.dump

$(BOOT_BIN): $(BOOT_ELF)
	$(COPY) -O binary $< $@

# ============================================================================
# SIM TARGET
# ============================================================================

SIM_OUT      := $(OUT)/sim
SIM_ELF      := $(OUT)/coremark_sim.elf
BOOT_ROM_ELF := $(OUT)/boot_rom.elf

SIM_OBJS := \
    $(SIM_OUT)/crt0_sim.o \
    $(SIM_OUT)/cm_core_list_join.o \
    $(SIM_OUT)/cm_core_main.o \
    $(SIM_OUT)/cm_core_matrix.o \
    $(SIM_OUT)/cm_core_state.o \
    $(SIM_OUT)/cm_core_util.o \
    $(SIM_OUT)/cm_core_portme.o \
    $(SIM_OUT)/cm_ee_printf_zenith.o

.PHONY: sim

sim: $(SIM_ELF) $(BOOT_ROM_ELF)
	@echo "=== Sim ELFs ready ==="
	@echo "  Firmware : $(SIM_ELF)"
	@echo "  Boot ROM : $(BOOT_ROM_ELF)"
	@echo "Run: cd tb/verilator && make run DDR=$(SIM_ELF) BOOT=$(BOOT_ROM_ELF)"

$(SIM_OUT)/crt0_sim.o: $(SIM_DIR)/crt0_sim.s | $(SIM_OUT)
	$(AS) -march=$(MARCH) -mabi=$(MABI) $< -o $@

$(SIM_OUT)/cm_core_list_join.o: $(CORE_DIR)/core_list_join.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_core_main.o: $(CORE_DIR)/core_main.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_core_matrix.o: $(CORE_DIR)/core_matrix.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_core_state.o: $(CORE_DIR)/core_state.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_core_util.o: $(CORE_DIR)/core_util.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_core_portme.o: $(PORT_DIR)/core_portme.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_OUT)/cm_ee_printf_zenith.o: $(PORT_DIR)/ee_printf_zenith.c | $(SIM_OUT)
	$(CC) $(CFLAGS_BASE) $(SIM_DEFS) -c $< -o $@

$(SIM_ELF): $(SIM_OBJS) | $(OUT)
	$(LD) $(LFLAGS_BASE) -T $(SIM_DIR)/link_sim.ld -o $@ $^ $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(OUT)/coremark_sim.dump

$(SIM_OUT)/boot_rom.o: $(COSIM_SW)/boot_rom.s | $(SIM_OUT)
	$(AS) -march=$(MARCH) -mabi=$(MABI) $< -o $@

$(BOOT_ROM_ELF): $(SIM_OUT)/boot_rom.o | $(OUT)
	$(RISCV_PREFIX)ld -T $(COSIM_SW)/boot_rom.ld -o $@ $<

# ============================================================================
# Directories and clean
# ============================================================================

$(OUT) $(SD_OUT) $(SIM_OUT):
	@mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OUT)