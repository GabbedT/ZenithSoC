# Nexys A7 DDR Vivado flow

The batch flow targets the Digilent Nexys A7 DDR 100T
(`xc7a100tcsg324-1`) with Vivado 2024.1.

```bash
cd fpga/nexys_a7
make project   # create the project and regenerate IP
make retry-synth # rerun RTL synthesis while reusing generated IP
make retry-impl  # rerun implementation from the synthesized checkpoint
make finish-routed # write reports/bitstream from an existing routed checkpoint
make synth     # synthesize
make impl      # place and route
make bitstream # generate build/vivado/ZenithSoC_nexys_a7.bit
```

Override the executable when Vivado is not on `PATH`:

```bash
make bitstream VIVADO=/tools/Xilinx/Vivado/2024.1/bin/vivado
```

Reports and generated files are written below `build/vivado/`.

The flow uses `Flow_PerfOptimized_high` for synthesis and
`Performance_ExplorePostRoutePhysOpt` for implementation. Physical
optimization runs with `AggressiveExplore` before and after routing, while the
router uses `NoTimingRelaxation` with TNS cleanup.

The main implementation reports are:

- `timing_impl.rpt`: timing summary and clock-domain details;
- `failing_paths_impl.rpt`: the 500 worst setup paths;
- `utilization_impl.rpt`: device utilization;
- `bus_skew_impl.rpt`: MIG bus-skew checks;
- `drc_impl.rpt`: design-rule checks.

The system clock is 100 MHz and the MIG input clock is 200 MHz. The DDR2
interface runs with a 3333 ps memory period and a 2:1 PHY/controller ratio.

The current full SoC implementation routes and produces a bitstream, but the
100 MHz system-clock domain does not yet close timing. Check the timing reports
before treating a generated image as timing-clean.
