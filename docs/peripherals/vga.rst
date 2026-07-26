VGA controller (RTL under development)
=======================================

The repository contains a VGA controller under ``hw/io/vga/``. It supports
640x480 and 800x600 timing, a line buffer, RGB output, and a small sprite
path. The register implementation also contains framebuffer auto-increment,
frame/event status, and sprite table access.

Signal interface
-----------------

Port directions are relative to the standalone ``vga`` module. Its register
signals use the same local SoC request/response convention as the integrated
peripherals, but the block currently has no top-level integration.

.. list-table:: VGA module signals
   :header-rows: 1
   :widths: 34 12 18 36

   * - Signal
     - Direction
     - Width
     - Description
   * - ``clk_i``
     - Input
     - 1
     - System clock.
   * - ``rst_n_i``
     - Input
     - 1
     - Active-low reset.
   * - ``interrupt_o``
     - Output
     - 1
     - Frame/event interrupt request.
   * - ``write_i`` / ``read_i``
     - Input
     - 1 each
     - Register or framebuffer write/read requests.
   * - ``write_address_i`` / ``read_address_i``
     - Input
     - ``$clog2(BUFFER_SIZE)+1`` each
     - Register or framebuffer word address.
   * - ``write_data_i[31:0]``
     - Input
     - 32
     - Register or pixel write data.
   * - ``write_done_o`` / ``read_done_o``
     - Output
     - 1 each
     - Completion responses; each mirrors its request in the current RTL.
   * - ``write_error_o`` / ``read_error_o``
     - Output
     - 1 each
     - Address-range error responses.
   * - ``hsync_o`` / ``vsync_o``
     - Output
     - 1 each
     - Horizontal and vertical VGA synchronization pulses.
   * - ``red_o`` / ``green_o`` / ``blue_o``
     - Output
     - 4 each
     - Four-bit RGB video channels.

Integration status
------------------

This block is not part of the current ``ZenithSoC`` top level. It is absent
from ``hw/_zenithSoC.f``, is not instantiated in ``hw/ZenithSoC.sv``, and no
VGA pins are exposed by that module. Consequently it has no current SoC MMIO
base address or interrupt vector and should not be used by firmware targeting
the integrated design.

RTL reference
-------------

The standalone module is ``hw/io/vga/vga.sv`` and its register package is
``hw/utils/pkg/vga_pkg.sv``. Before documenting it as an integrated
peripheral, connect it to the top-level filelist, add the external ports and
address/interrupt entries, and add a VP or full-SoC test.
