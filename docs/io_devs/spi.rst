SPI - Serial Peripheral Interface
==================================

Overview
--------

The Serial Peripheral Interface (SPI) is a synchronous serial communication protocol for short-distance, high-speed data transfer in embedded systems. Operating on a master-slave architecture, SPI enables efficient communication between a master device and one or more slave devices using a minimal pin count.

The ZenithSoC SPI implementation provides a full-duplex master-mode controller with configurable clock speed, phase, and polarity. The design emphasizes simplicity and low resource usage while maintaining high performance and flexibility.

Key Features
------------

* **Master Mode**: Single master controlling multiple slaves
* **Full-Duplex**: Simultaneous bidirectional data transfer
* **High-Speed**: Configurable clock up to System Clock / 2
* **Four SPI Modes**: CPOL and CPHA configuration for all standard modes
* **FIFO Buffering**: Independent TX/RX buffers (configurable depth, default 16 bytes)
* **Multi-Slave Support**: Up to 32 chip select lines (configurable parameter)
* **Bit Order**: MSB-first or LSB-first transmission
* **16-bit Clock Divider**: Wide range of SPI clock frequencies
* **Interrupt Support**: Transaction complete interrupt
* **Input Synchronization**: 2-stage synchronizer on MISO line

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The SPI controller consists of the following main components:

* **Register Bank**: Control, status, and data registers
* **Clock Generator**: Programmable frequency divider with CPOL/CPHA support
* **TX Path**: Transmit FIFO, shift register, bit order control
* **RX Path**: Receive FIFO, shift register, input synchronizer
* **FSM Controller**: Clock generation state machine
* **Chip Select Logic**: Multi-slave select control

Signal Description
~~~~~~~~~~~~~~~~~~

===============================  =========  ========  ====================================
Signal Name                      Direction  Width     Description
===============================  =========  ========  ====================================
**Global**
``clk_i``                        Input      1         System clock
``rst_n_i``                      Input      1         Active-low asynchronous reset
``interrupt_o``                  Output     1         Interrupt request
**SPI Interface**
``sclk_o``                       Output     1         SPI clock (to slaves)
``mosi_o``                       Output     1         Master Out Slave In data line
``miso_i``                       Input      1         Master In Slave Out data line
``cs_n_o[SLAVES-1:0]``           Output     N         Chip select lines (active low)
**AXI Write Interface**
``write_i``                      Input      1         Write enable
``write_address_i[2:0]``         Input      3         Register address
``write_data_i[31:0]``           Input      32        Write data
``write_strobe_i[3:0]``          Input      4         Byte write enable mask
``write_done_o``                 Output     1         Write acknowledge
``write_error_o``                Output     1         Write error (invalid operation)
**AXI Read Interface**
``read_i``                       Input      1         Read enable
``read_address_i[2:0]``          Input      3         Register address
``read_data_o[31:0]``            Output     32        Read data
``read_done_o``                  Output     1         Read acknowledge (1 cycle delay for RX read)
``read_error_o``                 Output     1         Read error (invalid operation)
===============================  =========  ========  ====================================

**Parameter**: SLAVES = Number of chip select lines (default 1, max 32)

Functional Description
----------------------

SPI Protocol Basics
~~~~~~~~~~~~~~~~~~~

SPI uses a four-wire interface for communication:

* **SCLK (Serial Clock)**: Master-generated clock signal
* **MOSI (Master Out Slave In)**: Data from master to slave
* **MISO (Master In Slave Out)**: Data from slave to master
* **CS_N (Chip Select, active low)**: Slave selection signal

**Full-Duplex Operation**: Data transmitted on MOSI and received on MISO simultaneously during each clock cycle.

SPI Modes
~~~~~~~~~

SPI supports four modes defined by CPOL (Clock Polarity) and CPHA (Clock Phase):

=======  ======  ======  ===================  ====================
Mode     CPOL    CPHA    Clock Idle State     Data Capture Edge
=======  ======  ======  ===================  ====================
0        0       0       Low                  Rising edge
1        0       1       Low                  Falling edge
2        1       0       High                 Falling edge
3        1       1       High                 Rising edge
=======  ======  ======  ===================  ====================

**CPOL (Clock Polarity)**:
  * 0: Clock idle low, active high
  * 1: Clock idle high, active low

**CPHA (Clock Phase)**:
  * 0: Data captured on first clock edge, shifted on second edge
  * 1: Data shifted on first clock edge, captured on second edge

Clock Generation
~~~~~~~~~~~~~~~~

The SPI clock frequency is derived from the system clock:

**SCLK Formula**:

.. code-block:: text

   SCLK Frequency = System Clock / (2 × (Divisor + 1))

**Example** (100 MHz system clock):

* Divisor = 0: SCLK = 50 MHz
* Divisor = 1: SCLK = 25 MHz
* Divisor = 9: SCLK = 5 MHz
* Divisor = 49: SCLK = 1 MHz
* Divisor = 499: SCLK = 100 kHz

**Clock Characteristics**:
  * 50% duty cycle
  * Configurable polarity (CPOL)
  * Configurable phase (CPHA)

