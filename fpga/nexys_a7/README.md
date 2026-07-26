# Nexys A7 DDR Vivado flow

The batch flow targets the Digilent Nexys A7 DDR 100T
(`xc7a100tcsg324-1`) with Vivado 2024.1.

```bash
cd fpga/nexys_a7
make project       # create the project and regenerate IP
make retry-synth   # rerun RTL synthesis while reusing generated IP
make retry-impl    # rerun implementation from the synthesized checkpoint
make finish-routed # write reports/bitstream from an existing routed checkpoint
make synth         # synthesize
make impl          # place and route
make bitstream     # generate build/vivado/ZenithSoC_nexys_a7.bit
make update-bootloader # patch a new bootloader into an existing bitstream
make sim           # run the accurate MIG + DDR2 + SD-card XSim testbench
make sim_gui       # open the simulation in the Vivado GUI
make wave          # open the captured waveform database in XSim
```

The simulation target uses the generated clock wizard and MIG controller, the
Micron DDR2 component model in `tb/ddr_model`, and the SD-card model maintained
in `vp/blocks/sd/rtl`.  Testbench sources are added explicitly to the Vivado
simulation fileset; `SoC_testbench.sv` therefore contains no source includes or
machine-specific paths.  The default simulated interval is 1 ms and can be
overridden, for example:

```bash
make sim SIM_RUNTIME=5ms JOBS=8
```

`make sim_gui` uses the same project and waveform setup but leaves the
simulation open for interactive use.  After a batch run, `make wave` creates a
curated waveform configuration and opens the captured database; it requires a
previous `make sim` run. Preferrebly one should do a `make sim_gui`, add the required signals
and save the waveform file, then exit and execute `make sim`.

The XSim project, log, waveform database, and trace files are written below
`build/vivado/simulation/`.  The simulation define set intentionally includes
`_VIVADO_` and excludes `VERILATOR`, so `ZenithSoC` instantiates the real MIG
path rather than its fast behavioural DDR replacement.

The simulation depends on the boot image and sine-table
initialization file configured in `project_config.tcl`, from here change the bootloader hex path.  Build those images
before running XSim if they are not already present.

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


## Updating only the bootloader

A full `make bitstream` generates both the base bitstream and
`build/vivado/bootloader.mmi`. The MMI records the placed locations of the four
block RAMs that form the 16 KiB boot memory. The flow is not tied to CoreMark;
the CoreMark paths are only defaults. To build a base bitstream with any boot
program, provide its byte-oriented initialization file:

```bash
make bitstream BOOTLOADER_HEX=/absolute/path/to/bootloader.hex
```

After changing only that boot program, provide the corresponding ELF:

```bash
make update-bootloader BOOTLOADER_ELF=/absolute/path/to/bootloader.elf
```

The software must already have been built; the FPGA target does not invoke a
software-project-specific Makefile. It writes
`build/vivado/ZenithSoC_nexys_a7_bootloader.bit` with `updatemem`; synthesis,
placement, and routing are not repeated. Override the tool or output path with
`UPDATEMEM=/path/to/updatemem` or `BOOTLOADER_BIT=/path/to/output.bit`.

The two bootloader files have different roles:

- `bootloader.hex` is a byte-oriented initialization file read by
  `$readmemh` while Vivado synthesizes the inferred boot RAM. It determines the
  contents of a newly implemented base bitstream.
- `bootloader.elf` contains the same program plus its load-address metadata.
  UpdateMEM uses those addresses with `bootloader.mmi` to modify only the boot
  RAM initialization data in an existing bitstream.

Run a full `make bitstream` again after RTL, constraints, FPGA part, boot-memory
size, or implementation settings change. The regenerated MMI is tied to that
specific placed bitstream.