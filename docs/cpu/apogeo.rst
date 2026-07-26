ApogeoRV CPU
============

ZenithSoC uses ApogeoRV as its 32-bit RISC-V processor. The core is imported
as the ``hw/cpu/ApogeoRV`` submodule and connected to the SoC through
``hw/cpu/cpu_complex.sv``.

This page is the hand-off point between the two documentation sets:

* :doc:`cpu_complex` explains how the core is connected to ROM, caches, DDR,
  MMIO, interrupts, and tracing.
* The core's own manual covers the full ISA, CSR, privilege, exception,
  microarchitecture, and native interface details. It is maintained with the
  imported core in ``hw/cpu/ApogeoRV/doc/sphinx/`` and is also available as
  the `ApogeoRV online documentation <https://rv32-apogeo.readthedocs.io/en/latest/>`_.

Current ZenithSoC configuration
-------------------------------

The SoC passes these parameters from ``hw/utils/pkg/soc_parameters.sv``:

.. list-table:: Core integration parameters
   :header-rows: 1
   :widths: 34 18 48

   * - Parameter
     - Value
     - Meaning
   * - ``PREDICTOR_SIZE``
     - 512
     - GShare pattern-table entries
   * - ``BTB_SIZE``
     - 512
     - Branch target buffer entries
   * - ``STORE_BUFFER_SIZE``
     - 4
     - Store-buffer entries
   * - ``INSTRUCTION_BUFFER_SIZE``
     - 8
     - Instructions held between memory and the fetch stage
   * - ``ROB_DEPTH``
     - 32
     - Reorder-buffer entries

The configured instruction-set profile is RV32I with ``M``, ``C``, ``Zicsr``,
``Zfinx``, ``Zba``, ``Zbs``, and the implemented bit-manipulation support
enabled in the checked-in core configuration. The exact floating-point
operation subset and timing are documented in the
`ApogeoRV online documentation <https://rv32-apogeo.readthedocs.io/en/latest/>`_.

When updating the ApogeoRV submodule, review this page and the CPU complex
page at the same time. The submodule's own documentation may describe a
different revision than the one pinned by the ZenithSoC checkout.
