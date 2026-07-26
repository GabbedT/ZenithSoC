UART - Universal Asynchronous Receiver/Transmitter
==================================================

Overview
--------

The ZenithSoC UART implementation features independent TX/RX paths with FIFO buffering, configurable frame format, hardware flow control, and interrupt support.

Key Features
------------

* **Full-Duplex Communication**: Independent transmit and receive paths
* **Configurable Baud Rate**: 15-bit clock divider for wide baud rate range
* **Frame Format**: Configurable data bits (5-8), optional even/odd parity, and stop bits (1-2)
* **Hardware Flow Control**: RTS/CTS handshaking support
* **FIFO Buffering**: Independent TX/RX buffers (2 KiB each in the SoC integration)
* **Interrupt System**: Five interrupt sources (RX done, TX done, RX full, TX empty, parity error)
* **Oversampling**: 16× oversampling for robust bit detection
* **Input Synchronization**: 2-stage synchronizer for metastability protection
* **Low Resource Usage**: Minimal logic footprint and power consumption

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The UART controller consists of the following main components:

* **Register Bank**: Control, status, and data registers
* **Baud Generator**: Programmable clock divider for baud rate generation
* **TX Path**: Transmit FSM, TX FIFO, parity generator
* **RX Path**: Receive FSM, RX FIFO, parity checker, input synchronizer
* **Flow Control**: RTS/CTS logic
* **Interrupt Controller**: Edge-triggered interrupt generation

Signal interface
~~~~~~~~~~~~~~~~~~

Port directions are relative to the ``uart`` module. The register signals
form the local SoC memory-mapped interface, rather than a standalone AXI
channel. Addresses are word indices inside the UART register window and
``write_strobe_i`` enables individual bytes of ``write_data_i``.

.. list-table:: UART module signals
   :header-rows: 1
   :widths: 34 12 14 40

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
     - Configured UART event interrupt request.
   * - ``write_i`` / ``read_i``
     - Input
     - 1 each
     - Register write and read requests.
   * - ``write_address_i[1:0]`` / ``read_address_i[1:0]``
     - Input
     - 2 each
     - Word indices in the UART register window.
   * - ``write_data_i[31:0]``
     - Input
     - 32
     - Register write data.
   * - ``write_strobe_i[3:0]``
     - Input
     - 4
     - Byte write enables.
   * - ``write_done_o`` / ``read_done_o``
     - Output
     - 1 each
     - Register transaction completion responses; RX-buffer reads add one
       cycle of read latency.
   * - ``write_error_o`` / ``read_error_o``
     - Output
     - 1 each
     - Invalid register access responses.
   * - ``uart_tx_o`` / ``uart_rx_i``
     - Output / Input
     - 1 each
     - UART transmit and receive data lines.
   * - ``uart_rts_o`` / ``uart_cts_i``
     - Output / Input
     - 1 each
     - Optional active flow-control signals.
   * - ``uart_tx_full_o``
     - Output
     - 1
     - TX FIFO full flag used by the Trace Unit.
   * - ``trace_data_i[7:0]`` / ``trace_write_i``
     - Input
     - 8 / 1
     - Serialized trace byte and write strobe from the Trace Unit.

When tracing is enabled, the trace unit can write serialized bytes directly
into the UART TX FIFO. This path shares the FIFO's full/backpressure status
with normal software writes and is connected to UART instance 0 in the SoC.

Functional Description
----------------------

Frame Format
~~~~~~~~~~~~

UART frames consist of:

.. code-block:: text

   START | DATA (5-8 bits) | PARITY (optional) | STOP (1-2 bits)

**Start Bit**: Logic 0, signals beginning of frame

**Data Bits**: 5, 6, 7, or 8 bits (LSB first)

**Parity Bit** (optional):
  * Even parity: XOR of all data bits = 0
  * Odd parity: XOR of all data bits = 1

**Stop Bits**: Logic 1, 1 or 2 bits duration

**Idle State**: Line held at logic 1

