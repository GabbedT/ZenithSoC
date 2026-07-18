VIVADO ?= vivado
JOBS ?= 8

.PHONY: project retry-synth retry-impl finish-routed synth impl bitstream

project synth impl bitstream:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs $@ $(JOBS)

retry-synth:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_synth $(JOBS)

retry-impl:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs retry_impl $(JOBS)

finish-routed:
	$(VIVADO) -mode batch -nojournal -nolog -source build.tcl -tclargs finish_routed $(JOBS)
