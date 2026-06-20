.. ZenithSoC Documentation master file

===================================
ZenithSoC Documentation
===================================

Welcome to the ZenithSoC documentation. This comprehensive guide covers the complete system-on-chip architecture, including the CPU complex, memory subsystems, peripherals, and interconnect infrastructure.

.. toctree::
   :maxdepth: 2
   :caption: System Architecture

   cpu_complex/cpu_complex
   cpu_complex/instruction_cache
   cpu_complex/data_cache
   cpu_complex/axi_bus

.. toctree::
   :maxdepth: 2
   :caption: Audio Processing Unit (APU)

   apu/apu
   apu/audio_capture_unit
   apu/audio_synthesis_unit

.. toctree::
   :maxdepth: 2
   :caption: IO Devices

   io_devs/gpio
   io_devs/uart
   io_devs/spi
   io_devs/timer
   io_devs/ethernet
   io_devs/sd
   io_devs/prng
   io_devs/trace_unit

.. toctree::
   :maxdepth: 1
   :caption: Legal

   license

Overview
========

ZenithSoC is a RISC-V based system-on-chip featuring:

- **CPU**: ApogeoRV (RV32IMFZfinx + B extensions) with out-of-order execution
- **Memory Hierarchy**: Separate instruction and data caches with write-back policy
- **Interconnect**: AXI4-Lite bus with priority-based arbitration
- **Audio Processing**: Integrated capture and synthesis units with PDM/PCM conversion
- **Peripherals**: GPIO, UART, SPI, Timer, Ethernet, SD Card, PRNG, Trace Unit

Key Features
============

CPU Complex
-----------

- **Architecture**: RISC-V RV32IMFZfinx with bit manipulation extensions
- **Execution Model**: Out-of-order with reorder buffer (ROB)
- **Branch Prediction**: 2-level adaptive predictor with branch target buffer (BTB)
- **Instruction Cache**: Direct-mapped, configurable size
- **Data Cache**: Direct-mapped with separate load/store controllers
- **Memory Protection**: Privilege levels (Machine/User mode)
- **Interrupt Controller**: Hardware interrupt with priority support

Audio Processing Unit (APU)
----------------------------

- **Audio Capture**: PDM microphone interface with CIC filtering
- **Audio Synthesis**: 4-channel waveform generator with ADSR modulation
- **Waveforms**: Sine, square, triangle, custom (arbitrary)
- **Sample Rates**: 8 kHz to 256 kHz (capture), DC to f_clk/8 (synthesis)
- **Audio Mixing**: 4-channel mixer with automatic normalization
- **Output**: PWM/PDM for class-D amplifiers

IO Peripherals
--------------

- **GPIO**: Up to 32 configurable pins with interrupt support
- **UART**: Hardware flow control, programmable baud rate, FIFO buffers
- **SPI**: Master/slave modes, configurable clock polarity and phase
- **Timer**: 64-bit counter with compare value and interrupt
- **Ethernet**: MAC controller with DMA support
- **SD Card**: SDIO/SPI interface with DMA transfers
- **PRNG**: Hardware random number generator
- **Trace Unit**: Real-time instruction tracing for debugging

Interconnect
------------

- **Protocol**: AXI4-Lite with 5-channel architecture
- **Arbitration**: Priority-based (data cache > instruction cache)
- **Address Decoding**: Memory-mapped regions for ROM, DDR, MMIO
- **Error Handling**: SLVERR and DECERR response codes

Documentation Structure
=======================

This documentation is organized into the following sections:

**System Architecture**
   Core system components including the CPU complex, cache subsystems, and interconnect infrastructure.

**Audio Processing Unit (APU)**
   Complete audio capture and synthesis subsystem with detailed register descriptions and programming examples.

**io_devs**
   Peripheral devices for general-purpose input/output, communication interfaces, and system utilities.

**Legal**
   License information and copyright notices.

Getting Started
===============

For hardware developers integrating ZenithSoC:

1. Start with :doc:`cpu_complex/cpu_complex` for the processor architecture
2. Review :doc:`io_devs/axi_bus` for interconnect integration
3. Consult individual peripheral documentation for interface details

For software developers writing firmware:

1. Review the CPU complex documentation for ISA details
2. Consult peripheral register maps for driver development
3. Use programming examples provided in each peripheral section

Version Information
===================

:Version: 1.0
:Release Date: May 4, 2026
:Author: Gabriele Tripi
:License: MIT License

Indices and Tables
==================

* :ref:`genindex`
* :ref:`search`