Baud Rate Generation
~~~~~~~~~~~~~~~~~~~~

The baud rate generator uses a 15-bit clock divider:

**Baud Rate Formula**:

.. code-block:: text

   Baud Rate = System Clock / (16 × (Divider + 1))

**Example** (100 MHz system clock):

* 115200 baud: Divider = 53 → 100M/(16×54) = 115740 baud (-0.47% error)
* 9600 baud: Divider = 650 → 100M/(16×651) = 9600 baud (0% error)

The 16× oversampling provides robust sampling at the center of each bit period.

TX Path Operation
~~~~~~~~~~~~~~~~~

**Transmission Flow**:

1. Software writes data to TX FIFO via TX_BUFFER register
2. TX FSM monitors FIFO and CTS signal (if flow control enabled)
3. When data available and CTS asserted (or flow control disabled):
   - Pop byte from FIFO
   - Transmit start bit
   - Transmit data bits (LSB first)
   - Compute and transmit parity bit (if enabled)
   - Transmit stop bit(s)
4. Generate TX done interrupt (if enabled)
5. Repeat for next byte in FIFO

**TX FIFO**:
  * Depth: Configurable (2 KiB in the SoC integration)
  * Status: Full/empty flags visible in status register
  * Interrupt: TX empty interrupt when FIFO becomes empty

RX Path Operation
~~~~~~~~~~~~~~~~~

**Reception Flow**:

1. RX line synchronized via 2-stage synchronizer
2. RX FSM detects start bit (1 → 0 transition)
3. Oversample at 16× baud rate for robust bit detection
4. Sample bit value at center (8th sample of 16)
5. Receive data bits (LSB first)
6. Receive and verify parity bit (if enabled)
7. Verify stop bit(s)
8. Write byte to RX FIFO
9. Assert RTS low if FIFO full (if flow control enabled)
10. Generate RX done interrupt (if enabled)

**RX FIFO**:
  * Depth: Configurable (2 KiB in the SoC integration)
  * Status: Full/empty flags visible in status register
  * Interrupt: RX full interrupt when FIFO becomes full
  * Overrun: New data discarded if FIFO full

**Parity Error Handling**:
  * Parity mismatch sets error flag
  * Data still written to FIFO
  * Parity error interrupt generated (if enabled)

Hardware Flow Control
~~~~~~~~~~~~~~~~~~~~~

**RTS (Request-To-Send)** - Output:
  * Asserted (low) when RX FIFO has space
  * Deasserted (high) when RX FIFO full
  * Signals remote device to stop/resume transmission

**CTS (Clear-To-Send)** - Input:
  * TX waits for CTS assertion before transmitting
  * Prevents overrun of remote device's RX buffer

**Enable/Disable**:
  * Flow control can be enabled/disabled via status register
  * When disabled, RTS held high, CTS ignored

Oversampling and Synchronization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**16× Oversampling**:
  * Each bit sampled 16 times
  * Bit value determined at 8th sample (center of bit period)
  * Provides ±6.25% timing margin for baud rate mismatch

**Input Synchronization**:
  * 2-stage flip-flop synchronizer on RX line
  * Prevents metastability from asynchronous input
  * Adds 2 clock cycle latency

Interrupt System
~~~~~~~~~~~~~~~~

Five interrupt sources (edge-triggered):

1. **RX Data Received**: Byte received and written to RX FIFO
2. **TX Data Transmitted**: Byte transmitted from TX FIFO
3. **RX Buffer Full**: RX FIFO full condition
4. **TX Buffer Empty**: TX FIFO empty condition
5. **RX Parity Error**: Parity mismatch detected

Each interrupt can be individually enabled/disabled via status register.

TX FSM Description
------------------

State Machine
~~~~~~~~~~~~~

The TX FSM implements UART frame transmission:

.. code-block:: text

   IDLE → START → DATA → PARITY → STOP → IDLE

State Definitions
~~~~~~~~~~~~~~~~~