TX/RX Operation
~~~~~~~~~~~~~~~

**Transmission/Reception Flow**:

1. Software writes data bytes to TX FIFO via TX_BUFFER register
2. Software selects slave via SLAVE_SELECT register
3. When TX FIFO has data and FSM idle:
   - Assert chip select (CS_N low for selected slave)
   - Load byte from TX FIFO into shift register
   - Generate 8 clock cycles
   - Shift out bits on MOSI (MSB or LSB first)
   - Sample bits from MISO (synchronized)
   - After 8 bits, write received byte to RX FIFO
4. Deassert chip select (CS_N high)
5. Generate transaction complete interrupt (if enabled)
6. Repeat for next byte in TX FIFO

**TX FIFO**:
  * Depth: Configurable (default 16 bytes)
  * Status: Full/empty flags in status register
  * Triggers transaction when data available

**RX FIFO**:
  * Depth: Configurable (default 16 bytes)
  * Status: Full/empty flags in status register
  * One byte written per transaction

**Simultaneous Operation**: Each SPI transaction transmits and receives simultaneously (full-duplex).

Bit Order
~~~~~~~~~

Data can be transmitted/received in two orders:

**MSB First** (Most Significant Bit First):
  * Bit 7 transmitted/sampled first
  * Standard for most SPI devices

**LSB First** (Least Significant Bit First):
  * Bit 0 transmitted/sampled first
  * Less common, device-specific

Multi-Slave Support
~~~~~~~~~~~~~~~~~~~

The SPI controller supports multiple slaves via independent chip select lines:

* Up to 32 slaves (configurable parameter)
* Only one slave active at a time
* Chip select lines are active-low
* Software controls which slave is selected via SLAVE_SELECT register

**Chip Select Behavior**:
  * Asserted (low) during transaction
  * Deasserted (high) between transactions
  * Allows slaves to tri-state MISO when not selected

Input Synchronization
~~~~~~~~~~~~~~~~~~~~~

The MISO line passes through a 2-stage synchronizer:

* Prevents metastability from asynchronous slave responses
* Adds 2 clock cycle latency
* Ensures stable sampling

Clock FSM Description
---------------------

State Machine
~~~~~~~~~~~~~

The SPI controller uses an FSM to generate clock edges and control data transfer:

.. code-block:: text

   IDLE → PHASE_DELAY → CLOCK_LOW ⇄ CLOCK_HIGH → IDLE

State Definitions
~~~~~~~~~~~~~~~~~

**IDLE**
  * Wait for data in TX FIFO
  * All chip selects deasserted (high)
  * SCLK at idle state (determined by CPOL)
  * Reset bit counter
  * **Transition**:
    - data_available & CPHA=1 → PHASE_DELAY
    - data_available & CPHA=0 → CLOCK_LOW/HIGH (based on CPOL)

**PHASE_DELAY** (CPHA=1 only)
  * Initial half-cycle delay before first data edge
  * SCLK transitions to opposite of CPOL
  * Sample MISO (CPHA=1, data valid before first edge)
  * **Duration**: (Divisor + 1) cycles
  * **Transition**: counter == divisor → CLOCK_LOW/HIGH (based on CPOL)

**CLOCK_LOW**
  * SCLK held low
  * Depending on mode:
    - CPHA=0, CPOL=0: Sample MISO (rising edge coming)
    - CPHA=1, CPOL=0: Shift MOSI (falling edge just occurred)
  * Increment counter
  * **Duration**: (Divisor + 1) cycles
  * **Transition**: counter == divisor → CLOCK_HIGH

**CLOCK_HIGH**
  * SCLK held high
  * Depending on mode:
    - CPHA=0, CPOL=1: Sample MISO (falling edge coming)
    - CPHA=1, CPOL=1: Shift MOSI (rising edge just occurred)
  * Increment counter and bit counter
  * **Duration**: (Divisor + 1) cycles
  * **Transition**:
    - bit_counter < 8: counter == divisor → CLOCK_LOW
    - bit_counter == 8: counter == divisor → IDLE

FSM Control Signals
~~~~~~~~~~~~~~~~~~~

* ``sample_data``: Latch MISO bit into RX shift register (at capture edge)
* ``shift_data``: Shift TX shift register, output next bit on MOSI (at shift edge)
* ``bit_sent``: Increment bit counter (every complete bit cycle)
* ``tx_read``: Pop byte from TX FIFO (when starting new transaction)
* ``rx_write``: Push byte to RX FIFO (when 8 bits complete)
* ``start_transaction``: Load TX shift register from FIFO
* ``trx_done``: Transaction complete, generate interrupt
* ``trx_idle``: FSM in IDLE state
* ``cs_n_o``: Chip select outputs (low during transaction)

Mode-Specific Behavior
~~~~~~~~~~~~~~~~~~~~~~~

**Mode 0 (CPOL=0, CPHA=0)**:
  * Idle: SCLK low, no delay
  * Rising edge: Sample MISO
  * Falling edge: Shift MOSI
  * Flow: IDLE → CLOCK_LOW (sample) → CLOCK_HIGH (shift) → ...

