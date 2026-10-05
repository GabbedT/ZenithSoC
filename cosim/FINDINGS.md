# Findings exposed by the stronger co-simulation

These are diagnostic regressions, not changes to CPU RTL. All four minimal
firmware programs in `cases/` terminate successfully on standalone Spike with
`rv32imc_zfinx_zba_zbs_zicsr`. The observations below use RTL assertions and the
default co-simulation CPU/cache geometry. Commands run from `cosim/` after the
toolchain environment has been set up.

## 1. Loads targeting x0 raise illegal-instruction exceptions

```bash
make run-case CASE=load_x0 MEM_RANDOM=0
```

The first test instruction is `lw x0, 0(x31)` at a valid, aligned data address.
The DUT raises exception vector 2. Spike performs the load and discards its
result. The destination register must not suppress the architectural access.

Root cause: the `LOAD` case in
[`integer_decoder.sv`](../hw/cpu/ApogeoRV/hw/front_end/decoder/integer_decoder.sv)
sets `exception_generated = (instr_i.I.reg_dest == riscv32::X0)`.
The random suite also exposes this with LBU, LHU and LW. The old generator
excluded x0 from its ordinary operand pool.

## 2. Floating-point rounding mode is not honored

```bash
make run-case CASE=rounding_rdn MEM_RANDOM=0
```

`fcvt.w.s x6, x5, rdn`, with x5 = `0x3fc00000` (1.5), produces:

| Model | x6 |
| --- | --- |
| Spike | `0x00000001` |
| DUT | `0x00000002` |

[`float_converter.sv`](../hw/cpu/ApogeoRV/hw/back_end/exu/fpu_subm/float_converter.sv)
computes conversion rounding without a rounding-mode input.
[`float_rounding_unit.sv`](../hw/cpu/ApogeoRV/hw/back_end/exu/fpu_subm/float_rounding_unit.sv)
also implements nearest/even without a mode input. Random tests expose additional
rounding mismatches in FADD, FMUL and integer-to-float conversions; those are not
all individually reduced here. The old generator always requested RNE.

## 3. A legal compressed hint raises an illegal-instruction exception

```bash
make run-case CASE=compressed_hint MEM_RANDOM=0
```

`c.addi x12, 0` raises vector 2 on the DUT; Spike executes the hint without
changing x12. In the C.ADDI case,
[`decompressor.sv`](../hw/cpu/ApogeoRV/hw/front_end/decoder/decompressor.sv)
marks a zero immediate as illegal. The new generator explicitly emits compressed
forms, including this hint, instead of relying solely on assembler compression.

## 4. DDR arbiter changes a stalled request

```bash
make run-case CASE=ddr_backpressure MEM_RANDOM=1 MEM_SEED=0 \
  MEM_LATENCY_MIN=2 MEM_LATENCY_MAX=40 MEM_STALL_PERCENT=60
```

The existing assertion in
[`cache_ddr_interface.sv`](../hw/memory/ddr/cache_ddr_interface.sv)
fails with `Cache DDR request changed while stalled`. Its combinational arbiter
selects reads whenever the read queue becomes nonempty, even if it had already
presented an unaccepted write.

A waveform from the random campaign (seed 4, memory seed 2700688153,
latency 1..40, stall 60%) confirms that `trx_req` stays high and `ready` stays low
while `trx_type` changes from write to read and `trx_addr` from `0xdb00` to
`0x50`. This is a change of transaction, not merely irrelevant read-data bits.
The old DDR model held `ready` high, so it could not exercise this assertion.

## Validation record

- Original baseline: 8 generator tests passed; seed 0 / 300 blocks completed
  33,762 lockstep comparisons with final memory agreement.
- Updated environment: 19 unit/integration test methods passed, including ten
  generated ELF executions on standalone Spike across all profiles, RV32C on/off,
  memory-address invariants, coverage checks, and injected runner failures.
- New random integer/control campaign, fixed memory timing: 15/16 seeds passed
  (500 blocks each); seed 15 exposed the compressed hint defect.
- Same instruction classes with randomized DDR timing (latency 1..40, stall 60%):
  8/24 seeds passed; 16 hit the existing DDR request-stability assertion. Every
  generated program passed standalone Spike first.
- Early mixed memory campaign: 3/10 passed; 7 exposed loads to x0. Early FP
  campaign: 2/8 passed; 6 exposed rounding differences.
- All four minimal cases above pass standalone Spike and fail on the DUT for
  the stated reason. Cases are retained as ordinary failing tests, with no
  expected-failure override in the runner.
- `SOC_CONFIG=1 COVERAGE=1`: four integer/control seeds passed; all four
  structural coverage files were successfully merged with `verilator_coverage`.
- Actual simulator checks confirmed that retire/cycle limits return status 3
  without PASS. A 5000-cycle DDR-latency run passed with final memory agreement,
  exercising the new completion-based store drain.

One additional failure remains unclassified: with `SOC_CONFIG=1`, seed 2,
100 blocks, `CLASS=arith,mem,branch,ctrl` and `MEM_RANDOM=0`, retirement stops
after a FENCE at `0x800002d0`. Standalone Spike passes; the DUT reaches the
200,000-cycle no-retirement watchdog. This is retained as a failure, but its
root cause has not been isolated between the SoC cache path and the changed
memory timing model. The corresponding default-cache seed passes. Reproduce
with `make regress SOC_CONFIG=1 N=1 START_SEED=2 PROG_LEN=100
CLASS=arith,mem,branch,ctrl MEM_RANDOM=0 GEN_SEED=0` (on one shell line).

Only co-simulation code and its firmware/linker support were changed. No FPGA
implementation was rerun for these simulation-only edits. The existing routed
post-physical-optimization report read at baseline had WNS +0.005 ns and TNS
0.000 ns; that report is historical evidence, not a new timing measurement.
