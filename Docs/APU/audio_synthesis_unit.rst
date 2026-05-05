===================================
Audio Synthesis Unit (ASU)
===================================

Overview
========

The Audio Synthesis Unit is a versatile waveform generation and audio synthesis subsystem capable of producing four simultaneous audio channels with independent frequency, amplitude, and envelope control. Each channel supports sine, square, triangle, or custom waveform generation with ADSR (Attack-Decay-Sustain-Release) envelope modulation. The synthesized audio is mixed, gain-adjusted, and converted to PDM (Pulse Density Modulation) for external audio output.

Features
========

- **4 Independent Audio Channels**: Each channel can generate a different waveform simultaneously
- **Multiple Waveform Types**:

  - **Sine Wave**: Quarter-period lookup table with phase interpolation
  - **Square Wave**: Programmable duty cycle (0-100%)
  - **Triangle Wave**: Linear ramp up/down generation
  - **Custom Waveform**: 1024-sample double-buffered arbitrary waveform playback

- **ADSR Envelope Modulation**: Independent ADSR modulator per channel with configurable attack, decay, sustain, and release parameters
- **Audio Mixer**: Four-channel mixer with independent gain control per channel
- **PCM to PDM Conversion**: First-order delta-sigma modulator for class-D amplifier output
- **Fixed-Point Arithmetic**: Q1.15 format for gain and Q0.32 format for phase accumulation

Address Space
=============

The Audio Synthesis Unit occupies addresses **ACU_END_ADDRESS** to **ASU_END_ADDRESS** (ACU_END_ADDRESS + 2081).

.. list-table:: Address Map
   :widths: 20 20 60
   :header-rows: 1

   * - Address Range
     - Region
     - Description
   * - 0x0000 - 0x07FF
     - Custom Waveform Table
     - 2048 entries for custom waveform data (double-buffered)
   * - 0x0800 - 0x0807
     - Channel 0 Registers
     - Control, frequency, ADSR parameters for channel 0
   * - 0x0808 - 0x080F
     - Channel 1 Registers
     - Control, frequency, ADSR parameters for channel 1
   * - 0x0810 - 0x0817
     - Channel 2 Registers
     - Control, frequency, ADSR parameters for channel 2
   * - 0x0818 - 0x081F
     - Channel 3 Registers
     - Control, frequency, ADSR parameters for channel 3
   * - 0x0820
     - Square Wave Duty Cycle
     - Global duty cycle register for square wave generator

Architecture
============

The Audio Synthesis Unit consists of three main processing stages:

1. **Waveform Generation Pipeline**: Four independent waveform generators with ADSR envelope modulation
2. **Audio Mixer**: Combines four channels with independent gain control and normalization
3. **PCM to PDM Converter**: Converts mixed PCM output to PDM for external audio interface

.. code-block:: text

    ┌─────────────────────────────────────────────────────────────────┐
    │                    Audio Synthesis Pipeline                     │
    ├─────────────────────────────────────────────────────────────────┤
    │                                                                  │
    │  ┌──────────────┐    ┌────────────────┐    ┌──────────────┐   │
    │  │ Sine Wave    │───▶│ ADSR Modulator │───▶│              │   │
    │  │ Generator    │    │  (Channel 0)   │    │              │   │
    │  └──────────────┘    └────────────────┘    │              │   │
    │                                              │              │   │
    │  ┌──────────────┐    ┌────────────────┐    │              │   │
    │  │ Square Wave  │───▶│ ADSR Modulator │───▶│              │   │
    │  │ Generator    │    │  (Channel 1)   │    │  Audio       │   │
    │  └──────────────┘    └────────────────┘    │  Mixer       │   │
    │                                              │              │   │
    │  ┌──────────────┐    ┌────────────────┐    │  (4-channel) │   │
    │  │ Triangle Wave│───▶│ ADSR Modulator │───▶│              │   │
    │  │ Generator    │    │  (Channel 2)   │    │              │   │
    │  └──────────────┘    └────────────────┘    │              │   │
    │                                              │              │   │
    │  ┌──────────────┐    ┌────────────────┐    │              │   │
    │  │ Custom Wave  │───▶│ ADSR Modulator │───▶│              │   │
    │  │ Generator    │    │  (Channel 3)   │    │              │   │
    │  └──────────────┘    └────────────────┘    └──────┬───────┘   │
    │                                                     │           │
    └─────────────────────────────────────────────────────┼───────────┘
                                                          │ 16-bit PCM
                                                          ▼
                                                  ┌───────────────┐
                                                  │  PCM to PDM   │
                                                  │  Converter    │
                                                  └───────┬───────┘
                                                          │ PDM
                                                          ▼
                                                     PWM Output

