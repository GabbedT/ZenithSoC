# ============================================================================
# Targets:
#   make sd     Build SD-boot image (bootloader.elf + eiot_*.elf)
#   make sim    Build Verilator-direct ELF (firmware only, DDR@0x8000_0000)
#   make clean
#
# Usage (SD card real hardware):
#   make sd
#   sudo tools/write_sd.sh /dev/sdX out/bootloader.bin out/coremark_app.bin
# ============================================================================

SHELL := /bin/bash

# ----------------------------------------------------------------------------
# Paths
# ----------------------------------------------------------------------------

TOP         := $(abspath .)
SRC_DIR     := $(abspath embench-iot/src)
SUPPORT_DIR := $(abspath embench-iot/support)
PORT_DIR    := $(abspath port)
APP_DIR     := $(abspath app)
BOOT_DIR    := $(abspath bootloader)
SIM_DIR     := $(abspath sim)
OUT         := $(abspath out)


# ----------------------------------------------------------------------------
# Toolchain
# ----------------------------------------------------------------------------

RISCV_PREFIX ?= riscv32-unknown-elf-

CC    := $(RISCV_PREFIX)gcc
AS    := $(RISCV_PREFIX)as
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
# Common compiler/linker flags
# ----------------------------------------------------------------------------

COMMON_CFLAGS := \
	-march=$(MARCH) \
	-mabi=$(MABI) \
	-O3 \
	-g3 \
	-ffreestanding \
	-fno-common \
	-fno-builtin \
	-fdata-sections \
	-ffunction-sections \
	-msmall-data-limit=0 \
	-mno-fdiv \
	-DHAVE_CONFIG_H \
	-DHAVE_BOARDSUPPORT_H \
	-include $(PORT_DIR)/config.h \
	-I$(PORT_DIR) \
	-I$(SUPPORT_DIR) \
	-I$(SRC_DIR)

COMMON_LDFLAGS := \
	-march=$(MARCH) \
	-mabi=$(MABI) \
	-nostartfiles \
	-Wl,--gc-sections \
	-T$(SIM_DIR)/crt0_sim.ld

LDLIBS := -lm -lc -lgcc


# ----------------------------------------------------------------------------
# Sources
# ----------------------------------------------------------------------------

# Go into Embench-IoT source folder and extract each benchmark
SUPPORT_SRCS := $(shell find $(SUPPORT_DIR) -maxdepth 1 -name "*.c" | sort)

# Do the same but in port directory
PORT_SRCS := $(shell find $(PORT_DIR) -maxdepth 1 -name "*.c" | sort)

# Extract the name on each directory
BENCHES := $(notdir $(wildcard $(SRC_DIR)/*))


BOOT_ROM_ELF := $(OUT)/sim/boot_rom.elf


# ============================================================================
# SIM TARGET
# ============================================================================

SIM_OUT := $(OUT)/sim

.INTERMEDIATE: $(SIM_OUT)/boot_rom.o

# Build each benchmark directory
SIM_ELFS := $(foreach bench,$(BENCHES),$(SIM_OUT)/$(bench)/$(bench).elf)
SIM_DUMPS := $(SIM_ELFS:.elf=.dump)

sim: $(SIM_DUMPS) $(BOOT_ROM_ELF)

$(SIM_ELFS): FORCE $(SIM_DIR)/crt0_sim.s $(SIM_DIR)/crt0_sim.ld
	@mkdir -p $(@D)
	@bench="$(notdir $(@D))"; \
	echo "==> Building Embench benchmark: $$bench"; \
	$(CC) $(COMMON_CFLAGS) \
		-I$(SRC_DIR)/$$bench \
		$(SIM_DIR)/crt0_sim.s \
		$$(find "$(SRC_DIR)/$$bench" -name "*.c" | sort) \
		$(SUPPORT_SRCS) \
		$(COMMON_LDFLAGS) \
		$(LDLIBS) \
		-o $@
	$(SIZE) $@

%.dump: %.elf
	$(DUMP) -d -S --demangle $< > $@

$(SIM_OUT)/boot_rom.o: $(SIM_DIR)/boot_rom.s | $(SIM_OUT)
	$(AS) -march=$(MARCH) -mabi=$(MABI) $< -o $@

$(BOOT_ROM_ELF): $(SIM_OUT)/boot_rom.o | $(OUT)
	$(RISCV_PREFIX)ld -T $(SIM_DIR)/boot_rom.ld -o $@ $<


# ============================================================================
# CLEAN TARGET
# ============================================================================

clean:
	rm -rf $(OUT)

FORCE:


# ============================================================================
# DIRECTORIES
# ============================================================================

$(OUT) $(SD_OUT) $(SIM_OUT):
	@mkdir -p $@

.PHONY: sim clean