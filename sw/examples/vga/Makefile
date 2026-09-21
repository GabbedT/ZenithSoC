SHELL := /bin/bash
.SHELLFLAGS := -e -o pipefail -c
.DEFAULT_GOAL := all

REPO_ROOT := $(abspath ../../..)
LIB_DIR := $(abspath ../../lib)
SRC_DIR := $(abspath ../../src)
BOOTLOADER_DIR := $(abspath ../../bootloader)
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
LINK_FLAGS := $(ARCH) -nostdlib -fno-exceptions -fno-use-cxa-atexit
LIBGCC := $(shell $(CC) $(ARCH) -print-libgcc-file-name)

DEMO_FRAMES ?= 0
RESOLUTION ?= 320x240
VGA_DEMO ?= cube
SD_BLOCK ?= 0x2000

ifneq ($(filter cube fractal,$(VGA_DEMO)),$(VGA_DEMO))
$(error VGA_DEMO must be cube or fractal)
endif

ifeq ($(VGA_DEMO),fractal)
DEMO_FLAGS := -DVGA_DEMO_FRACTAL=1
else
DEMO_FLAGS :=
endif

ifeq ($(RESOLUTION),320x240)
VGA_HIGH_RES := 0
else ifeq ($(RESOLUTION),640x480)
VGA_HIGH_RES := 1
else
$(error RESOLUTION must be 320x240 or 640x480)
endif

# Configuration-specific objects ensure changing the resolution or finite-run
# count cannot silently reuse an object compiled with different defines.
OBJ := $(OUT)/obj_$(subst x,_,$(RESOLUTION))_$(VGA_DEMO)_f$(DEMO_FRAMES)

APP_ELF := $(OUT)/vga_$(VGA_DEMO).elf
APP_BIN := $(OUT)/program.bin
BOOT_ELF := $(OUT)/boot.elf
BOOT_BIN := $(OUT)/boot.bin
BOOT_HEX := $(OUT)/boot.hex
SD_HEX := $(OUT)/vga_$(VGA_DEMO)_sd_words.hex

APP_OBJS := \
    $(OBJ)/app_start.o \
    $(OBJ)/vga.o \
    $(OBJ)/cube.o \
    $(OBJ)/fractal.o \
    $(OBJ)/runtime.o \
    $(OBJ)/serial_io.o \
    $(OBJ)/vga_driver.o

BOOTLOADER_SOURCES := \
    $(BOOTLOADER_DIR)/Makefile \
    $(BOOTLOADER_DIR)/boot.ld \
    $(BOOTLOADER_DIR)/boot_config.h \
    $(BOOTLOADER_DIR)/boot_start.s \
    $(BOOTLOADER_DIR)/boot_sd.cpp \
    $(BOOTLOADER_DIR)/boot_runtime.cpp

BOOT_IMAGE_BLOCK ?= $(SD_BLOCK)
BOOT_LOAD_ADDRESS ?= 0x80000000
BOOT_ENTRY_ADDRESS ?= $(BOOT_LOAD_ADDRESS)
BOOT_UART_INDEX ?= 0
BOOT_BAUD_RATE ?= 115200
BOOT_SD_CLOCK ?= 1
BOOT_SD_BUS ?= 1
BOOT_LOGGING ?= 1
BOOT_DUMP_WORDS ?= 16

.PHONY: all run run-headless clean FORCE

FORCE:

all: $(BOOT_HEX) $(APP_BIN) $(SD_HEX)
	@echo "=== ZenithSoC VGA SD demo ==="
	@echo "Boot ROM : $(BOOT_HEX)"
	@echo "SD image : $(APP_BIN) (write at block $(SD_BLOCK))"
	@echo "SD hex   : $(SD_HEX)"
	@echo "Demo     : $(VGA_DEMO)"
	@echo "Mode     : $(RESOLUTION)"
	@echo "Verilator: make run"

$(OBJ)/app_start.o: app_start.S | $(OBJ)
	$(AS) $(ARCH) $< -o $@

$(OBJ)/vga.o: vga.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) $(DEMO_FLAGS) -DDEMO_FRAMES=$(DEMO_FRAMES) \
		-DVGA_HIGH_RES=$(VGA_HIGH_RES) -c $< -o $@

$(OBJ)/cube.o: cube.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) \
		-DVGA_HIGH_RES=$(VGA_HIGH_RES) -c $< -o $@

$(OBJ)/fractal.o: fractal.cpp | $(OBJ)
	$(CXX) $(COMMON_FLAGS) \
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
	$(DUMP) -d $@ > $(OUT)/vga_$(VGA_DEMO).dump

$(APP_BIN): $(APP_ELF)
	$(COPY) -O binary $< $@

$(SD_HEX): $(APP_BIN)
	@command -v xxd >/dev/null || { echo "xxd is required to create the SD image" >&2; exit 1; }
	xxd -p -c 4 $< > $@

$(BOOT_ELF): $(APP_BIN) $(BOOTLOADER_SOURCES) FORCE | $(OUT)
	$(MAKE) -C $(BOOTLOADER_DIR) all \
		APP_BIN=$(APP_BIN) \
		OBJ_DIR=$(OUT)/boot_obj \
		BOOT_ELF=$(BOOT_ELF) \
		BOOT_BIN=$(BOOT_BIN) \
		BOOT_HEX=$(BOOT_HEX) \
		BOOT_DUMP=$(OUT)/boot.dump \
		BOOT_IMAGE_BLOCK=$(BOOT_IMAGE_BLOCK) \
		BOOT_LOAD_ADDRESS=$(BOOT_LOAD_ADDRESS) \
		BOOT_ENTRY_ADDRESS=$(BOOT_ENTRY_ADDRESS) \
		BOOT_UART_INDEX=$(BOOT_UART_INDEX) \
		BOOT_BAUD_RATE=$(BOOT_BAUD_RATE) \
		BOOT_SD_CLOCK=$(BOOT_SD_CLOCK) \
		BOOT_SD_BUS=$(BOOT_SD_BUS) \
		BOOT_LOGGING=$(BOOT_LOGGING) \
		BOOT_DUMP_WORDS=$(BOOT_DUMP_WORDS)

$(BOOT_BIN) $(BOOT_HEX): $(BOOT_ELF)
	@test -s "$@"

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