Waveform Generators
===================

Sine Wave Generator
-------------------

The sine wave generator uses a 512-entry quarter-period lookup table stored in ``sine_quarter.hex``. The generator employs a 4-state FSM to traverse the full sine period:

.. code-block:: text

    FIRST_π/4 → SECOND_π/4 → THIRD_π/4 → FOURTH_π/4 → FIRST_π/4
       (inc)      (dec)        (inc)        (dec)

**Phase Accumulator**:

.. code-block:: systemverilog

    phase_register = phase_register + phase_increment

Where ``phase_increment`` controls the frequency:

.. math::

    phase\_increment = \frac{f_{desired} \times 2^{32}}{f_{clk}}

**FSM States**:

- **FIRST_π/4**: Phase increments, offset = 0, output = ``sine_table[index]``
- **SECOND_π/4**: Phase decrements, offset = 0, output = ``sine_table[index]``
- **THIRD_π/4**: Phase increments, offset = 0xFFFF, output = ``offset - sine_table[index]``
- **FOURTH_π/4**: Phase decrements, offset = 0xFFFF, output = ``offset - sine_table[index]``

The FSM transitions when the phase accumulator overflows (bit 32 = 1).

Square Wave Generator
---------------------

The square wave generator produces a pulse with programmable duty cycle.

**Operation**:

.. code-block:: systemverilog

    counter = counter + phase_increment
    square_wave = (counter >= duty_cycle) ? 0x0000 : 0xFFFF

**Duty Cycle Range**: 0x00000000 (0%) to 0xFFFFFFFF (100%)

**Frequency Calculation**: Same as sine wave generator

Triangle Wave Generator
-----------------------

The triangle wave generator produces a linear ramp that alternates between ascending and descending.

**Operation**:

.. code-block:: systemverilog

    if (up_down) begin
        counter = counter + phase_increment
    end else begin
        counter = counter - phase_increment
    end

**Output**:

.. code-block:: systemverilog

    triangle_wave = counter[31:16]

The direction inverts when the counter overflows, producing a symmetric triangle waveform.

Custom Wave Generator
---------------------

The custom wave generator supports arbitrary waveform playback from a 1024-sample double-buffered memory.

**Double Buffering**:

- **Buffer 0**: ``audio_table_1[1023:0]`` (addresses 0x0000 - 0x03FF)
- **Buffer 1**: ``audio_table_2[1023:0]`` (addresses 0x0400 - 0x07FF)

**Operation**:

1. Software writes samples to inactive buffer via write interface
2. Active buffer plays continuously using phase accumulator indexing
3. When phase accumulator wraps (overflow detected), buffers swap
4. ``active_table`` status bit indicates which buffer is currently playing

**Phase Accumulator**:

.. code-block:: systemverilog

    phase_register = phase_register + phase_increment
    table_index = phase_register[31:22]  // Top 10 bits index 1024 samples
    wave_output = active_table ? audio_table_2[table_index] : audio_table_1[table_index]

**Buffer Swap Detection**:

.. code-block:: systemverilog

    if (table_index < table_index_prev) begin
        // Overflow detected
        active_table = !active_table
    end

ADSR Envelope Modulator
========================

Each channel has an independent ADSR modulator that shapes the amplitude envelope of the generated waveform.

FSM States
----------

The ADSR modulator implements a 5-state FSM:

.. code-block:: text

    IDLE → ATTACK → DECAY → SUSTAIN → RELEASE → IDLE

.. list-table:: ADSR FSM States
   :widths: 20 80
   :header-rows: 1

   * - State
     - Description
   * - **IDLE**
     - Modulator is inactive, envelope = 0. Transitions to ATTACK on ``adsr_start``
   * - **ATTACK**
     - Envelope increases by ``attack_step`` each cycle until reaching ``attack_level``
   * - **DECAY**
     - Envelope decreases by ``decay_step`` each cycle until reaching ``sustain_level``
   * - **SUSTAIN**
     - Envelope held constant at ``sustain_level`` for ``sustain_duration`` cycles
   * - **RELEASE**
     - Envelope decreases by ``release_step`` each cycle until reaching zero

**Envelope Calculation**:

.. code-block:: systemverilog

    // ATTACK phase
    modulator = modulator + attack_step
    if (modulator >= attack_level) begin
        state = DECAY
        modulator = attack_level
    end

    // DECAY phase
    modulator = modulator - decay_step
    if (modulator <= sustain_level) begin
        state = SUSTAIN
        modulator = sustain_level
    end

    // SUSTAIN phase
    sustain_time = sustain_time + 1
    if (sustain_time == sustain_duration) begin
        state = RELEASE
    end

    // RELEASE phase
    modulator = modulator - release_step
    if (modulator <= release_step) begin
        state = IDLE
        modulator = 0
    end