**IDLE**
  * Wait for data in TX FIFO
  * Check CTS signal if flow control enabled
  * TX line held high (idle state)
  * **Transition**: data_available & (cts | !flow_control) → START

**START**
  * Transmit start bit (logic 0)
  * Load data byte from FIFO
  * Initialize parity accumulator
  * Reset bit counter
  * **Duration**: 16 sample periods (1 bit time)
  * **Transition**: sampling_counter == 15 → DATA

**DATA**
  * Transmit data bits LSB first
  * Shift data register right after each bit
  * Update parity accumulator with each bit
  * Increment bit counter
  * **Duration**: 5-8 bit times (depending on data_bits config)
  * **Transition**: bit_count == data_bits_max → PARITY (if enabled) or STOP

**PARITY** (conditional)
  * Transmit computed parity bit
  * Even parity: XOR of all data bits
  * Odd parity: XOR of all data bits inverted
  * **Duration**: 16 sample periods (1 bit time)
  * **Transition**: sampling_counter == 15 → STOP

**STOP**
  * Transmit stop bit(s) (logic 1)
  * Assert TX done signal
  * **Duration**: 16 or 32 sample periods (1 or 2 bit times)
  * **Transition**: stop_bits_sent == stop_bits_config → IDLE


RX FSM Description
------------------

State Machine
~~~~~~~~~~~~~

The RX FSM implements UART frame reception:

.. code-block:: text

   IDLE → START → DATA → PARITY → STOP → IDLE

State Definitions
~~~~~~~~~~~~~~~~~

**IDLE**
  * Monitor RX line for start bit (1 → 0 transition)
  * Line synchronized via 2-stage synchronizer
  * **Transition**: rx_line == 0 → START

**START**
  * Validate start bit
  * Wait for center of bit period (8th sample)
  * Verify RX line still low
  * Reset all counters and registers
  * Initialize parity accumulator
  * **Duration**: 16 sample periods
  * **Frame Error**: If RX line high at center, return to IDLE
  * **Transition**: sampling_counter == 7 & rx_line == 0 → DATA

**DATA**
  * Sample RX line at center of each bit period
  * Shift received bit into RX data register (MSB direction)
  * Update parity accumulator with each bit
  * Increment bit counter
  * **Duration**: 5-8 bit times (depending on data_bits config)
  * **Sampling Point**: 8th of 16 samples per bit
  * **Transition**: bit_count == data_bits_max → PARITY (if enabled) or STOP

**PARITY** (conditional)
  * Sample parity bit at center of bit period
  * Compare received parity with computed parity
  * Set parity_error flag if mismatch
  * **Duration**: 16 sample periods (1 bit time)
  * **Transition**: sampling_counter == 7 → STOP

**STOP**
  * Verify stop bit(s) are logic 1
  * Write received byte to RX FIFO (if space available)
  * Assert RX done signal
  * **Duration**: 16 or 32 sample periods (1 or 2 bit times)
  * **Frame Error**: If stop bit not high, set error flag
  * **Overrun**: If FIFO full, discard byte
  * **Transition**: stop_bits_verified == stop_bits_config → IDLE


Error Handling
~~~~~~~~~~~~~~

**Parity Error**:
  * Detected in PARITY state
  * Data still written to FIFO
  * Error flag set, interrupt generated

**Framing Error**:
  * Start bit not low at center sample
  * Stop bit not high
  * Frame discarded, FSM returns to IDLE

**Overrun Error**:
  * RX FIFO full when new byte received
  * New data discarded
  * RX full interrupt generated

Register Map
------------

Base Address
~~~~~~~~~~~~

The UART base address is determined by the SoC memory map.

Register Summary
~~~~~~~~~~~~~~~~

===========  ===========  ======  =====  =====================================
Address      Name         Access  Reset  Description
===========  ===========  ======  =====  =====================================
0x00         STATUS       R/W     *      Status and control register
0x04         TX_BUFFER    W       0x00   TX data buffer (byte write)
0x08         RX_BUFFER    R       0x00   RX data buffer (byte read)
0x0C         EVENT        R/W     0x00   Interrupt event register
===========  ===========  ======  =====  =====================================

