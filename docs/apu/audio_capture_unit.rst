Audio Capture Unit
==================

Overview
--------

The Audio Capture Unit (ACU) is a hardware accelerator for capturing and processing audio from PDM (Pulse Density Modulation) microphones. It implements a complete signal processing chain from PDM sampling to PCM (Pulse Code Modulation) conversion, featuring CIC filtering, normalization, and programmable gain control.

The ACU operates autonomously after configuration, performing real-time audio capture and processing with minimal CPU intervention. It supports both single-channel (mono) and dual-channel (stereo) operation, providing 16-bit PCM samples through a FIFO buffer accessible via memory-mapped registers.

Features
~~~~~~~~

* PDM microphone interface with programmable clock generation
* Single-channel (mono) or dual-channel (stereo) capture
* CIC (Cascaded Integrator-Comb) low-pass filter for anti-aliasing
* Configurable decimation factor (1-127)
* Fixed-point normalization for dynamic range optimization
* Programmable gain stage (Q1.15 format)
* 16-bit PCM output at configurable sample rates
* FIFO buffer for sample storage (default 512 samples)
* Interrupt generation on buffer full and threshold crossing
* Event-driven error reporting

Architecture
------------

The Audio Capture Unit consists of four main pipeline stages:

* **PDM Converter**: Samples PDM data and generates clock/channel select signals
* **CIC Filter**: 5th-order low-pass filter with programmable decimation
* **Normalizer**: Fixed-point division to scale samples to [0:1] range
* **Gain Stage**: Programmable amplification and final PCM conversion
* **Register Interface**: Configuration, control, and sample buffer management

Signal Interface
~~~~~~~~~~~~~~~~

**Global Signals:**

* ``clk_i``: System clock
* ``rst_n_i``: Active-low reset
* ``interrupt_o``: Interrupt request to CPU

**PDM Microphone Interface:**

* ``pdm_data_i``: PDM data input from microphone
* ``pdm_clk_o``: PDM clock output to microphone
* ``pdm_lrsel_o``: Left/Right channel select (stereo mode)

**Register Interface:**

* ``write_i``: Write enable
* ``write_address_i[2:0]``: Write register address
* ``write_data_i[31:0]``: Write data
* ``write_strobe_i[3:0]``: Byte-level write enable
* ``write_done_o``: Write completed
* ``write_error_o``: Write error (read-only register)

* ``read_i``: Read enable
* ``read_address_i[2:0]``: Read register address
* ``read_data_o[31:0]``: Read data
* ``read_done_o``: Read completed
* ``read_error_o``: Read error

PDM Converter
-------------

PDM Interface
~~~~~~~~~~~~~

The PDM converter generates the clock signal for the microphone and samples incoming PDM data:

**Clock Generation:**

.. code-block:: text

   PDM Clock Frequency = System Clock / (2 × (clock_divisor + 1))

   Example: 50 MHz system clock, divisor = 24
   PDM Clock = 50 MHz / (2 × 25) = 1 MHz

**Typical PDM Clock Frequencies:**

+------------+----------------+---------------------------+
| Divisor    | PDM Clock      | Application               |
+============+================+===========================+
| 24         | 1.0 MHz        | Standard microphones      |
+------------+----------------+---------------------------+
| 12         | 2.0 MHz        | Higher bandwidth          |
+------------+----------------+---------------------------+
| 49         | 500 kHz        | Low-power applications    |
+------------+----------------+---------------------------+

Channel Configuration
~~~~~~~~~~~~~~~~~~~~~

**Single-Channel (Mono) Mode:**

* ``dual_channel`` = 0
* ``channel`` selects LEFT (0) or RIGHT (1)
* Samples on rising edge (LEFT) or falling edge (RIGHT) of PDM clock
* ``pdm_lrsel_o`` held constant at selected channel

**Dual-Channel (Stereo) Mode:**

* ``dual_channel`` = 1
* Samples on both rising and falling edges of PDM clock
* ``pdm_lrsel_o`` toggles every PDM clock cycle
* Doubles effective sample rate

**Timing Diagram (Mono - Left Channel):**

.. code-block:: text

   pdm_clk:     ──┐  ┌──┐  ┌──┐  ┌──┐  ┌──
                  │  │  │  │  │  │  │  │
   pdm_lrsel:   ──────────────────────────  (LEFT = 0)
   pdm_data:    ──┘  └──┐  ┌──┘  └──┐  ┌──

   Sample:         ↑     ↑     ↑     ↑
                  D0    D1    D2    D3

**Timing Diagram (Stereo):**