**Gain Selection**:

Each channel can use either:

- **Static Gain**: Fixed gain value from ``wave_gain`` register (when ``adsr_enable = 0``)
- **Dynamic Gain**: ADSR envelope output (when ``adsr_enable = 1``)

.. code-block:: systemverilog

    effective_gain = adsr_enable ? adsr_modulator : wave_gain

Audio Mixer
===========

The audio mixer combines four channels with independent gain control and automatic normalization.

Mixing Pipeline
---------------

**Stage 1: Gain Application**

Each channel's waveform is multiplied by its gain (Q1.15 format):

.. code-block:: systemverilog

    // 16-bit waveform × 16-bit gain = 32-bit result
    sine_wave_scaled = sine_wave × sine_wave_gain       // Q0.15 × Q1.15 = Q1.30
    square_wave_scaled = square_wave × square_wave_gain
    triangle_wave_scaled = triangle_wave × triangle_wave_gain
    custom_wave_scaled = custom_wave × custom_wave_gain

Extract Q1.15 result: ``scaled_wave[30:15]``

**Stage 2: Pairwise Addition**

.. code-block:: systemverilog

    intermediate_1 = sine_wave_scaled[30:15] + square_wave_scaled[30:15]     // 17-bit
    intermediate_2 = triangle_wave_scaled[30:15] + custom_wave_scaled[30:15] // 17-bit

**Stage 3: Final Summation**

.. code-block:: systemverilog

    mixed_wave = intermediate_1 + intermediate_2  // 18-bit

**Stage 4: Normalization**

Automatic division based on number of active channels to prevent clipping:

.. list-table:: Normalization Logic
   :widths: 30 20 50
   :header-rows: 1

   * - Active Channels
     - Division
     - Implementation
   * - 0 channels
     - N/A
     - Output = 0
   * - 1 channel
     - ÷1
     - Output = mixed_wave
   * - 2 channels
     - ÷2
     - Output = mixed_wave >> 1
   * - 3 channels
     - ÷3
     - Output = (mixed_wave × 0xAAAB) >> 17
   * - 4 channels
     - ÷4
     - Output = mixed_wave >> 2

The division by 3 uses a multiplication by the reciprocal:

.. math::

    \frac{1}{3} \approx \frac{0xAAAB}{2^{17}} = \frac{43691}{131072} \approx 0.33333

PCM to PDM Converter
====================

The PCM to PDM converter implements a first-order delta-sigma modulator using error feedback.

**Algorithm**:

.. code-block:: systemverilog

    // 1. Convert unsigned PCM to signed
    sample_scaled = signed(pcm_input) - 32768

    // 2. Add bias (center around half-scale)
    sample_biased = sample_scaled + 32768  // BIAS = 2^15

    // 3. Accumulate with feedback
    accumulator = accumulator[15:0] + sample_biased

    // 4. PDM output is MSB
    pdm_output = accumulator[16]

**Oversampling**: The PDM output updates at the system clock rate, providing high oversampling ratio for noise shaping.

**Output Interface**: The ``pwm_o`` signal is driven as tri-state:

.. code-block:: systemverilog

    pwm_o = pdm_signal ? 1'bZ : 1'b0

This allows direct connection to class-D amplifier input with pull-up resistor.

Register Interface
==================

Each of the 4 audio channels has an identical 8-register interface.

Channel Register Map
---------------------

.. list-table:: Per-Channel Registers (8 registers × 4 channels)
   :widths: 25 15 60
   :header-rows: 1

   * - Register
     - Offset
     - Description
   * - WAVE_CONTROL
     - 0x00
     - Wave and ADSR control bits, gain
   * - WAVE_INCREMENT
     - 0x01
     - Phase increment (frequency control)
   * - WAVE_ATTACK_STEP
     - 0x02
     - ADSR attack increment per cycle
   * - WAVE_DECAY_STEP
     - 0x03
     - ADSR decay decrement per cycle
   * - WAVE_RELEASE_STEP
     - 0x04
     - ADSR release decrement per cycle
   * - WAVE_SUSTAIN_TIME
     - 0x05
     - ADSR sustain duration in clock cycles
   * - WAVE_ATTACK_LEVEL
     - 0x06
     - ADSR maximum attack level
   * - WAVE_SUSTAIN_LEVEL
     - 0x07
     - ADSR sustain level

