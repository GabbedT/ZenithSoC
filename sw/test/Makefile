# ======================================================================
# ZenithSoC bare-metal test build.
#
# Select a test codebase with TEST=<directory under tests/>. Each test owns
# a test.mk file that lists the library drivers it needs.
# ======================================================================

SHELL := /bin/bash

TEST ?= io_direct_interrupt
TRACE ?= 0
MAX_CYCLES ?= 50000000

ROOT      := $(abspath ../..)
TEST_DIR  := tests/$(TEST)
COMMON    := common
BUILD     := out/$(TEST)/$(CASE)
ELF       := $(BUILD)/firmware.elf
DUMP      := $(BUILD)/firmware.dump
BOOT_ELF  := $(BUILD)/boot_rom.elf
VERILATOR := $(ROOT)/tb/verilator

ifeq ($(wildcard $(TEST_DIR)/test.mk),)
$(error Unknown TEST '$(TEST)': missing $(TEST_DIR)/test.mk)
endif

include $(TEST_DIR)/test.mk

RISCV_PREFIX ?= riscv32-unknown-elf-

CXX     := $(RISCV_PREFIX)g++
AS      := $(RISCV_PREFIX)as
LD      := $(RISCV_PREFIX)g++
OBJDUMP := $(RISCV_PREFIX)objdump
SIZE    := $(RISCV_PREFIX)size

MARCH ?= rv32im_zfinx_zba_zbs_zicsr
MABI  ?= ilp32

CPPFLAGS := -I$(ROOT)/sw/lib -I$(COMMON) -I$(TEST_DIR)
CPPFLAGS += $(CASE_CPPFLAGS)
CXXFLAGS := \
	-std=c++17 -march=$(MARCH) -mabi=$(MABI) -mno-fdiv \
	-O2 -g3 -ffreestanding -fno-builtin -fno-common \
	-fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-fno-threadsafe-statics -ffunction-sections -fdata-sections \
	-msmall-data-limit=0 -Wall -Wextra
ASFLAGS := -march=$(MARCH) -mabi=$(MABI)
LDFLAGS := \
	-march=$(MARCH) -mabi=$(MABI) -nostartfiles -nostdlib \
	-Wl,--gc-sections -T$(COMMON)/linker.ld

LIBGCC := $(shell $(CXX) -march=$(MARCH) -mabi=$(MABI) -print-libgcc-file-name)

TEST_CPP   := $(sort $(wildcard $(TEST_DIR)/*.cpp))
COMMON_CPP := $(sort $(wildcard $(COMMON)/*.cpp))

TEST_OBJS   := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD)/test_%.o,$(TEST_CPP))
COMMON_OBJS := $(patsubst $(COMMON)/%.cpp,$(BUILD)/common_%.o,$(COMMON_CPP))
SHARED_OBJS := $(addprefix $(BUILD)/shared_,$(addsuffix .o,$(basename $(notdir $(SHARED_CPP)))))
DRIVER_OBJS := $(addprefix $(BUILD)/driver_,$(addsuffix .o,$(DRIVERS)))
OBJS        := $(BUILD)/startup.o $(TEST_OBJS) $(COMMON_OBJS) $(SHARED_OBJS) $(DRIVER_OBJS)

CPPFLAGS += $(addprefix -I,$(SHARED_INCLUDE))

.PHONY: all build run regress clean list

all: build

build: $(ELF) $(BOOT_ELF) $(DUMP)
	@echo "=== [SW-TEST] built $(TEST) -> $(ELF) ==="

run: build
	$(MAKE) -C $(VERILATOR) run \
		DDR=$(abspath $(ELF)) \
		BOOT=$(abspath $(BOOT_ELF)) \
		TRACE=$(TRACE) MAX_CYCLES=$(MAX_CYCLES)

regress:
	@set -e; \
	for case_name in $(CASES); do \
		$(MAKE) run TEST=$(TEST) CASE=$$case_name TRACE=$(TRACE) MAX_CYCLES=$(MAX_CYCLES); \
	done

list:
	@find tests -mindepth 1 -maxdepth 1 -type d -printf '%f\n' | sort

$(ELF): $(OBJS) $(COMMON)/linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBGCC)
	$(SIZE) $@

$(DUMP): $(ELF)
	$(OBJDUMP) -d -S --demangle $< > $@

$(BOOT_ELF): $(BUILD)/boot_rom.o $(COMMON)/boot_rom.ld
	$(RISCV_PREFIX)ld -T$(COMMON)/boot_rom.ld -o $@ $<

$(BUILD)/startup.o: $(COMMON)/startup.S | $(BUILD)
	$(CXX) -x assembler-with-cpp $(ASFLAGS) $(STARTUP_DEFINES) -c $< -o $@

$(BUILD)/boot_rom.o: $(COMMON)/boot_rom.S | $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD)/test_%.o: $(TEST_DIR)/%.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/common_%.o: $(COMMON)/%.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/shared_%.o: $(SHARED_CPP) | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(filter %/$*.cpp,$(SHARED_CPP)) -o $@

$(BUILD)/driver_%.o: $(ROOT)/sw/src/%.cpp | $(BUILD)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	@mkdir -p $@

clean:
	rm -rf out
