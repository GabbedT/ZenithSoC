SD - Secure Digital Card Controller
====================================

Overview
--------

The Secure Digital (SD) Card Controller provides a hardware interface for accessing SD/SDHC/SDXC memory cards using the SD protocol. The controller implements the command and data paths required for card initialization, configuration, and data transfer operations in compliance with the SD Card Physical Layer Specification.

The ZenithSoC SD implementation features independent command and data controllers with hardware CRC generation/verification, configurable bus width, dual clock speed support, and FIFO-buffered data paths for efficient block transfers.

Key Features
------------

* **SD Protocol Compliance**: Physical layer compatible with SD 2.0+ specifications
* **Bus Width**: 1-bit or 4-bit data bus (configurable)
* **Dual Clock Speed**: 400 kHz (initialization) and 25 MHz (high-speed transfer)
* **Hardware CRC**: Automatic CRC7 (command) and CRC16 (data) generation/verification
* **Buffered Data Path**: Independent TX/RX FIFOs for block data
* **Command/Response**: Full command set support with 48-bit and 136-bit responses
* **Burst Transfer**: Multi-block read/write operations
* **Card Detection**: Hardware card detect input
* **Interrupt System**: Nine interrupt sources for event notification
* **Error Detection**: Timeout and CRC error detection

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The SD controller consists of the following main components:

* **Register Bank**: Control, status, command, and data registers
* **Command Controller**: CMD line protocol, CRC7 engine, response capture
* **Data Controller**: 1/4-bit data path, CRC16 engines, token handling
* **Clock Generator**: Programmable divider for 400kHz/25MHz operation
* **Reset Controller**: Card power-on reset generation
* **TX/RX Buffers**: FIFOs for command response and data blocks
* **Interrupt Controller**: Edge-triggered interrupt generation

Signal Description
~~~~~~~~~~~~~~~~~~

===============================  =========  ========  ====================================
Signal Name                      Direction  Width     Description
===============================  =========  ========  ====================================
**Global**
``clk_i``                        Input      1         System clock (100MHz typical)
``rst_n_i``                      Input      1         Active-low asynchronous reset
``interrupt_o``                  Output     1         Interrupt request
**SD Card Interface**
``sd_clk_o``                     Output     1         SD clock (400kHz or 25MHz)
``sd_cmd_io``                    Inout      1         Command/response bidirectional line
``sd_data_io[3:0]``              Inout      4         Data lines (DAT0-DAT3)
``sd_cd_n_i``                    Input      1         Card detect (active low)
``sd_reset_n_o``                 Output     1         Card reset (active low)
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
``read_done_o``                  Output     1         Read acknowledge
``read_error_o``                 Output     1         Read error (invalid operation)
===============================  =========  ========  ====================================

Functional Description
----------------------

SD Protocol Basics
~~~~~~~~~~~~~~~~~~

The SD card interface uses a command-response-data protocol:

**Command Line (CMD)**:
  * Bidirectional serial line
  * Master (host) sends commands
  * Slave (card) sends responses
  * Protected by CRC7

**Data Lines (DAT0-DAT3)**:
  * Bidirectional parallel data
  * 1-bit mode: DAT0 only
  * 4-bit mode: DAT0-DAT3 parallel
  * Protected by CRC16 per line

**Clock Line (CLK)**:
  * Unidirectional from host
  * 400 kHz during initialization
  * 25 MHz during data transfer

Card Initialization Sequence
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1. **Card Detection**: Monitor ``sd_cd_n_i`` signal
2. **Power-On Reset**: Assert ``sd_reset_n_o`` for ~1ms
3. **Clock Setup**: Start with 400 kHz clock
4. **CMD0** (GO_IDLE_STATE): Reset card to idle state
5. **CMD8** (SEND_IF_COND): Check voltage and card version
6. **ACMD41** (SD_SEND_OP_COND): Initialize card, get OCR
7. **CMD2** (ALL_SEND_CID): Get Card Identification
8. **CMD3** (SEND_RELATIVE_ADDR): Get Relative Card Address (RCA)
9. **CMD7** (SELECT_CARD): Select card for data transfer
10. **ACMD6** (SET_BUS_WIDTH): Configure 4-bit mode (optional)
11. **Clock Switch**: Increase to 25 MHz for high-speed operation