WAVE_CONTROL Register (Offset 0x00)
------------------------------------

.. list-table:: WAVE_CONTROL Bit Fields
   :widths: 15 15 70
   :header-rows: 1

   * - Bits
     - Field
     - Description
   * - [31:16]
     - wave_gain
     - Q1.15 fixed-point gain (0.0 to 1.99998). Used when ``adsr_enable = 0``
   * - [21]
     - adsr_idle
     - Read-only. ``1`` = ADSR FSM is in IDLE state, ``0`` = ADSR active
   * - [4]
     - adsr_enable
     - ``1`` = Use ADSR envelope for gain, ``0`` = Use static ``wave_gain``
   * - [3]
     - adsr_start
     - Write ``1`` to start ADSR cycle and reset to ATTACK state. Self-clearing
   * - [1]
     - wave_enable
     - ``1`` = Enable waveform output, ``0`` = Waveform muted (output = 0)
   * - [0]
     - wave_start
     - Write ``1`` to reset phase accumulator. Self-clearing

WAVE_INCREMENT Register (Offset 0x01)
--------------------------------------

**32-bit phase increment value**. Determines waveform frequency:

.. math::

    f_{output} = \frac{phase\_increment \times f_{clk}}{2^{32}}

**Example**: For 440 Hz output at 100 MHz clock:

.. math::

    phase\_increment = \frac{440 \times 2^{32}}{100000000} = 18,908,481 = 0x01207BA1

WAVE_ATTACK_STEP Register (Offset 0x02)
----------------------------------------

**32-bit increment value** for ATTACK phase. Envelope increases by this value each cycle.

**Time Calculation**:

.. math::

    t_{attack} = \frac{attack\_level - 0}{attack\_step} \times \frac{1}{f_{clk}}

**Example**: 10ms attack to full scale (0xFFFFFFFF) at 100 MHz:

.. math::

    attack\_step = \frac{0xFFFFFFFF}{0.01 \times 100000000} = 4294

WAVE_DECAY_STEP Register (Offset 0x03)
---------------------------------------

**32-bit decrement value** for DECAY phase. Envelope decreases by this value each cycle.

**Time Calculation**:

.. math::

    t_{decay} = \frac{attack\_level - sustain\_level}{decay\_step} \times \frac{1}{f_{clk}}

WAVE_RELEASE_STEP Register (Offset 0x04)
-----------------------------------------

**32-bit decrement value** for RELEASE phase. Envelope decreases by this value each cycle.

**Time Calculation**:

.. math::

    t_{release} = \frac{sustain\_level - 0}{release\_step} \times \frac{1}{f_{clk}}

WAVE_SUSTAIN_TIME Register (Offset 0x05)
-----------------------------------------

**32-bit duration** in clock cycles for SUSTAIN phase.

**Time Calculation**:

.. math::

    t_{sustain} = \frac{sustain\_duration}{f_{clk}}

**Example**: 500ms sustain at 100 MHz:

.. math::

    sustain\_duration = 0.5 \times 100000000 = 50,000,000

WAVE_ATTACK_LEVEL Register (Offset 0x06)
-----------------------------------------

**32-bit maximum envelope level** during ATTACK phase. Typical value: ``0xFFFFFFFF`` (full scale).

WAVE_SUSTAIN_LEVEL Register (Offset 0x07)
------------------------------------------

**32-bit envelope level** maintained during SUSTAIN phase. Range: ``0x00000000`` to ``attack_level``.

Square Wave Duty Cycle Register (Offset 0x0820)
------------------------------------------------

**Global 32-bit duty cycle** for the square wave generator (channel 1).

.. list-table:: Duty Cycle Values
   :widths: 30 30 40
   :header-rows: 1

   * - Value
     - Duty Cycle
     - Description
   * - 0x00000000
     - 0%
     - Always low
   * - 0x80000000
     - 50%
     - Symmetric square wave
   * - 0xFFFFFFFF
     - 100%
     - Always high

Custom Waveform Table
---------------------

**Address Range**: 0x0000 - 0x07FF (2048 entries, 16-bit each)

**Double Buffer Organization**:

- **Buffer 0**: Addresses 0x0000 - 0x03FF (1024 samples)
- **Buffer 1**: Addresses 0x0400 - 0x07FF (1024 samples)

**Write Procedure**:

1. Write 1024 samples to inactive buffer
2. Enable custom wave channel
3. Hardware automatically swaps buffers when active buffer completes playback
4. Monitor ``active_table`` status bit (WAVE_CONTROL[21] of channel 3) to determine which buffer is playing