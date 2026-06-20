===================================
Audio Processing Unit (APU)
===================================

Overview
========

The Audio Processing Unit (APU) is a comprehensive audio subsystem that integrates bidirectional audio capabilities: audio capture from external microphones via PDM interface and audio synthesis/playback via PWM output. The APU combines two independent but complementary subsystems—the Audio Capture Unit (ACU) and Audio Synthesis Unit (ASU)—into a unified memory-mapped peripheral with a common register interface.

Features
========

- **Bidirectional Audio Processing**: Simultaneous audio capture and synthesis
- **Audio Capture Unit (ACU)**:

  - PDM microphone interface with stereo support
  - CIC filtering and decimation
  - Programmable gain and sample rate
  - Sample buffer with interrupt generation

- **Audio Synthesis Unit (ASU)**:

  - 4-channel waveform synthesis (sine, square, triangle, custom)
  - ADSR envelope modulation per channel
  - Audio mixing with automatic normalization
  - PCM to PDM conversion for PWM output

- **Unified Memory-Mapped Interface**: Single 12-bit address space for both subsystems
- **Configurable Buffer Size**: Parameterizable sample buffer depth (default 512 samples)
- **Interrupt Support**: Hardware interrupt generation for capture events

Architecture
============

The APU integrates two major subsystems with address decoding and output multiplexing logic.

.. code-block:: text

    ┌─────────────────────────────────────────────────────────────────┐
    │                    Audio Processing Unit (APU)                  │
    ├─────────────────────────────────────────────────────────────────┤
    │                                                                  │
    │  ┌───────────────────────────────────────────────────────────┐ │
    │  │           Audio Capture Unit (ACU)                        │ │
    │  │  Address: 0x000 - 0x007                                   │ │
    │  ├───────────────────────────────────────────────────────────┤ │
    │  │                                                            │ │
    │  │   PDM Mic ──▶ CIC Filter ──▶ Gain ──▶ Sample Buffer      │ │
    │  │   Interface      Decimation      Stage     (FIFO)         │ │
    │  │                                                │            │ │
    │  │                                                └───▶ INT   │ │
    │  └────────────────────────────────────────────────────────────┘ │
    │                                                                  │
    │  ┌───────────────────────────────────────────────────────────┐ │
    │  │        Audio Synthesis Unit (ASU)                         │ │
    │  │  Address: 0x008 - 0x828                                   │ │
    │  ├───────────────────────────────────────────────────────────┤ │
    │  │                                                            │ │
    │  │   ┌──────────┐    ┌──────┐    ┌─────────┐    ┌────────┐ │ │
    │  │   │ Wave Gen ├───▶│ ADSR ├───▶│  Mixer  ├───▶│ PCM2PDM├─┼─┼──▶ PWM
    │  │   │ (×4)     │    │ (×4) │    │ 4-ch    │    │        │ │ │
    │  │   └──────────┘    └──────┘    └─────────┘    └────────┘ │ │
    │  │                                                            │ │
    │  └────────────────────────────────────────────────────────────┘ │
    │                                                                  │
    │  ┌───────────────────────────────────────────────────────────┐ │
    │  │               Address Decoder & Mux Logic                 │ │
    │  │  ┌──────────────┐        ┌──────────────┐                │ │
    │  │  │ ACU Address  │        │ ASU Address  │                │ │
    │  │  │ Decoder      │        │ Decoder      │                │ │
    │  │  │ (0x000-0x007)│        │ (0x008-0x828)│                │ │
    │  │  └──────────────┘        └──────────────┘                │ │
    │  └────────────────────────────────────────────────────────────┘ │
    │                          │                                       │
    │                          ▼                                       │
    │              Write/Read Data Multiplexer                        │
    │                                                                  │
    └──────────────────────────────────────────────────────────────────┘
           │                                          │
           ▼                                          ▼
    Write Interface                           Read Interface
    (12-bit addr, 32-bit data)               (12-bit addr, 32-bit data)

Address Space Organization
==========================

The APU implements a 12-bit address space (4096 addresses) partitioned between the two subsystems:

.. list-table:: APU Address Map
   :widths: 25 25 50
   :header-rows: 1

   * - Address Range
     - Subsystem
     - Description
   * - 0x000 - 0x007
     - Audio Capture Unit
     - 8 registers: status, control, gain, decimation, normalizer, sample buffer, threshold, event
   * - 0x008 - 0x828
     - Audio Synthesis Unit
     - 2089 addresses: custom waveform table (2048) + 4 channel registers (32) + duty cycle (1)
   * - 0x829 - 0xFFF
     - Reserved
     - Future expansion

Address Decoding Logic
----------------------

The APU implements address-based routing with error detection:

.. code-block:: systemverilog

    // ACU address range: 0x000 - 0x007
    acu_write = (write_address < 8) & write_i
    acu_read = (read_address < 8) & read_i

    // ASU address range: 0x008 - 0x828
    asu_write = (write_address >= 8) & (write_address < 2089) & write_i
    asu_read = (read_address >= 8) & (read_address < 2089) & read_i

**Address Translation**:

- **ACU**: Uses lower 3 bits directly ``[2:0]``
- **ASU**: Subtracts ACU_END_ADDRESS (8) from incoming address

**Error Handling**:

- Access to addresses ≥ 2089 generates ``read_error`` or ``write_error``
- Subsystem-specific errors propagate to top-level error outputs

Data Interface
==============

Write Interface
---------------

.. list-table:: Write Interface Signals
   :widths: 30 15 55
   :header-rows: 1

   * - Signal
     - Width
     - Description
   * - write_i
     - 1
     - Write enable
   * - write_address_i
     - 12
     - Write address (byte-aligned)
   * - write_data_i
     - 32
     - Write data (little-endian byte array [3:0][7:0])
   * - write_strobe_i
     - 4
     - Byte-enable strobe (1 = write byte)
   * - write_done_o
     - 1
     - Write completion acknowledgment
   * - write_error_o
     - 1
     - Write error flag (address out of range or subsystem error)

**Write Operation**:

.. code-block:: text

    Clock Cycle:    1         2         3
                    │         │         │
    write_i      ───┘▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔───
    write_addr   ───<  ADDR   >─────────────────────
    write_data   ───<  DATA   >─────────────────────
    write_strobe ───< STROBE  >─────────────────────
    write_done   ─────────────┘▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔───
    write_error  ─────────────┘▔ (if err)│─────────────

Read Interface
--------------

.. list-table:: Read Interface Signals
   :widths: 30 15 55
   :header-rows: 1

   * - Signal
     - Width
     - Description
   * - read_i
     - 1
     - Read enable
   * - read_address_i
     - 12
     - Read address (byte-aligned)
   * - read_data_o
     - 32
     - Read data
   * - read_done_o
     - 1
     - Read completion acknowledgment
   * - read_error_o
     - 1
     - Read error flag (address out of range or subsystem error)

**Read Operation**:

.. code-block:: text

    Clock Cycle:    1         2         3
                    │         │         │
    read_i       ───┘▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔───
    read_addr    ───<  ADDR   >─────────────────────
    read_done    ─────────────┘▔▔▔▔▔▔▔▔▔│▔▔▔▔▔▔▔▔▔───
    read_data    ─────────────< DATA OUT>───────────
    read_error   ─────────────┘▔ (if err)│─────────────

External Interfaces
===================

PDM Microphone Interface (ACU)
-------------------------------

.. list-table:: PDM Interface Signals
   :widths: 30 15 55
   :header-rows: 1

   * - Signal
     - Direction
     - Description
   * - pdm_data_i
     - Input
     - PDM data input from microphone
   * - pdm_clk_o
     - Output
     - PDM clock output to microphone
   * - pdm_lrsel_o
     - Output
     - Left/Right channel select (stereo mode)

**Clock Generation**:

.. math::

    f_{PDM\_CLK} = \frac{f_{system}}{2 \times (clock\_divisor + 1)}

**Sample Rate**:

.. math::

    f_{sample} = \frac{f_{PDM\_CLK}}{decimation\_factor}

PWM Audio Output (ASU)
-----------------------

.. list-table:: PWM Interface Signals
   :widths: 30 15 55
   :header-rows: 1

   * - Signal
     - Direction
     - Description
   * - pwm_o
     - Output
     - PWM/PDM audio output (tri-state: 1'bZ for high, 1'b0 for low)
   * - audio_enable_o
     - Output
     - Audio output enable (high when any ASU channel is active)

**Output Circuit**:

.. code-block:: text

           3.3V
            │
            ├─── 10kΩ Pull-up
            │
    pwm_o ──┴───▶ To Class-D Amplifier Input

    (Tri-state: Z = High, 0 = Low)

Interrupt Interface
-------------------

.. list-table:: Interrupt Signal
   :widths: 30 15 55
   :header-rows: 1

   * - Signal
     - Direction
     - Description
   * - interrupt_o
     - Output
     - Interrupt request (generated by ACU only)

**Interrupt Sources** (ACU):

- Sample buffer full
- Left channel threshold high/low
- Right channel threshold high/low
- Capture error

**Note**: The ASU does not generate interrupts. Software must poll ADSR status if needed.

Subsystem Details
=================

Audio Capture Unit (ACU)
-------------------------

**Address Range**: 0x000 - 0x007 (8 registers)

**Key Features**:

- PDM to PCM conversion with 5th-order CIC filter
- Programmable decimation factor (8 to 256)
- Programmable gain (Q1.15 format)
- Sample FIFO buffer (configurable depth, default 512)
- Dual-channel (stereo) or single-channel (mono) operation
- Threshold-based event generation

**Detailed Documentation**: See ``audio_capture_unit.rst``

Audio Synthesis Unit (ASU)
---------------------------

**Address Range**: 0x008 - 0x828 (2089 addresses)

**Key Features**:

- 4 independent waveform generators
- Waveform types: sine, square, triangle, custom
- ADSR envelope modulation per channel
- 4-channel audio mixer with normalization
- PCM to PDM conversion
- Custom waveform double buffering (1024 samples per buffer)

**Detailed Documentation**: See ``audio_synthesis_unit.rst``