.. code-block:: text

   pdm_clk:     ──┐  ┌──┐  ┌──┐  ┌──┐  ┌──
                  │  │  │  │  │  │  │  │
   pdm_lrsel:   ──┘  └──┐  └──┐  └──┐  └──  (toggles)
                  L     R     L     R
   pdm_data:    ──┘  └──┐  ┌──┘  └──┐  ┌──

   Sample:         ↑  ↓  ↑  ↓  ↑  ↓  ↑  ↓
                  L0 R0 L1 R1 L2 R2 L3 R3

Input Synchronization
~~~~~~~~~~~~~~~~~~~~~

PDM data input is synchronized through a 3-stage synchronizer to prevent metastability:

.. code-block:: systemverilog

   synchronizer #(3, 0) input_sync (
       .signal_i ( pdm_data_i ),
       .sync_o   ( pdm_data_o )
   );

This introduces 3 clock cycles of latency but ensures reliable sampling.

CIC Filter
----------

Filter Architecture
~~~~~~~~~~~~~~~~~~~

The CIC (Cascaded Integrator-Comb) filter implements a 5th-order low-pass filter with configurable decimation:

**Structure:**

.. code-block:: text

   PDM → [Integrator₁] → [Integrator₂] → ... → [Integrator₅]
              ↓
         Decimator (M)
              ↓
   [Comb₁] → [Comb₂] → ... → [Comb₅] → PCM

**Filter Order:** 5 (parameter: CIC_FILTER_ORDER = 5)

**Comb Delay:** 1 (parameter: CIC_COMB_DELAY = 1)

Integrator Stage
~~~~~~~~~~~~~~~~

The integrator stage accumulates PDM bits at full sample rate:

.. code-block:: systemverilog

   integrator[i] = integrator_ff[i-1] + integrator_ff[i]

**Characteristics:**

* Operates at PDM clock rate (e.g., 1 MHz)
* Each stage accumulates previous stage output
* No decimation (all samples processed)
* 32-bit accumulator width to prevent overflow

Decimation
~~~~~~~~~~

The decimator reduces sample rate by factor M:

.. code-block:: text

   PCM Sample Rate = PDM Clock / M

   Example: PDM = 1 MHz, M = 64
   Sample Rate = 1 MHz / 64 = 15.625 kHz

**Decimator Counter:**

.. code-block:: systemverilog

   if (decimator == decimation_factor) begin
       valid_sample = 1;  // Pass sample to comb stage
       decimator = 0;     // Reset counter
   end else begin
       decimator = decimator + 1;  // Discard sample
   end

Comb Stage
~~~~~~~~~~

The comb stage performs differentiation at decimated rate:

.. code-block:: systemverilog

   comb[i] = comb_ff[i-1] - comb_delay[i-1][M-1]

**Characteristics:**

* Operates at decimated rate (e.g., 15.625 kHz)
* Subtracts delayed version to create high-pass response
* Delay length equals comb delay parameter (M = 1)
* Combined with integrators creates low-pass filter

Filter Response
~~~~~~~~~~~~~~~

**Passband:**

* DC to approximately fs/2M (Nyquist frequency)
* Slight droop near band edge

**Stopband:**

* Strong attenuation above fs/2M
* Nulls at multiples of fs/M

**Gain:**

.. code-block:: text

   CIC Gain = (R × M)^N

   Where:
   R = decimation ratio
   M = comb delay
   N = filter order

   Example: R = 64, M = 1, N = 5
   Gain = (64 × 1)^5 = 1,073,741,824

This large gain requires normalization to prevent overflow.

Normalization Stage
-------------------

Fixed-Point Division
~~~~~~~~~~~~~~~~~~~~

The normalizer performs fixed-point division to scale CIC output to [0:1] range:

.. code-block:: systemverilog

   normalized_sample = (filtered_sample << 32) / normalizer

**Input Format:** Q32.0 (32-bit integer from CIC filter)

**Output Format:** Q0.32 (32-bit fraction, range [0:1])

**Normalizer Value:**

.. code-block:: text

   normalizer = decimation_factor^(filter_order - 1)

   Example: M = 64, N = 5
   normalizer = 64^4 = 16,777,216

This compensates for CIC filter gain, resulting in unity gain overall.

Division Algorithm
~~~~~~~~~~~~~~~~~~

Uses non-restoring division algorithm:

* **Latency:** 64 clock cycles (1 cycle per bit)
* **Throughput:** 1 division per 64 cycles (pipelined)
* **Precision:** Full 32-bit fractional result

**Division Controller:**

