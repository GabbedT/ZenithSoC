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

.PHONY: project retry-synth retry-impl finish-routed synth impl bitstream update-bootloader sim sim_batch sim_gui wave wave-config sweep-impl postroute-sweep physopt-repl

project synth impl bitstream:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs $@ $(JOBS)

retry-synth:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_synth $(JOBS)

retry-impl:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_impl $(JOBS)

# Implementation strategy sweep: tries each configuration on the existing
# synthesized netlist and prints a WNS/TNS comparison. Pass a subset with
# SWEEP_CONFIGS (space-separated names, see sweep_impl.tcl).
sweep-impl:
	SWEEP_JOBS=$(JOBS) $(VIVADO) -mode batch -nojournal -nolog -source sweep_impl.tcl -tclargs $(BUILD_DIR) $(SWEEP_CONFIGS)

# Compare post-route physical optimization directives from one identical
# routed checkpoint. This is much shorter than a full strategy sweep and is
# useful when route already passes most endpoints but post-route phys_opt is
# the remaining timing lever. Results stay in build/vivado/timing_search/.
postroute-sweep:
	$(VIVADO) -mode batch -nojournal -nolog -source postroute_sweep.tcl -tclargs $(BUILD_DIR) $(POSTROUTE_DIRECTIVES)

# Targeted driver replication: re-opens the pre-route physopt checkpoint (or
# builds it from the synth netlist) and runs a dedicated
# phys_opt_design -force_replication_on_nets pass (an option that is exclusive
# of every other physopt option, so it must be its own pass), then route +
# post-route physopt. By default the net list is derived from the worst failing
# setup paths of the current design (nets with PIN_COUNT >= 20). Override with
# FORCE_REPL_NETS (space-separated hierarchical net names, or "@file" to read
# from a file), or "*" for every net with PIN_COUNT >= 25.
physopt-repl:
	$(VIVADO) -mode batch -nojournal -nolog -source physopt_forced_repl.tcl -tclargs $(BUILD_DIR) $(FORCE_REPL_NETS)

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