**Mode 1 (CPOL=0, CPHA=1)**:
  * Idle: SCLK low, initial delay
  * Falling edge: Sample MISO
  * Rising edge: Shift MOSI
  * Flow: IDLE → PHASE_DELAY (sample) → CLOCK_HIGH (shift) → CLOCK_LOW (sample) → ...

**Mode 2 (CPOL=1, CPHA=0)**:
  * Idle: SCLK high, no delay
  * Falling edge: Sample MISO
  * Rising edge: Shift MOSI
  * Flow: IDLE → CLOCK_HIGH (sample) → CLOCK_LOW (shift) → ...

**Mode 3 (CPOL=1, CPHA=1)**:
  * Idle: SCLK high, initial delay
  * Rising edge: Sample MISO
  * Falling edge: Shift MOSI
  * Flow: IDLE → PHASE_DELAY (sample) → CLOCK_LOW (shift) → CLOCK_HIGH (sample) → ...

Register Map
------------

Base Address
~~~~~~~~~~~~

The SPI base address is determined by the SoC memory map.

Register Summary
~~~~~~~~~~~~~~~~

===========  =============  ======  =====  =====================================
Address      Name           Access  Reset  Description
===========  =============  ======  =====  =====================================
0x00         STATUS         R/W     *      Status and control register
0x04         TX_BUFFER      W       0x00   TX data buffer (byte write)
0x08         RX_BUFFER      R       0x00   RX data buffer (byte read)
0x0C         EVENT          R/W     0x00   Interrupt event register
0x10         SLAVE_SELECT   R/W     0x00   Chip select control (active low)
===========  =============  ======  =====  =====================================

*Reset: All zeros (Mode 0, divisor 0, interrupts disabled)*

Register Descriptions
---------------------

STATUS Register (0x00)
~~~~~~~~~~~~~~~~~~~~~~

**SPI Status and Control Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:25]        R/W     0x00   **Clock Divisor [15:9]**: Upper 7 bits
[24:17]        R/W     0x00   **Clock Divisor [8:1]**: Middle 8 bits
[16:10]        -       0x00  Reserved
[9]            R/W     0x0    **Clock Divisor [0]**: LSB
[8]            R/W     0x0    **Bit Order**: 0=MSB first, 1=LSB first
[7]            R/W     0x0    **CPOL**: Clock polarity (0=idle low, 1=idle high)
[6]            R/W     0x0    **CPHA**: Clock phase (0=capture 1st edge, 1=capture 2nd edge)
[5]            R/W     0x0    **Interrupt Enable**: Enable transaction complete interrupt
[4]            R       0x1    **Idle**: SPI controller idle (not transmitting)
[3]            R       0x0    **RX Full**: RX FIFO full
[2]            R       0x1    **RX Empty**: RX FIFO empty
[1]            R       0x0    **TX Full**: TX FIFO full
[0]            R       0x1    **TX Empty**: TX FIFO empty
=============  ======  =====  =============================================

**Clock Divisor Encoding**:
  * Bits [31:25] → Divisor[15:9]
  * Bits [24:17] → Divisor[8:1]
  * Bit [9] → Divisor[0]
  * Combined to form 16-bit divisor value

**SPI Mode Selection**:

* Mode 0: CPOL=0, CPHA=0
* Mode 1: CPOL=0, CPHA=1
* Mode 2: CPOL=1, CPHA=0
* Mode 3: CPOL=1, CPHA=1

TX_BUFFER Register (0x04, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TX Data Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       -      Ignored
[7:0]          W       0x00   Transmit data byte
=============  ======  =====  =============================================

Write a byte to enqueue into TX FIFO. Transaction starts automatically when data available.

RX_BUFFER Register (0x08, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**RX Data Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       0x00   Reserved (read as 0)
[7:0]          R       0x00   Received data byte
=============  ======  =====  =============================================

Read a byte to dequeue from RX FIFO. Check RX_Empty flag before reading.

**Note**: Read has 1 cycle latency.

EVENT Register (0x0C, R/W)
~~~~~~~~~~~~~~~~~~~~~~~~~~

**Interrupt Event Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:1]         -       0x00   Reserved
[0]            R/W     0x0    **Transaction Complete**: Byte transmitted/received
=============  ======  =====  =============================================

**Write**: Write 0 to clear event flag

**Read**: Returns current event status

**Interrupt**: Edge-triggered on 0→1 transition

SLAVE_SELECT Register (0x10, R/W)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Chip Select Control**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         R/W     0x00   Chip select bits (one per slave, active high)
=============  ======  =====  =============================================

**Encoding**: Write 1 to bit N to select slave N (CS_N[N] driven low)

**Example**:
  * 0x00000001 → Select slave 0 (CS_N[0] = 0, others = 1)
  * 0x00000002 → Select slave 1 (CS_N[1] = 0, others = 1)
  * 0x00000000 → No slave selected (all CS_N = 1)

**Note**: Only select one slave at a time. Hardware inverts bits to generate active-low CS_N signals.

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial SPI documentation
========  ==========  ============  ===============================================