Command Operation
~~~~~~~~~~~~~~~~~

**Command Structure** (48 bits):

.. code-block:: text

   [47:46] Start bits (01)
   [45:40] Command index (CMD0-CMD63)
   [39:8]  Argument (32 bits)
   [7:1]   CRC7 checksum
   [0]     End bit (1)

**Response Types**:

* **R1 (48-bit)**: Normal response with card status
* **R2 (136-bit)**: CID/CSD register contents
* **R3 (48-bit)**: OCR register (no CRC)
* **R6 (48-bit)**: Published RCA response
* **R7 (48-bit)**: Card interface condition

**Command Flow**:

1. Software writes command number and argument to registers
2. Software triggers command transmission via control register
3. Command controller shifts out command on CMD line
4. Card responds after N cycles (card-dependent)
5. Response captured and stored in response FIFO
6. Interrupt generated on completion, timeout, or CRC error

Data Transfer Operation
~~~~~~~~~~~~~~~~~~~~~~~

**Block Read**:

1. Send CMD17 (READ_SINGLE_BLOCK) or CMD18 (READ_MULTIPLE_BLOCK)
2. Card sends start token (0xFE for single, 0xFC for multiple)
3. Card sends data block (512 bytes typical)
4. Card sends CRC16 (16 bits per data line)
5. Data written to RX FIFO
6. For multi-block: repeat until CMD12 (STOP_TRANSMISSION)

**Block Write**:

1. Send CMD24 (WRITE_BLOCK) or CMD25 (WRITE_MULTIPLE_BLOCK)
2. Host sends start token
3. Host sends data from TX FIFO (512 bytes)
4. Host sends CRC16
5. Card responds with data response token
6. Card signals busy by holding DAT0 low
7. For multi-block: repeat until CMD12

**Data Tokens**:
  * 0xFE: Single block read start
  * 0xFC: Multiple block read start
  * 0xFD: Multiple block write start
  * 0xE5: Data accepted
  * 0xEB: Data rejected (CRC error)
  * 0xED: Data rejected (write error)

Clock Generation
~~~~~~~~~~~~~~~~

The SD clock is derived from the system clock:

**400 kHz Clock** (initialization):

.. code-block:: text

   Divisor = 125 (for 100MHz system clock)
   SD_CLK = 100MHz / (125 × 2) = 400 kHz

**25 MHz Clock** (data transfer):

.. code-block:: text

   Divisor = 2 (for 100MHz system clock)
   SD_CLK = 100MHz / (2 × 2) = 25 MHz

**Clock Control**:
  * 50% duty cycle
  * Configurable via control register
  * Shift and sample edges generated internally

Bus Width Configuration
~~~~~~~~~~~~~~~~~~~~~~~

**1-bit Mode**:
  * Only DAT0 used
  * Sequential bit transfer
  * Compatible with all SD cards
  * Slower transfer rate

**4-bit Mode**:
  * DAT0-DAT3 used in parallel
  * 4× throughput vs 1-bit mode
  * Requires card support and ACMD6 configuration
  * CRC16 computed independently per line

Hardware CRC
~~~~~~~~~~~~

**CRC7 (Command/Response)**:
  * Polynomial: x^7 + x^3 + 1 (0x89)
  * Protects command and response integrity
  * Automatic generation on TX, verification on RX

**CRC16 (Data)**:
  * Polynomial: x^16 + x^12 + x^5 + 1 (0x1021)
  * Independent CRC per data line (4 CRCs in 4-bit mode)
  * Automatic generation on TX, verification on RX

Interrupt System
~~~~~~~~~~~~~~~~

Nine interrupt sources (edge-triggered):

