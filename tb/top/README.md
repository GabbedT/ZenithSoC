# ZenithSoC – XSim top-level testbench

Xilinx Vivado/XSim testbench for the **full** `ZenithSoC` with the physical
DDR2 interface (`DDR_MEMORY=1`): the MIG configuration, the Micron MT47H64M16
DDR2 model, the SD-card protocol/PHY model (`sd_top` + `bram_whishbone`
storage) and the same peripheral loopbacks as the Verilator flow.

## What it does

- Drives `clk_i` at 100 MHz (`#5ns` half-period) and de-asserts reset after 40
  cycles.
- Preloads the SD storage from `coremark_sd_words.hex` (stored as
  `$readmemh` into `sd_storage.mem`).
- Traps the legacy UART TX path into `output_trace.txt` and stdout.
- Stops on `dut.ApogeoRV.system_cpu.exception` or after `TEST_TIMEOUT` (1 s),
  then prints the performance summary and calls `$finish`.

## Performance counters

`SoC_testbench.sv` includes the same shared counter module used by the
Verilator flow:

```systemverilog
`include "../verilator/perf_counters.sv"
...
perf_counters perf_unit (.clk_i(clk_i), .rst_n_i(rst_n_i));
```

`perf_unit.report()` is called in the `cpu_stopped` process just before
`$finish`, printing the same `ZENITHSOC PERFORMANCE SUMMARY` with the
per-hazard stall breakdown, cache hit rates and the BOTTLENECK line. The
module is fully testbench-side: it taps DUT signals via hierarchical
references (`dut.ApogeoRV...`) and requires no RTL changes. See
[`tb/verilator/README.md`](../verilator/README.md) for the full list of
counter categories and how to read the report.

## Files

| File | Role |
|------|------|
| `SoC_testbench.sv` | top-level XSim harness: clock/reset, DUT, DDR2/SD models, UART trace, perf report |
| `../verilator/perf_counters.sv` | shared bottleneck-analysis module (included, not duplicated) |

## Run

Run from Vivado: add `SoC_testbench.sv` as the top simulation source together
with the RTL filelist used for the Nexys A7 build (or run `tb/top` through
your preferred XSim project flow), then run behavioral simulation. The
performance summary is printed on stdout at the end.
