# ----- Base ISA + extensions ------------------------------------------------
# Leave empty ("") to disable an extension, assign a value to enable it.

ISA_BASE   := rv32i
EXT_M      := m      # mul/div
EXT_C      :=        # compressed -> set "c" to enable
EXT_ZICSR  := zicsr  # CSR (required for zfinx / traps)
EXT_ZBA    := zba    # address generation (sh1add/sh2add/sh3add ...)
EXT_ZBS    := zbs    # single-bit operations (bset/bclr/binv/bext ...)
EXT_ZBB    :=        # basic bit manipulation -> set "zbb" to enable
EXT_ZFINX  := zfinx  # floating-point in GPRs (no F registers)

# ----- Options --------------------------------------------------------------

NO_FDIV    := 1      # 1 = pass -mno-fdiv to GCC and disable fdiv/fsqrt in generator
PRIV       := mu     # "m" or "mu" (Machine + User). No interrupts in either case.

# ----- ISA string construction (single source, three consumers) ------------
# Current default result:
#   rv32im_zfinx_zba_zbs_zicsr
# (= firmware in the repository)
#
# NOTE: comments after EXT_* leave trailing spaces in the value;
# $(strip) removes them.

s_BASE  := $(strip $(ISA_BASE))
s_M     := $(strip $(EXT_M))
s_C     := $(strip $(EXT_C))
s_ZFINX := $(strip $(EXT_ZFINX))
s_ZBA   := $(strip $(EXT_ZBA))
s_ZBS   := $(strip $(EXT_ZBS))
s_ZBB   := $(strip $(EXT_ZBB))
s_ZICSR := $(strip $(EXT_ZICSR))

ISA := $(s_BASE)$(s_M)$(s_C)
ISA := $(ISA)$(if $(s_ZFINX),_$(s_ZFINX),)
ISA := $(ISA)$(if $(s_ZBA),_$(s_ZBA),)
ISA := $(ISA)$(if $(s_ZBS),_$(s_ZBS),)
ISA := $(ISA)$(if $(s_ZBB),_$(s_ZBB),)
ISA := $(ISA)$(if $(s_ZICSR),_$(s_ZICSR),)
ISA := $(strip $(ISA))

# Derived flags ($(strip) removes spaces left by comments in variables)

s_NO_FDIV := $(strip $(NO_FDIV))
s_PRIV    := $(strip $(PRIV))

MARCH_FLAGS := -march=$(ISA) -mabi=ilp32 \
               $(if $(filter 1,$(s_NO_FDIV)),-mno-fdiv,)

GEN_FLAGS := --ext $(ISA) --priv $(s_PRIV) \
             $(if $(filter 1,$(s_NO_FDIV)),--no-fdiv,)

# ----- Tool paths (overridable from environment, defaults match vp/Makefile)
VERILATOR     ?= verilator
RISCV_GCC     ?= riscv32-unknown-elf-g++
RISCV_OBJDUMP ?= riscv32-unknown-elf-objdump
RISCV_OBJCOPY ?= riscv32-unknown-elf-objcopy
SPIKE_DIR     ?=
PYTHON        ?= python3