1. **Card Detected**: Card insertion detected
2. **TX Buffer Empty**: TX FIFO empty (ready for more data)
3. **RX Buffer Full**: RX FIFO full (read data promptly)
4. **Command Done**: Command/response completed successfully
5. **Command CRC Error**: Response CRC mismatch
6. **Command Timeout**: No response received
7. **Data Done**: Data transfer completed successfully
8. **Data CRC Error**: Data CRC mismatch
9. **Data Timeout**: No data token received

Each interrupt can be individually enabled/disabled via control register.

Command FSM Description
-----------------------

The command controller implements command transmission and response reception.

**States** (simplified):

.. code-block:: text

   IDLE → TX_START → TX_CMD → TX_ARG → TX_CRC → WAIT_RESP → RX_RESP → IDLE

**Key Operations**:

* **TX_CMD**: Shift out 6-bit command index
* **TX_ARG**: Shift out 32-bit argument
* **TX_CRC**: Compute and shift out CRC7
* **WAIT_RESP**: Wait for response start (timeout if not received)
* **RX_RESP**: Capture 48-bit or 136-bit response
* **CRC Check**: Verify response CRC7, set error flag if mismatch

**Timeout**: Configurable timeout for response detection

Data FSM Description
--------------------

The data controller handles data block transfers with CRC.

**Read States** (simplified):

.. code-block:: text

   IDLE → WAIT_TOKEN → RX_DATA → RX_CRC → IDLE

**Write States** (simplified):

.. code-block:: text

   IDLE → TX_TOKEN → TX_DATA → TX_CRC → WAIT_RESP → IDLE

**Key Operations**:

* **WAIT_TOKEN**: Detect start token on DAT0 (timeout if not received)
* **RX_DATA**: Sample data lines, write to RX FIFO
* **RX_CRC**: Receive and verify CRC16
* **TX_TOKEN**: Send start token
* **TX_DATA**: Read from TX FIFO, shift out on data lines
* **TX_CRC**: Compute and send CRC16
* **WAIT_RESP**: Capture data response token from card

**Burst Mode**: Automatically continues for multi-block transfers until STOP command

Register Map
------------

Base Address
~~~~~~~~~~~~

The SD controller base address is determined by the SoC memory map.

Register Summary
~~~~~~~~~~~~~~~~

===========  =================  ======  =====  =====================================
Address      Name               Access  Reset  Description
===========  =================  ======  =====  =====================================
0x00         CTRL               R/W     0x00   Control register
0x04         STATUS             R       *      Status register (read-only flags)
0x08         CMD_NUMBER         R/W     0x00   Command index (CMD0-CMD63)
0x0C         CMD_ARGUMENT       R/W     0x00   Command argument (32 bits)
0x10         EVENT              R/W     0x00   Interrupt event register
0x14         CMD_RESPONSE       R       0x00   Response data (FIFO read)
0x18         DATA_TX_BUFFER     W       0x00   Data transmit buffer (32-bit write)
0x1C         DATA_RX_BUFFER     R       0x00   Data receive buffer (32-bit read)
===========  =================  ======  =====  =====================================

*Status flags updated dynamically*

Register Descriptions
---------------------

CTRL Register (0x00)
~~~~~~~~~~~~~~~~~~~~

**SD Control Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:15]        R/W     0x00   **Interrupt Enable [8:0]**: Enable interrupts
[14]           R/W     0x0    **Send Command**: Trigger command transmission
[13]           Reserved 0x0   Reserved
[12]           R/W     0x0    **Bus Width**: 0=1-bit, 1=4-bit
[11]           R/W     0x0    **Clock Speed**: 0=400kHz, 1=25MHz
[10]           R/W     0x0    **Enable**: Enable SD controller
[9]            W       0x0    **Reset Card**: Pulse to reset card (1ms)
[8:0]          Reserved 0x00  Reserved
=============  ======  =====  =============================================

**Interrupt Enable Bits**:
  * Bit 31: Card detected
  * Bit 30: TX buffer empty
  * Bit 29: RX buffer full
  * Bit 28: Command done
  * Bit 27: Command CRC error
  * Bit 26: Command timeout
  * Bit 25: Data done
  * Bit 24: Data CRC error
  * Bit 23: Data timeout

