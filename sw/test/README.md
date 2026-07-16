# ZenithSoC software tests

This directory contains self-checking bare-metal tests intended for the full
SoC Verilator environment. Every test codebase lives under `tests/<name>/` and
declares the library drivers it needs in `test.mk`.

```bash
source setenv.sh
cd sw/test
make build TEST=io_direct_interrupt
make run TEST=io_direct_interrupt CASE=timer
make regress TEST=io_direct_interrupt
make regress TEST=io_vectored_interrupt
```

`make run` builds one isolated interrupt case, its boot ROM, and the Verilator
model from `tb/verilator`, and terminates through `tohost`. A zero exit code is
reported as `[ZTB] PASS`. UART output is also captured in
`tb/verilator/out/stdout.txt`. The software test runner disables the instruction
trace and uses a 50-million-cycle guard by default; both can be overridden with
`TRACE=1` and `MAX_CYCLES=<count>`.

`make regress TEST=<suite>` runs every case declared by that suite. Keeping one
interrupt per firmware prevents one peripheral state from contaminating the
next result and gives each case its own output directory under `out/`.

To add another codebase, create `tests/<name>/`, put its C++ files there, and
add a `test.mk` containing the required driver names, for example:

```make
DRIVERS := UART Timer
```

The common startup, linker scripts, trap dispatcher, boot ROM, driver objects,
ELF and disassembly generation are reused automatically.

The `io_direct_interrupt` and `io_vectored_interrupt` suites cover timer compare,
UART TX-empty/TX-done/RX-done/RX-full, GPIO rising/falling/both/high triggers,
SPI transaction completion, APU capture threshold, and trace-buffer-empty
interrupts. The vectored suite also verifies that `mtvec` routes each source to
`BASE + 4 * cause`. The Verilator wrapper supplies small deterministic loopbacks
for UART, GPIO, and SPI pins.

Ethernet and SD test functions are compiled in the codebase but deliberately
not called by `main`, because they require protocol models. They can be enabled
when the corresponding model is connected to the full-SoC wrapper.
