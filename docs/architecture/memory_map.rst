Memory map and interrupt map
============================

The addresses below are derived from
``hw/cpu/ApogeoRV/hw/inc/headers/apogeo_memory_map.svh`` and
``hw/utils/pkg/soc_parameters.sv``. Peripheral registers are 32-bit aligned;
each peripheral is assigned a ``0x2000`` address window even when it uses only
a few registers.

CPU regions
-----------

.. list-table:: CPU regions
   :header-rows: 1
   :widths: 28 30 42

   * - Region
     - Address range
     - Notes
   * - Boot ROM
     - ``0x0000_0000`` – ``0x0000_3FFF``
     - 16 KiB, M-mode, non-writable
   * - MMIO aperture
     - ``0x0000_4000`` – ``0x0800_3FFF``
     - M-mode, non-cacheable
   * - Private/protected memory
     - ``0x0000_0000`` – ``0x7FFF_FFFF``
     - Includes boot and MMIO regions
   * - User/DDR memory
     - ``0x8000_0000`` – ``0xFFFF_FFFF``
     - Cacheable general-purpose region

The CPU starts at ``0x0000_0000``. The boot flow then transfers execution to
DDR at ``0x8000_0000``. MMIO transactions must not be cached.

Peripheral windows
------------------

.. list-table:: Peripheral windows
   :header-rows: 1
   :widths: 24 18 18

   * - Device
     - Base
     - High address
   * - UART
     - ``0x0000_4000``
     - ``0x0000_400C``
   * - Timer
     - ``0x0000_6000``
     - ``0x0000_601C``
   * - GPIO
     - ``0x0000_8000``
     - ``0x0000_8014``
   * - SPI
     - ``0x0000_A000``
     - ``0x0000_A010``
   * - Ethernet
     - ``0x0000_C000``
     - ``0x0000_C09C``
   * - PRNG
     - ``0x0000_E000``
     - ``0x0000_E00C``
   * - APU
     - ``0x0001_0000``
     - ``0x0001_20A0``
   * - Non-cacheable memory
     - ``0x0001_2400``
     - ``0x0001_2C00``
   * - SD controller
     - ``0x0001_4400``
     - ``0x0001_441C``
   * - Trace unit
     - ``0x0001_6400``
     - ``0x0001_640C``


Register references
-------------------

The device pages describe register offsets relative to these bases. The
register enumerations are kept in the shared packages:

* ``hw/utils/pkg/uart_pkg.sv``
* ``hw/utils/pkg/spi_pkg.sv``
* ``hw/utils/pkg/sd_pkg.sv``
* ``hw/utils/pkg/ethernet_pkg.sv``
* ``hw/utils/pkg/apu_pkg.sv``
* ``hw/utils/pkg/trace_unit_pkg.sv``

Interrupt vectors
-----------------

The interrupt controller uses the following vector indices. The CPU receives
timer interrupts separately and treats bus errors as the non-maskable input.

.. list-table:: Interrupt vectors
   :header-rows: 1
   :widths: 30 12 58

   * - Source
     - Vector
     - Description
   * - Trace unit
     - 0
     - Trace buffer event
   * - SD controller
     - 1
     - SD/FIFO/controller event
   * - APU
     - 2
     - Audio capture event
   * - Ethernet
     - 3
     - MAC/PHY event
   * - SPI
     - 4
     - SPI transfer event
   * - GPIO
     - 5
     - GPIO input event
   * - UART
     - 6
     - UART event
   * - Timer
     - 7
     - Timer interrupt
   * - Bus error
     - 8
     - AXI read/write error

These values are the ``*_IRQ`` constants in ``soc_parameters``; they are not
derived from peripheral declaration order.