STATUS Register (0x04, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**SD Status Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31]           R       0x0    **Card Detected**: Card present in slot
[30:23]        R       0x00   **Data Token**: Last received data response token
[22]           R       0x1    **TX Buffer Empty**: TX FIFO empty
[21]           R       0x0    **TX Buffer Full**: TX FIFO full
[20]           R       0x1    **RX Buffer Empty**: RX FIFO empty
[19]           R       0x0    **RX Buffer Full**: RX FIFO full
[18]           R       0x1    **Response Buffer Empty**: Response FIFO empty
[17]           R       0x0    **Response Buffer Full**: Response FIFO full
[16]           R       0x1    **CMD Idle**: Command controller idle
[15]           R       0x0    **CMD CRC Error**: Last command had CRC error
[14]           R       0x0    **CMD Timeout**: Last command timed out
[13]           R       0x0    **CMD Response Type**: 0=R1(48-bit), 1=R2(136-bit)
[12]           R       0x1    **Data Idle**: Data controller idle
[11]           R       0x0    **Data CRC Error**: Last data had CRC error
[10]           R       0x0    **Data Timeout**: Last data timed out
[9:0]          Reserved 0x00  Reserved
=============  ======  =====  =============================================

CMD_NUMBER Register (0x08)
~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Command Index Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:6]         -       0x00   Reserved
[5:0]          R/W     0x00   Command index (0-63)
=============  ======  =====  =============================================

**Common Commands**:
  * CMD0 (0): GO_IDLE_STATE
  * CMD8 (8): SEND_IF_COND
  * CMD17 (17): READ_SINGLE_BLOCK
  * CMD18 (18): READ_MULTIPLE_BLOCK
  * CMD24 (24): WRITE_BLOCK
  * CMD25 (25): WRITE_MULTIPLE_BLOCK
  * CMD55 (55): APP_CMD (prefix for ACMD)

CMD_ARGUMENT Register (0x0C)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Command Argument Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         R/W     0x00   32-bit command argument
=============  ======  =====  =============================================

**Argument Usage** (command-dependent):
  * CMD17/18/24/25: Block address (byte address for SDSC, block address for SDHC/SDXC)
  * CMD8: Voltage and check pattern
  * ACMD41: HCS and voltage window

EVENT Register (0x10)
~~~~~~~~~~~~~~~~~~~~~

**Interrupt Event Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:9]         -       0x00   Reserved
[8]            R/W     0x0    **Card Detected**
[7]            R/W     0x0    **TX Buffer Empty**
[6]            R/W     0x0    **RX Buffer Full**
[5]            R/W     0x0    **Command Done**
[4]            R/W     0x0    **Command CRC Error**
[3]            R/W     0x0    **Command Timeout**
[2]            R/W     0x0    **Data Done**
[1]            R/W     0x0    **Data CRC Error**
[0]            R/W     0x0    **Data Timeout**
=============  ======  =====  =============================================

**Write**: Write 0 to clear individual event flags

**Read**: Returns current event status

CMD_RESPONSE Register (0x14, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Response Data Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         R       0x00   Response data (32 bits per read, FIFO)
=============  ======  =====  =============================================

**Read Sequence**:
  * R1/R3/R6/R7 (48-bit): Read once (lower 32 bits, skip upper 16)
  * R2 (136-bit): Read four times sequentially

DATA_TX_BUFFER Register (0x18, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Data Transmit Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         W       0x00   32-bit data word for transmission
=============  ======  =====  =============================================

Write 128 times (512 bytes / 4 bytes per write) for one block.

DATA_RX_BUFFER Register (0x1C, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Data Receive Buffer**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         R       0x00   32-bit data word from reception
=============  ======  =====  =============================================

Read 128 times (512 bytes / 4 bytes per read) for one block.

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial SD controller documentation
========  ==========  ============  ===============================================
