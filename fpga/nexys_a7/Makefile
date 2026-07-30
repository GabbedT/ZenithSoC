VIVADO ?= vivado
UPDATEMEM ?= updatemem
XSIM ?= xsim
JOBS ?= 8

BUILD_DIR := ../../build/vivado
BOOTLOADER_HEX ?= ../../sw/benchmark/CoreMark/out/bootloader.hex
COREMARK_DIR := ../../sw/benchmark/CoreMark
DEFAULT_BOOTLOADER_ELF := $(COREMARK_DIR)/out/bootloader.elf
BOOTLOADER_ELF ?= $(DEFAULT_BOOTLOADER_ELF)

ifeq ($(abspath $(BOOTLOADER_ELF)),$(abspath $(DEFAULT_BOOTLOADER_ELF)))
BOOTLOADER_BUILD_DIR ?= $(COREMARK_DIR)
BOOTLOADER_BUILD_TARGET ?= fpga
else
BOOTLOADER_BUILD_DIR ?=
BOOTLOADER_BUILD_TARGET ?=
endif
BOOT_MMI := $(BUILD_DIR)/bootloader.mmi
DEPLOY_BIT := $(BUILD_DIR)/ZenithSoC_nexys_a7.bit
BASE_BIT := $(BUILD_DIR)/ZenithSoC_nexys_a7_base.bit
BOOTLOADER_BIT ?= $(DEPLOY_BIT)
BOOTLOADER_TMP := $(basename $(BOOTLOADER_BIT))_tmp.bit

# project_config.tcl reads this exported, absolute path for full synthesis and
# simulation builds. Override BOOTLOADER_HEX for a different boot program.
export ZENITH_BOOTLOADER_HEX := $(abspath $(BOOTLOADER_HEX))

.PHONY: project retry-synth retry-impl finish-routed synth impl bitstream update-bootloader sim sim_batch sim_gui wave wave-config

project synth impl bitstream:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs $@ $(JOBS)

retry-synth:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_synth $(JOBS)

retry-impl:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_impl $(JOBS)

finish-routed:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs finish_routed $(JOBS)

update-bootloader:
ifneq ($(strip $(BOOTLOADER_BUILD_DIR)),)
	$(MAKE) -C "$(BOOTLOADER_BUILD_DIR)" $(BOOTLOADER_BUILD_TARGET)
endif
	@test -s "$(BASE_BIT)" || { \
		test -s "$(DEPLOY_BIT)" || { echo "Bitstream not found: $(DEPLOY_BIT); run 'make bitstream' first." >&2; exit 1; }; \
		cp -p "$(DEPLOY_BIT)" "$(BASE_BIT)"; \
		echo "Preserved base bitstream: $(abspath $(BASE_BIT))"; \
	}
	@test -s "$(BOOT_MMI)" || { echo "Bootloader memory map not found: $(BOOT_MMI); run 'make bitstream' first." >&2; exit 1; }
	@test -s "$(BOOTLOADER_ELF)" || { echo "Bootloader ELF not found: $(BOOTLOADER_ELF)" >&2; exit 1; }
	$(RM) "$(BOOTLOADER_TMP)"
	$(UPDATEMEM) -meminfo "$(BOOT_MMI)" -data "$(BOOTLOADER_ELF)" -bit "$(BASE_BIT)" -proc zenith_cpu -out "$(BOOTLOADER_TMP)" -force
	@test -s "$(BOOTLOADER_TMP)" || { echo "UpdateMEM did not create a bitstream." >&2; exit 1; }
	@if cmp -s -i 256 "$(BASE_BIT)" "$(BOOTLOADER_TMP)"; then \
		echo "WARNING: Bitstream payload is unchanged; verify that $(abspath $(BOOTLOADER_ELF)) was rebuilt." >&2; \
	else \
		echo "Bitstream payload changed."; \
	fi
	mv -f "$(BOOTLOADER_TMP)" "$(BOOTLOADER_BIT)"
	@echo "Bootloader ELF: $(abspath $(BOOTLOADER_ELF))"
	@echo "Program this bitstream: $(abspath $(BOOTLOADER_BIT))"


SIM_RUNTIME ?= 1ms
SIM_WDB := ../../build/vivado/simulation/project/ZenithSoC_sim.sim/sim_1/behav/xsim/soc_testbench_behav.wdb
SIM_WCFG := ../../build/vivado/simulation/project/ZenithSoC_sim.sim/sim_1/behav/xsim/soc_testbench_behav.wcfg

sim: sim_batch

sim_batch:
	$(VIVADO) -mode batch -nojournal -nolog -source simulate.tcl -tclargs $(JOBS) $(SIM_RUNTIME)

wave-config:
	@test -f "$(SIM_WDB)" || { echo "Waveform database not found; run 'make sim' first." >&2; exit 1; }
	$(VIVADO) -mode batch -nojournal -nolog -source waveform_wdb.tcl -tclargs "$(abspath $(SIM_WDB))" "$(abspath $(SIM_WCFG))"

wave: wave-config
	$(XSIM) --gui "$(SIM_WDB)" -view "$(SIM_WCFG)"

sim_gui:
	$(VIVADO) -mode gui -source simulate.tcl -tclargs $(JOBS) $(SIM_RUNTIME) gui