*Reset value: 115200 baud @ 100MHz clock*

Register Descriptions
---------------------

STATUS Register (0x00)
~~~~~~~~~~~~~~~~~~~~~~

**UART Status and Control Register**

=============  ======  =======  =============================================
Bit Field      Access  Reset    Description
=============  ======  =======  =============================================
[31:24]        R/W     0x00     **Clock Divider [14:7]**: Upper 8 bits
[23:17]        R/W     0x35     **Clock Divider [6:0]**: Lower 7 bits (default=53)
[16]           R/W     0x0      **Flow Control Enable**: 0=Disabled, 1=Enabled
[15:14]        R/W     0x3      **Data Bits**: 0=5bit, 1=6bit, 2=7bit, 3=8bit
[13]           R/W     0x0      **Stop Bits**: 0=1 stop bit, 1=2 stop bits
[12]           R/W     0x0      **Parity Mode**: 0=Even, 1=Odd
[11]           R/W     0x1      **Parity Enable**: 0=Disabled, 1=Enabled
[10]           R/W     0x0      **TX Enable**: Enable transmitter
[9]            R/W     0x0      **RX Enable**: Enable receiver
[8]            R/W     0x0      **Parity Error Int Enable**: Enable parity error interrupt
[7]            R/W     0x0      **TX Empty Int Enable**: Enable TX empty interrupt
[6]            R/W     0x0      **RX Full Int Enable**: Enable RX full interrupt
[5]            R/W     0x0      **TX Done Int Enable**: Enable TX done interrupt
[4]            R/W     0x0      **RX Done Int Enable**: Enable RX done interrupt
[3]            R       0x0      **RX Full**: RX FIFO full flag
[2]            R       0x1      **RX Empty**: RX FIFO empty flag
[1]            R       0x0      **TX Full**: TX FIFO full flag
[0]            R       0x1      **TX Empty**: TX FIFO empty flag
=============  ======  =======  =============================================

**Baud Rate Calculation**:

.. code-block:: text

   Clock Divider = (System Clock / (16 × Desired Baud)) - 1

**Common Baud Rates** (100 MHz clock):

* 9600: Divider = 650
* 19200: Divider = 325
* 38400: Divider = 162
* 57600: Divider = 108
* 115200: Divider = 53 (default)
* 230400: Divider = 26
* 460800: Divider = 13
* 921600: Divider = 6

TX_BUFFER Register (0x04, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TX Data Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       -      Ignored
[7:0]          W       0x00   Transmit data byte
=============  ======  =====  =============================================

Write a byte to enqueue it into TX FIFO. If FIFO full, byte is discarded.

RX_BUFFER Register (0x08, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**RX Data Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       0x00   Reserved (read as 0)
[7:0]          R       0x00   Received data byte
=============  ======  =====  =============================================

Read a byte to dequeue it from RX FIFO. If FIFO empty, reads 0x00.

**Note**: Read has 1 cycle latency. Check RX_Empty flag before reading.

EVENT Register (0x0C, R/W)
~~~~~~~~~~~~~~~~~~~~~~~~~~

**Interrupt Event Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:5]         -       0x00   Reserved
[4]            R/W     0x0    **Parity Error**: Parity mismatch detected
[3]            R/W     0x0    **TX Empty**: TX FIFO empty
[2]            R/W     0x0    **RX Full**: RX FIFO full
[1]            R/W     0x0    **TX Done**: Byte transmitted
[0]            R/W     0x0    **RX Done**: Byte received
=============  ======  =====  =============================================

**Write**: Write bit value to set/clear specific event flag

**Read**: Returns current event flags

**Interrupt Generation**: Edge-triggered on any bit transition 0→1

**Parity Errors**: Check EVENT register after read
**Overruns**: Monitor RX full flag, increase RX buffer or reduce baud rate

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial UART documentation
========  ==========  ============  ===============================================