.. code-block:: systemverilog

   non_restoring_divider #(
       .DATA_WIDTH ( 64 )
   ) normalizer (
       .dividend_i   ( {filtered_sample, 32'b0} ),  // Q32.32
       .divisor_i    ( {32'b0, normalizer_i}    ),  // Q32.0
       .quotient_o   ( normalized_sample        ),  // Q0.32
       .data_valid_i ( valid_filter & idle      ),
       .data_valid_o ( normalized_valid         ),
       .idle_o       ( normalizer_idle          )
   );

Error Handling
~~~~~~~~~~~~~~

Division by zero detection:

* ``divide_by_zero_o`` asserted if normalizer is zero
* Sample marked as invalid via ``invalid_o`` flag
* Prevents propagation of erroneous samples

Gain Stage
----------

PCM Conversion
~~~~~~~~~~~~~~

Normalized sample [0:1] is mapped to 16-bit unsigned PCM:

.. code-block:: systemverilog

   // Convert Q0.32 to Q32.0 (shift left by 32)
   full_sample = normalized_sample << 32;

   // Take upper 16 bits for 16-bit PCM
   pcm_sample = full_sample[63:48];

**Output Range:** 0 to 65535 (16-bit unsigned)

Saturation:

* If ``normalized_sample[32]`` = 1 (overflow), saturate to 0xFFFF

Programmable Gain
~~~~~~~~~~~~~~~~~

Final gain stage applies user-defined amplification:

.. code-block:: text

   output = pcm_sample × gain

   Gain Format: Q1.15 (1 integer bit, 15 fractional bits)
   Range: 0.0 to 1.999969482421875

**Gain Multiplier:**

* Xilinx DSP48 slice (when ``_VIVADO_`` defined)
* Latency: 2 clock cycles (registered multiplier)
* Product: Q17.15 (32-bit intermediate)
* Output: bits [30:15] extracted for Q16.0 result

**Gain Examples:**

+-------------+---------+----------------------+
| Gain Value  | Decimal | Effect               |
+=============+=========+======================+
| 0x8000      | 1.0     | Unity gain (default) |
+-------------+---------+----------------------+
| 0x4000      | 0.5     | -6 dB attenuation    |
+-------------+---------+----------------------+
| 0xC000      | 1.5     | +3.5 dB boost        |
+-------------+---------+----------------------+
| 0xFFFF      | ~2.0    | +6 dB boost (max)    |
+-------------+---------+----------------------+

Pipeline Latency
~~~~~~~~~~~~~~~~

Total pipeline latency from PDM input to PCM output:

1. Input synchronization: 3 cycles
2. CIC integrator: 5 cycles (filter order)
3. Decimation: M cycles (decimation factor)
4. CIC comb: 5 cycles (filter order)
5. Normalization: 64 cycles (division)
6. Gain multiplication: 2 cycles

**Total:** ~79 + M cycles (e.g., ~143 cycles for M = 64)

Register Map
------------

+--------+----------------------------+------+------------------------------------------------+
| Offset | Register                   | R/W  | Description                                    |
+========+============================+======+================================================+
| 0x00   | STATUS                     | R    | Buffer status (empty/full flags)               |
+--------+----------------------------+------+------------------------------------------------+
| 0x04   | CONTROL                    | R/W  | Interface control and configuration            |
+--------+----------------------------+------+------------------------------------------------+
| 0x08   | GAIN                       | R/W  | Gain value (Q1.15 format)                      |
+--------+----------------------------+------+------------------------------------------------+
| 0x0C   | DECIMATION_FACTOR          | R/W  | CIC decimation factor (1-127)                  |
+--------+----------------------------+------+------------------------------------------------+
| 0x10   | NORMALIZER                 | R/W  | Normalizer divisor value                       |
+--------+----------------------------+------+------------------------------------------------+
| 0x14   | SAMPLE_BUFFER              | R    | FIFO read port (16-bit samples)                |
+--------+----------------------------+------+------------------------------------------------+
| 0x18   | THRESHOLD                  | R/W  | Threshold values for event generation          |
+--------+----------------------------+------+------------------------------------------------+
| 0x1C   | EVENT                      | R    | Event flags (buffer full, threshold, error)    |
+--------+----------------------------+------+------------------------------------------------+

Register Descriptions
~~~~~~~~~~~~~~~~~~~~~

**STATUS (Offset: 0x00)**

+--------+---------------+-----+------------------------------------------------+
| Bit    | Field         | R/W | Description                                    |
+========+===============+=====+================================================+
| 0      | BUFFER_EMPTY  | R   | Sample buffer empty (1 = empty)                |
+--------+---------------+-----+------------------------------------------------+
| 1      | BUFFER_FULL   | R   | Sample buffer full (1 = full)                  |
+--------+---------------+-----+------------------------------------------------+
| 31:2   | Reserved      | R   | Reserved (read as 0)                           |
+--------+---------------+-----+------------------------------------------------+

*Reset Value:* 0x00000001 (buffer empty)

**CONTROL (Offset: 0x04)**

+--------+---------------------+-----+------------------------------------------------+
| Bit    | Field               | R/W | Description                                    |
+========+=====================+=====+================================================+
| 0      | INTERFACE_ENABLE    | R/W | Enable PDM interface (1 = enabled)             |
+--------+---------------------+-----+------------------------------------------------+
| 1      | BUFFER_ENABLE       | R/W | Enable writing to sample buffer                |
+--------+---------------------+-----+------------------------------------------------+
| 2      | DUAL_CHANNEL        | R/W | Stereo mode (1 = dual channel)                 |
+--------+---------------------+-----+------------------------------------------------+
| 3      | CHANNEL             | R/W | Channel select in mono mode (0=L, 1=R)         |
+--------+---------------------+-----+------------------------------------------------+
| 9:4    | INTERRUPT_ENABLE    | R/W | Interrupt enable mask (6 bits)                 |
+--------+---------------------+-----+------------------------------------------------+
| 16:10  | CLOCK_DIVISOR       | R/W | PDM clock divisor (0-127)                      |
+--------+---------------------+-----+------------------------------------------------+
| 31:17  | Reserved            | R   | Reserved (read as 0)                           |
+--------+---------------------+-----+------------------------------------------------+

*Reset Value:* 0x00000000

**Interrupt Enable Bits:**

+-----+--------------------------------+
| Bit | Event                          |
+=====+================================+
| 4   | Buffer full                    |
+-----+--------------------------------+
| 5   | Left channel threshold high    |
+-----+--------------------------------+
| 6   | Left channel threshold low     |
+-----+--------------------------------+
| 7   | Right channel threshold high   |
+-----+--------------------------------+
| 8   | Right channel threshold low    |
+-----+--------------------------------+
| 9   | Processing error (div by zero) |
+-----+--------------------------------+

**GAIN (Offset: 0x08)**

+--------+-------+-----+------------------------------------------------+
| Bit    | Field | R/W | Description                                    |
+========+=======+=====+================================================+
| 15:0   | GAIN  | R/W | Gain value in Q1.15 format                     |
+--------+-------+-----+------------------------------------------------+
| 31:16  | -     | -   | Unused                                         |
+--------+-------+-----+------------------------------------------------+

*Reset Value:* 0x00008000 (gain = 1.0)

**DECIMATION_FACTOR (Offset: 0x0C)**

+--------+-------+-----+------------------------------------------------+
| Bit    | Field | R/W | Description                                    |
+========+=======+=====+================================================+
| 7:0    | FACTOR| R/W | CIC decimation factor (1-127)                  |
+--------+-------+-----+------------------------------------------------+
| 31:8   | -     | -   | Unused                                         |
+--------+-------+-----+------------------------------------------------+

*Reset Value:* 0x00000000

**NORMALIZER (Offset: 0x10)**

+--------+-----------+-----+------------------------------------------------+
| Bit    | Field     | R/W | Description                                    |
+========+===========+=====+================================================+
| 31:0   | NORMALIZER| R/W | Normalizer divisor value                       |
+--------+-----------+-----+------------------------------------------------+

*Reset Value:* 0x00000000

Calculation: ``normalizer = decimation_factor^(filter_order - 1)``

**SAMPLE_BUFFER (Offset: 0x14)**

+--------+-------+-----+------------------------------------------------+
| Bit    | Field | R/W | Description                                    |
+========+=======+=====+================================================+
| 15:0   | SAMPLE| R   | 16-bit PCM sample from FIFO                    |
+--------+-------+-----+------------------------------------------------+
| 31:16  | -     | -   | Unused (read as 0)                             |
+--------+-------+-----+------------------------------------------------+

Reading this register pops one sample from the FIFO. Check BUFFER_EMPTY before reading.


Sample Rates
~~~~~~~~~~~~

Achievable sample rates depend on PDM clock and decimation:

+------------------+------------+--------------+
| PDM Clock        | Decimation | Sample Rate  |
+==================+============+==============+
| 1 MHz            | 64         | 15.625 kHz   |
+------------------+------------+--------------+
| 2 MHz            | 64         | 31.25 kHz    |
+------------------+------------+--------------+
| 1 MHz            | 32         | 31.25 kHz    |
+------------------+------------+--------------+
| 500 kHz          | 32         | 15.625 kHz   |
+------------------+------------+--------------+