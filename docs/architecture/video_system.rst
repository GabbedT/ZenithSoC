Video System
============

The Video System is the general future-development area for graphics,
display, and high-bandwidth memory movement. These blocks are not part of the
current ``ZenithSoC`` top-level integration; this page describes the intended
architecture and the current RTL status.

Planned structure
-----------------

The planned data path is:

.. code-block:: text

   CPU / GPU / DMA engine
             |
             v
      Multi-master bus  --->  DDR / framebuffer memory
             |
             v
             VGA output

The multi-master bus is the arbitration point for CPU, GPU, and DMA traffic.
The VGA block consumes display data from a framebuffer or line-buffer path.
The exact bus protocol, ownership rules, framebuffer layout, and arbitration
policy remain to be defined.

Components
----------

.. list-table:: Video System roadmap
   :header-rows: 1
   :widths: 22 20 58

   * - Component
     - Status
     - Scope
   * - VGA
     - RTL present, not integrated
     - Standalone timing generator, line buffer, RGB output, and sprite path.
       See :doc:`../peripherals/vga` for its current module interface.
   * - GPU
     - Future development
     - Graphics accelerator for rendering or composition before display.
       No current top-level module, address window, or interrupt assignment.
   * - DMA engine
     - Future development
     - High-bandwidth transfers between memory and video or peripheral
       buffers. No current top-level module or descriptor interface.
   * - Multi-master bus
     - Future development
     - Arbitration and routing for CPU, GPU, and DMA masters. The current
       interconnect has a single CPU-side master path.

VGA status
----------

The existing VGA RTL is under ``hw/io/vga/`` and is not included by the
current ``hw/_zenithSoC.f`` file list. It is not instantiated by
``hw/ZenithSoC.sv``, and the top level exposes no VGA pins, MMIO window, or
interrupt vector. The standalone block supports 640x480 and 800x600 timing,
a line buffer, RGB output, and a small sprite path.

.. toctree::
   :maxdepth: 1

   ../peripherals/vga

Integration milestones
----------------------

Before the Video System becomes part of the integrated hardware, the project
needs to define and verify:

* the multi-master bus protocol and arbitration policy;
* DMA ownership, descriptors, burst behavior, and memory coherency rules;
* GPU command and framebuffer interfaces;
* VGA framebuffer ownership and external pin timing;
* MMIO windows, interrupt vectors, and software drivers;
* VP/full-SoC tests covering concurrent CPU, GPU, and DMA traffic.
