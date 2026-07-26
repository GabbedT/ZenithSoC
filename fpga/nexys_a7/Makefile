VIVADO ?= vivado
XSIM ?= xsim
JOBS ?= 8

.PHONY: project retry-synth retry-impl finish-routed synth impl bitstream sim sim_batch sim_gui wave wave-config

project synth impl bitstream:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs $@ $(JOBS)

retry-synth:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_synth $(JOBS)

retry-impl:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_impl $(JOBS)

finish-routed:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs finish_routed $(JOBS)



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
