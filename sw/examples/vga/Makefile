SHELL := /bin/bash
.SHELLFLAGS := -e -o pipefail -c
.DEFAULT_GOAL := all

REPO_ROOT := $(abspath ../../..)
LIB_DIR := $(abspath ../../lib)
SRC_DIR := $(abspath ../../src)
COREMARK_BOOT := $(abspath ../../benchmark/CoreMark/bootloader)
TB_DIR := $(abspath ../../../tb/verilator)
OUT := $(abspath out)

RISCV_PREFIX ?= riscv32-unknown-elf-
CC := $(RISCV_PREFIX)gcc
CXX := $(RISCV_PREFIX)g++
AS := $(RISCV_PREFIX)as
COPY := $(RISCV_PREFIX)objcopy
DUMP := $(RISCV_PREFIX)objdump
SIZE := $(RISCV_PREFIX)size

MARCH := rv32im_zfinx_zba_zbs_zicsr
MABI := ilp32
ARCH := -march=$(MARCH) -mabi=$(MABI)
COMMON_FLAGS := $(ARCH) -O3 -mno-fdiv -ffreestanding -fno-exceptions \
    -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -fno-common \
    -fno-builtin \
    -I$(LIB_DIR)
BOOT_FLAGS := $(filter-out -O3,$(COMMON_FLAGS)) -Os -fno-inline-functions
LINK_FLAGS := $(ARCH) -nostdlib -fno-exceptions -fno-use-cxa-atexit
LIBGCC := $(shell $(CC) $(ARCH) -print-libgcc-file-name)

DEMO_FRAMES ?= 0
RESOLUTION ?= 320x240
SD_BLOCK ?= 0x2000

ifeq ($(RESOLUTION),320x240)
VGA_HIGH_RES := 0
else ifeq ($(RESOLUTION),640x480)
VGA_HIGH_RES := 1
else
$(error RESOLUTION must be 320x240 or 640x480)
endif

# Configuration-specific objects ensure changing the resolution or finite-run
# count cannot silently reuse an object compiled with different defines.
OBJ := $(OUT)/obj_$(subst x,_,$(RESOLUTION))_f$(DEMO_FRAMES)

APP_ELF := $(OUT)/vga_cube.elf
APP_BIN := $(OUT)/program.bin
BOOT_ELF := $(OUT)/boot.elf
BOOT_HEX := $(OUT)/boot.hex

APP_OBJS := \
    $(OBJ)/app_start.o \
    $(OBJ)/cube.o \
    $(OBJ)/runtime.o \
    $(OBJ)/serial_io.o \
    $(OBJ)/vga_driver.o

BOOT_OBJS := \
    $(OBJ)/boot_start.o \
    $(OBJ)/boot_sd.o \
    $(OBJ)/runtime.o \
    $(OBJ)/boot_uart.o \
    $(OBJ)/boot_sd_driver.o

.PHONY: all run run-headless clean FORCE

FORCE:

all: $(BOOT_HEX) $(APP_BIN)
	@echo "=== ZenithSoC VGA SD demo ==="
	@echo "Boot ROM : $(BOOT_HEX)"
	@echo "SD image : $(APP_BIN) (write at block $(SD_BLOCK))"
	@echo "Mode     : $(RESOLUTION)"
	@echo "Verilator: make run"

$(OBJ)/app_start.o: app_start.S | $(OBJ)
	$(AS) $(ARCH) $< -o $@

$(OBJ)/cube.o: cube.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) -DDEMO_FRAMES=$(DEMO_FRAMES) \
		-DVGA_HIGH_RES=$(VGA_HIGH_RES) -c $< -o $@

$(OBJ)/runtime.o: runtime.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) -c $< -o $@

$(OBJ)/serial_io.o: $(SRC_DIR)/Serial_IO.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) -c $< -o $@

$(OBJ)/vga_driver.o: $(SRC_DIR)/VGA.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) -c $< -o $@

$(APP_ELF): $(APP_OBJS) app.ld FORCE | $(OUT)
	$(CXX) $(LINK_FLAGS) -T app.ld -o $@ $(APP_OBJS) $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(OUT)/vga_cube.dump

$(APP_BIN): $(APP_ELF)
	$(COPY) -O binary $< $@

$(OBJ)/boot_start.o: $(COREMARK_BOOT)/boot_start.s | $(OBJ)
	$(AS) $(ARCH) $< -o $@

$(OBJ)/boot_sd.o: $(COREMARK_BOOT)/boot_sd.cpp $(APP_BIN) | $(OBJ)
	$(CXX) $(BOOT_FLAGS) \
		-DIMG_BLOCKS=$$((($$(stat -c%s "$(APP_BIN)") + 511) / 512)) \
		-c $< -o $@

$(OBJ)/boot_uart.o: $(SRC_DIR)/UART.cpp | $(OBJ)
	$(CXX) $(BOOT_FLAGS) -c $< -o $@

$(OBJ)/boot_sd_driver.o: $(SRC_DIR)/SD.cpp | $(OBJ)
	$(CXX) $(BOOT_FLAGS) -c $< -o $@

$(BOOT_ELF): $(BOOT_OBJS) $(COREMARK_BOOT)/boot.ld | $(OUT)
	$(CXX) $(LINK_FLAGS) -T $(COREMARK_BOOT)/boot.ld -o $@ $(BOOT_OBJS) $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(OUT)/boot.dump

$(BOOT_HEX): $(BOOT_ELF) $(REPO_ROOT)/tools/elf2hex.sh
	bash $(REPO_ROOT)/tools/elf2hex.sh $< $@

run: all
	$(MAKE) -C $(TB_DIR) run \
		DDR=$(APP_ELF) BOOT=$(BOOT_ELF) SD=$(APP_BIN) SD_BLOCK=$(SD_BLOCK) \
		VGA=1 VGA_SCALE=2 TRACE=0

run-headless: all
	$(MAKE) -C $(TB_DIR) run \
		DDR=$(APP_ELF) BOOT=$(BOOT_ELF) SD=$(APP_BIN) SD_BLOCK=$(SD_BLOCK) \
		VGA_DUMP=$(OUT)/vga_latest.png TRACE=0

$(OUT) $(OBJ):
	@mkdir -p $@

clean:
	rm -rf $(OUT)
