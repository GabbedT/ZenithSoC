# Shared SD-card bootloader for ZenithSoC applications.
#
# Required:
#   APP_BIN=/absolute/path/to/application.bin
#
# Useful overrides:
#   BOOT_IMAGE_BLOCK=0x2000     first 512-byte SD block
#   BOOT_LOAD_ADDRESS=0x80000000
#   BOOT_ENTRY_ADDRESS=0x80000000
#   BOOT_UART_INDEX=0
#   BOOT_BAUD_RATE=115200
#   BOOT_DDR_WAIT_MS=10         delay before SD boot / DDR access
#   BOOT_SD_CLOCK=1             SD::CLK_25MHZ
#   BOOT_SD_BUS=1               SD::BUS_WIDE
#   BOOT_LOGGING=1
#   BOOT_DUMP_WORDS=0           DDR words printed after loading
#
# Example:
#   make -C sw/bootloader APP_BIN=$PWD/out/program.bin \
#       BOOT_ELF=$PWD/out/boot.elf BOOT_HEX=$PWD/out/boot.hex

SHELL := /bin/bash
.SHELLFLAGS := -e -o pipefail -c
.DEFAULT_GOAL := all

BOOTLOADER_DIR := $(abspath .)
REPO_ROOT := $(abspath ../..)
LIB_DIR := $(REPO_ROOT)/sw/lib
SRC_DIR := $(REPO_ROOT)/sw/src

RISCV_PREFIX ?= riscv32-unknown-elf-
CC := $(RISCV_PREFIX)gcc
CXX := $(RISCV_PREFIX)g++
AS := $(RISCV_PREFIX)as
COPY := $(RISCV_PREFIX)objcopy
DUMP := $(RISCV_PREFIX)objdump
SIZE := $(RISCV_PREFIX)size

MARCH ?= rv32im_zfinx_zba_zbs_zicsr
MABI ?= ilp32
ARCH := -march=$(MARCH) -mabi=$(MABI)

APP_BIN ?=
ifeq ($(strip $(APP_BIN)),)
$(error APP_BIN is required; point it at the application binary on the SD card)
endif

OUT_DIR ?= $(REPO_ROOT)/build/bootloader
OBJ_DIR ?= $(OUT_DIR)/obj
BOOT_ELF ?= $(OUT_DIR)/bootloader.elf
BOOT_BIN ?= $(OUT_DIR)/bootloader.bin
BOOT_HEX ?= $(OUT_DIR)/bootloader.hex
BOOT_DUMP ?= $(OUT_DIR)/bootloader.dump

BOOT_IMAGE_BLOCK ?= 0x2000
BOOT_IMAGE_BLOCKS ?= $(shell stat -c%s "$(APP_BIN)" 2>/dev/null | awk '{print int(($$1 + 511) / 512)}')
BOOT_LOAD_ADDRESS ?= 0x80000000
BOOT_ENTRY_ADDRESS ?= $(BOOT_LOAD_ADDRESS)
BOOT_UART_INDEX ?= 0
BOOT_BAUD_RATE ?= 115200
BOOT_DDR_WAIT_MS ?= 10
BOOT_SD_CLOCK ?= 1
BOOT_SD_BUS ?= 1
BOOT_LOGGING ?= 1
BOOT_DUMP_WORDS ?= 0
BOOT_EXTRA_DEFS ?=

ifeq ($(strip $(BOOT_IMAGE_BLOCKS)),)
$(error cannot determine application size from APP_BIN; build APP_BIN first or set BOOT_IMAGE_BLOCKS)
endif

LIBGCC := $(shell $(CC) $(ARCH) -print-libgcc-file-name 2>/dev/null)
BOOT_DEFS := \
    -DBOOT_IMAGE_BLOCK=$(BOOT_IMAGE_BLOCK) \
    -DBOOT_IMAGE_BLOCKS=$(BOOT_IMAGE_BLOCKS) \
    -DBOOT_LOAD_ADDRESS=$(BOOT_LOAD_ADDRESS) \
    -DBOOT_ENTRY_ADDRESS=$(BOOT_ENTRY_ADDRESS) \
    -DBOOT_UART_INDEX=$(BOOT_UART_INDEX) \
    -DBOOT_BAUD_RATE=$(BOOT_BAUD_RATE) \
    -DBOOT_DDR_WAIT_MS=$(BOOT_DDR_WAIT_MS) \
    -DBOOT_SD_CLOCK=$(BOOT_SD_CLOCK) \
    -DBOOT_SD_BUS=$(BOOT_SD_BUS) \
    -DBOOT_LOGGING=$(BOOT_LOGGING) \
    -DBOOT_DUMP_WORDS=$(BOOT_DUMP_WORDS) \
    $(BOOT_EXTRA_DEFS)

CFLAGS_BOOT := $(ARCH) -Os -ffreestanding -fno-exceptions -fno-rtti \
    -fno-use-cxa-atexit -fno-threadsafe-statics -fno-common -fno-builtin \
    -nostartfiles -I$(LIB_DIR) -I$(BOOTLOADER_DIR)
LFLAGS_BOOT := $(ARCH) -nostdlib -fno-exceptions -fno-use-cxa-atexit

BOOT_CONFIG := $(BOOTLOADER_DIR)/boot_config.h
BOOT_OBJS := $(OBJ_DIR)/boot_start.o \
             $(OBJ_DIR)/boot_sd.o \
             $(OBJ_DIR)/boot_runtime.o \
             $(OBJ_DIR)/boot_uart.o \
             $(OBJ_DIR)/boot_timer.o \
             $(OBJ_DIR)/boot_sd_driver.o

.PHONY: all clean FORCE

all: $(BOOT_ELF) $(BOOT_BIN) $(BOOT_HEX)

$(OBJ_DIR)/boot_start.o: $(BOOTLOADER_DIR)/boot_start.s | $(OBJ_DIR)
	$(AS) $(ARCH) $< -o $@

$(OBJ_DIR)/boot_sd.o: $(BOOTLOADER_DIR)/boot_sd.cpp $(BOOT_CONFIG) $(APP_BIN) FORCE | $(OBJ_DIR)
	$(CXX) $(CFLAGS_BOOT) $(BOOT_DEFS) -c $< -o $@

$(OBJ_DIR)/boot_runtime.o: $(BOOTLOADER_DIR)/boot_runtime.cpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS_BOOT) -c $< -o $@

$(OBJ_DIR)/boot_uart.o: $(SRC_DIR)/UART.cpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS_BOOT) -c $< -o $@

$(OBJ_DIR)/boot_timer.o: $(SRC_DIR)/Timer.cpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS_BOOT) -c $< -o $@

$(OBJ_DIR)/boot_sd_driver.o: $(SRC_DIR)/SD.cpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS_BOOT) -c $< -o $@

$(BOOT_ELF): $(BOOT_OBJS) $(BOOTLOADER_DIR)/boot.ld | $(OUT_DIR)
	$(CXX) $(LFLAGS_BOOT) -T $(BOOTLOADER_DIR)/boot.ld -o $@ $(BOOT_OBJS) $(LIBGCC)
	$(SIZE) $@
	$(DUMP) -d $@ > $(BOOT_DUMP)

$(BOOT_BIN): $(BOOT_ELF)
	$(COPY) -O binary $< $@

$(BOOT_HEX): $(BOOT_ELF) $(REPO_ROOT)/tools/elf2hex.sh
	bash $(REPO_ROOT)/tools/elf2hex.sh $< $@

$(OUT_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OUT_DIR)
