Hardware documentation status
=============================

This page records the integration status checked against the current
``hw/ZenithSoC.sv`` and ``hw/_zenithSoC.f``. Those files, together with
``hw/utils/pkg/soc_parameters.sv``, are the source of truth when this page and
the detailed reference pages disagree.

Integrated in the current SoC
-----------------------------

The top-level configuration defines and instantiates the following blocks:

* ApogeoRV through ``hw/cpu/cpu_complex.sv``;
* instruction and data cache complexes, on-chip memory, and DDR interfaces;
* the AXI-style bus network;
* UART, timer, GPIO, SPI, Ethernet, PRNG, APU, SD, and trace unit;
* the nine-source interrupt controller, including the bus-error source.

The default build has one instance of each listed peripheral. The GPIO
interface is eight one-bit GPIO instances exposed as ``pin_io[0][7:0]``.

Present in RTL but not integrated
---------------------------------

The VGA implementation under ``hw/io/vga/`` has its own controller, line
buffer, sprite support, and register block. It is not currently included by
``hw/_zenithSoC.f``, is not instantiated by ``ZenithSoC.sv``, and has no VGA
ports on the current SoC top level. Its page is therefore listed separately
under :doc:`../peripherals/vga`.

Recent hardware changes reflected here
---------------------------------------

The documentation was reviewed against the current checkout and recent
hardware history, including:

* the ROB-owned allocation and tag path between the scheduler and reorder
  buffer;
* the four-state compressed-instruction fetch stream and its halfword
  redirect handling;
* the BTB/GShare predictor split, prediction FIFO, and compressed-aware PC
  hashing;
* the halt-and-drain state machine, including interrupt service during a
  halt request; and
* the retimed Zfinx FPU, including current subnormal and gradual-underflow
  handling;
* the package-based interrupt indices and the direct trace-to-UART TX path;
* the SD controller's active-high ``sd_reset_o`` output and current clock
  choices;
* the current cache/DDR integration boundary;
* the pipelined I-cache fetch path: 16-entry request FIFO, retained-block
  serving, speculative next-block pre-read, and early-restart refill
  delivery;
* the current full-SoC testbench and SD-boot flow.

When changing a register package, memory-map parameter, top-level port, or
filelist, update this status page and the affected reference page in the same
change.
