.. ZenithSoC documentation master file

===================================
ZenithSoC Documentation
===================================

ZenithSoC is a 32-bit RISC-V system-on-chip built around the ApogeoRV CPU,
separate instruction and data caches, an AXI-style memory-mapped interconnect,
DDR memory, and a collection of communication, storage, timing, tracing, and
audio peripherals.

Start with :doc:`architecture/overview` for the shape of the system. The
:doc:`architecture/hardware_status` page distinguishes RTL that is connected
to the current ``ZenithSoC`` top level from blocks that are present in the
repository but are not yet integrated.

.. toctree::
   :maxdepth: 2
   :caption: Architecture

   architecture/overview
   architecture/hardware_status
   architecture/memory_map
   architecture/interconnect

.. toctree::
   :maxdepth: 2
   :caption: CPU and memory system

   cpu/apogeo
   cpu/cpu_complex
   cpu/instruction_cache
   cpu/data_cache

.. toctree::
   :maxdepth: 2
   :caption: Audio processing

   audio/apu
   audio/audio_capture_unit
   audio/audio_synthesis_unit

.. toctree::
   :maxdepth: 2
   :caption: Integrated peripherals

   peripherals/gpio
   peripherals/uart
   peripherals/spi
   peripherals/timer
   peripherals/ethernet
   peripherals/sd
   peripherals/prng
   peripherals/trace_unit

.. toctree::
   :maxdepth: 2
   :caption: RTL under development / Future development

   architecture/video_system

.. toctree::
   :maxdepth: 1
   :caption: Legal

   legal/license

Quick links
===========

* :doc:`architecture/memory_map` — current MMIO windows and interrupt vectors.
* :doc:`cpu/cpu_complex` — the boundary between ApogeoRV, caches, memory, and
  peripherals.
* :doc:`cpu/apogeo` — links from the SoC-specific CPU page to the upstream
  ApogeoRV documentation.
* :doc:`peripherals/trace_unit` — the trace path and its direct connection to
  the UART TX FIFO.

Build and simulation commands live in the repository-level ``README.md``.
The short ``docs/README.md`` is only for building and maintaining this Sphinx
site.

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
