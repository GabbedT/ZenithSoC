CPU complex integration
=======================

The CPU complex is the bridge between ApogeoRV and the rest of ZenithSoC. It
keeps the core's native fetch/load/store interfaces separate from the SoC's
ROM, DDR, and MMIO paths, while also providing the two caches and their
shared DDR-load arbitration.

The integration boundary is implemented in
``hw/cpu/cpu_complex.sv``. The top-level SoC instantiates it from
``hw/ZenithSoC.sv``.

What is inside
--------------

The complex contains:

* the ApogeoRV core;
* an instruction cache for code in the user/DDR region;
* a data cache for cacheable DDR loads and stores;
* routing for boot-ROM fetches and non-cacheable MMIO accesses;
* an arbiter for the instruction-cache and data-cache DDR load channel; and
* synchronization for external, timer, and non-maskable interrupt inputs.

Boot-ROM fetches bypass the I-cache. Once execution moves to
``0x8000_0000`` or above, instruction requests use the I-cache. Data requests
to the MMIO aperture bypass the D-cache; DDR requests use the D-cache and its
four-entry store buffer.

Parameters in the current SoC
-----------------------------

.. list-table:: CPU complex parameters
   :header-rows: 1
   :widths: 34 18 48

   * - Parameter
     - Value
     - Meaning
   * - ``PREDICTOR_SIZE``
     - 512
     - ApogeoRV GShare pattern table
   * - ``BTB_SIZE``
     - 512
     - ApogeoRV branch target buffer
   * - ``STORE_BUFFER_SIZE``
     - 4
     - ApogeoRV store buffer
   * - ``INSTRUCTION_BUFFER_SIZE``
     - 8
     - ApogeoRV instruction buffer
   * - ``ROB_DEPTH``
     - 32
     - ApogeoRV reorder buffer
   * - ``ICACHE_SIZE``
     - 4096 bytes
     - Instruction-cache capacity
   * - ``IBLOCK_SIZE_BYTE``
     - 16 bytes
     - Instruction-cache line size
   * - ``DCACHE_SIZE``
     - 4096 bytes
     - Data-cache capacity
   * - ``DBLOCK_SIZE_BYTE``
     - 16 bytes
     - Data-cache line size

Fetch routing
-------------

The core's fetch request is presented to both memory paths. The complex
selects the boot-ROM path for addresses through ``BOOT_END`` and the cached
DDR path for addresses at or above ``USER_MEMORY_REGION_START``. Cache
invalidation is forwarded with the request, so branch recovery and trap
redirects cannot leave stale instructions in the frontend.

The I-cache and D-cache share one DDR load channel. Data-cache requests have
priority when both caches request a refill. Once a request has been selected,
the arbiter keeps the response associated with that cache until the transfer
finishes. This is why an instruction refill may wait behind a data refill, but
the two caches never consume one another's response.

Data and MMIO routing
---------------------

The D-cache classifies requests using the ApogeoRV memory regions:

* addresses from ``0x8000_0000`` upward use the DDR-facing cache path;
* addresses in the MMIO aperture use the direct ``io_load_channel`` or
  ``io_store_channel`` path; and
* boot memory is not a writable data target.

The complex translates DDR addresses by removing
``USER_MEMORY_REGION_START`` before sending them to the DDR controller. MMIO
addresses remain in the SoC address space for the bus router to decode.

External interface
------------------

The main ports are deliberately small:

* ``clk_i`` and ``rst_n_i`` provide the core clock and reset;
* ``halt_i`` requests the ApogeoRV halt-and-drain sequence;
* ``rom_fetch_channel`` connects to boot memory;
* ``ddr_load_channel`` and ``ddr_store_channel`` connect to the DDR path;
* ``io_load_channel`` and ``io_store_channel`` connect to the MMIO fabric;
* ``gen_interrupt_i``, ``timer_interrupt_i``, and ``nmsk_interrupt_i`` carry
  interrupt requests; and
* ``interrupt_vector_i`` plus ``interrupt_ackn_o`` complete the interrupt
  handshake.

The core's ``halted_o`` status is consumed inside the core integration and is
not currently exposed as a ZenithSoC top-level pin. The halt behavior itself,
including interrupt service while draining, is described in the ApogeoRV
manual linked from :doc:`apogeo`.

Related pages
-------------

* :doc:`apogeo` — core hand-off and configuration;
* :doc:`instruction_cache` and :doc:`data_cache` — cache-level details; and
* :doc:`../architecture/memory_map` — address regions and interrupt vectors.
