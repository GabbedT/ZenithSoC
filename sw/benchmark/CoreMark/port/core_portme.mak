# ============================================================================
# CoreMark port makefile for ZenithSoC
#
# Included by coremark/Makefile. Two build targets are driven
# from the top-level Makefile instead:
#
#   make sd   - full image (bootloader + app) for SD boot
#   make sim  - DDR-direct ELF for the Verilator testbench
#
# This file only needs to define the mandatory CoreMark vars.
# ============================================================================

OUTFLAG = -o

CC  = riscv32-unknown-elf-gcc
CXX = riscv32-unknown-elf-g++
LD  = riscv32-unknown-elf-g++
AS  = riscv32-unknown-elf-as

# Flags common to both targets (override from the calling make)

MARCH ?= rv32im_zfinx_zba_zbs_zicsr
MABI  ?= ilp32
OPT   ?= -O3

CFLAGS += -march=$(MARCH) -mabi=$(MABI) $(OPT) \
          -mno-fdiv -ffreestanding -fno-exceptions \
          -funroll-loops -finline-functions -fno-common \
          -nostartfiles \
          -I$(PORT_DIR) \
          -I$(CORE_DIR)

CXXFLAGS += $(CFLAGS) -fno-use-cxa-atexit

LFLAGS_APP = -march=$(MARCH) -mabi=$(MABI) -nostartfiles \
             -fno-exceptions -fno-use-cxa-atexit

PORT_SRCS = $(PORT_DIR)/core_portme.c $(PORT_DIR)/ee_printf_zenith.c
PORT_OBJS = $(addprefix $(OUTDIR)/,$(notdir $(PORT_SRCS:.c=.o)))

EXTRA_CFLAGS =

# CoreMark wants to know which extra source files to compile.
# We handle everything from the top-level Makefile, so leave
# LOAD empty and ITERATIONS / DATA_SIZE are defined by the
# calling make.

LOAD =