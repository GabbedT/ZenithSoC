Ethernet MAC Controller
=======================

Overview
--------

The Ethernet MAC (Media Access Control) controller provides IEEE 802.3 compliant network connectivity with support for both Ethernet II and 802.3 frame formats. The module implements a complete MAC layer with RMII (Reduced Media Independent Interface) physical layer interface, supporting 10/100 Mbps operation with hardware CRC32 generation and verification.

The controller features dual independent TX/RX paths with dedicated FIFO buffering, descriptor-based packet management, and integrated PHY management via SMII (Serial Management Interface). The architecture supports zero-copy operation with DMA-style access patterns for efficient data transfer.

Key Features
------------

* **IEEE 802.3 Compliant**: Full MAC layer implementation
* **Dual Frame Format**: Ethernet II and 802.3 support
* **Speed**: 10 Mbps and 100 Mbps operation
* **Interface**: RMII PHY interface
* **Hardware CRC32**: Automatic generation and verification
* **Descriptor-Based**: Packet metadata management
* **Buffering**: Independent TX/RX FIFO buffers (configurable size)
* **PHY Management**: SMII interface for PHY register access
* **Interrupt System**: TX done, RX done, RX error, PHY interrupt
* **Filtering**: MAC address filtering for received packets
* **Memory-Mapped**: AXI bus interface for register access

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The Ethernet controller consists of the following main components:

* **Register Bank**: Control, status, and configuration registers
* **TX Path**: Transmit FSM, CRC32 engine, descriptor/payload buffers
* **RX Path**: Receive FSM, CRC32 engine, descriptor/payload buffers
* **PHY Interface**: RMII signals (TXD, TXEN, RXD, CRSDV, RXER, REFCLK)
* **SMII Controller**: PHY management (MDC, MDIO)
* **Clock Generator**: 50MHz reference clock and speed-dependent sampling
* **Interrupt Controller**: Edge-triggered interrupt generation

Signal Description
~~~~~~~~~~~~~~~~~~

===============================  =========  ========  ====================================
Signal Name                      Direction  Width     Description
===============================  =========  ========  ====================================
**Global**
``clk_i``                        Input      1         System clock (100MHz for 50MHz RMII)
``rst_n_i``                      Input      1         Active-low asynchronous reset
``interrupt_o``                  Output     1         Interrupt request
``busy_o``                       Output     1         SMII operation in progress
**AXI Write Interface**
``write_i``                      Input      1         Write enable
``write_address_i[5:0]``         Input      6         Register address
``write_data_i[31:0]``           Input      32        Write data
``write_done_o``                 Output     1         Write acknowledge
``write_error_o``                Output     1         Write error (invalid address)
**AXI Read Interface**
``read_i``                       Input      1         Read enable
``read_address_i[5:0]``          Input      6         Register address
``read_data_o[31:0]``            Output     32        Read data
``read_done_o``                  Output     1         Read acknowledge
``read_error_o``                 Output     1         Read error (invalid address)
**RMII Interface**
``rmii_txd_o[1:0]``              Output     2         Transmit data (2-bit DDR)
``rmii_txen_o``                  Output     1         Transmit enable
``rmii_rxd_io[1:0]``             Inout      2         Receive data (2-bit DDR)
``rmii_crsdv_io``                Inout      1         Carrier sense / RX data valid
``rmii_rxer_i``                  Input      1         Receive error
``rmii_refclk_o``                Output     1         50MHz reference clock
``rmii_rstn_o``                  Output     1         PHY reset
**SMII Interface**
``smii_mdc_o``                   Output     1         Management data clock
``smii_mdio_io``                 Inout      1         Management data I/O
``phy_interrupt_i``              Input      1         PHY interrupt input
===============================  =========  ========  ====================================

Functional Description
----------------------

Frame Format Support
~~~~~~~~~~~~~~~~~~~~

The controller supports two Ethernet frame formats:

**Ethernet II (DIX)**
  * Type field (2 bytes) indicates protocol (e.g., 0x0800 for IPv4)
  * Payload length derived from actual received bytes
  * Most common format for IP networks

**IEEE 802.3**
  * Length field (2 bytes) indicates payload size
  * Used for LLC/SNAP encapsulation
  * Length ≤ 1500 bytes

Frame Structure (both formats):

.. code-block:: text

   Preamble (7 bytes)  : 0xAA_AA_AA_AA_AA_AA_AA
   SFD (1 byte)        : 0xAB
   Dest MAC (6 bytes)  : Target MAC address
   Src MAC (6 bytes)   : Source MAC address
   Type/Length (2 bytes): Ethernet II type or 802.3 length
   Payload (46-1500)   : Packet data
   CRC32 (4 bytes)     : Frame check sequence
   IPG (12 bytes)      : Inter-packet gap

TX Path Operation
~~~~~~~~~~~~~~~~~

**Transmission Flow:**

1. Software writes packet descriptor (destination MAC + length/type)
2. Software writes payload data to TX buffer
3. Software enables TX FSM via control register
4. TX FSM reads descriptor and payload from FIFOs
5. Frames packet with preamble, SFD, MAC addresses, CRC32, IPG
6. Transmits via RMII interface at configured speed
7. Generates TX done interrupt when complete

**TX Descriptor Format (64-bit):**
  * Bits [63:16]: Destination MAC address (6 bytes)
  * Bits [15:0]: Payload length (Ethernet II) or Type (802.3)

**TX Buffer:**
  * Byte-wide FIFO
  * Configurable depth (default 512 bytes)
  * Supports multiple packets queued

RX Path Operation
~~~~~~~~~~~~~~~~~

**Reception Flow:**

1. RX FSM detects carrier sense on RMII
2. Validates preamble and SFD
3. Captures destination MAC and filters:
   - Accepts if matches device MAC address
   - Accepts if broadcast (FF:FF:FF:FF:FF:FF)
   - Drops if no match
4. Captures source MAC, length/type, payload
5. Computes and verifies CRC32
6. Writes descriptor and payload to RX FIFOs
7. Generates RX done interrupt
8. Software reads descriptor and payload

**RX Descriptor Format (64-bit):**
  * Bits [63:16]: Source MAC address (6 bytes)
  * Bits [15:0]: Payload length/type

**RX Buffer:**
  * Byte-wide FIFO
  * Configurable depth (default 512 bytes)
  * Packet descriptor queue (default 32 entries)

CRC32 Engine
~~~~~~~~~~~~

Hardware CRC32 computation for error detection:

* **Polynomial**: 0x04C11DB7 (IEEE 802.3 standard)
* **Initial Value**: 0xFFFFFFFF
* **Final XOR**: 0xFFFFFFFF
* **Bit Order**: LSB first
* **Byte Order**: Network order (big endian)

**TX**: CRC computed over destination MAC, source MAC, type/length, and payload

**RX**: CRC verified; packet dropped if mismatch

Clock Generation
~~~~~~~~~~~~~~~~

The module generates timing signals from system clock:

* **50MHz REFCLK**: RMII reference clock (system clock / 2)
* **50MHz Sampling**: For 100 Mbps operation
* **5MHz Sampling**: For 10 Mbps operation

For 100MHz system clock:
  * 50MHz = clk_counter[0] (toggle every 2 cycles)
  * 5MHz = pulse every 20 cycles

SMII PHY Management
~~~~~~~~~~~~~~~~~~~

The SMII controller provides access to PHY registers:

* **MDC Clock**: Derived from system clock
* **MDIO**: Bidirectional serial data
* **Operations**: Read and write PHY registers
* **Addressing**: 5-bit register address, 5-bit PHY address
* **Access Time**: Multiple clock cycles (serial protocol)

Software polls ``busy_o`` signal until operation completes.

Interrupt System
~~~~~~~~~~~~~~~~

Four interrupt sources (edge-triggered):

1. **TX Done**: Transmission completed
2. **RX Done**: Reception completed and stored
3. **RX Error**: CRC error or framing error
4. **PHY Interrupt**: External PHY chip interrupt

Each interrupt can be individually enabled/disabled via control register.

TX FSM Description
------------------

State Machine
~~~~~~~~~~~~~

The TX FSM implements IEEE 802.3 frame transmission:

.. code-block:: text

   IDLE → PREAMBLE → SFD → MAC_DST → MAC_SRC → TYPE → PAYLOAD → CRC → IPG → IDLE

State Definitions
~~~~~~~~~~~~~~~~~

**IDLE**
  * Wait for descriptor available and TX enable
  * Reset counters and CRC engine
  * **Transition**: data_ready & tx_enable → PREAMBLE

**PREAMBLE**
  * Transmit 7 bytes of 0xAA (alternating 1010...)
  * Assert TXEN signal
  * **Duration**: 7 bytes × 4 cycles/byte = 28 cycles (100Mbps)
  * **Transition**: byte_count == 7 → START_FRAME_DELIMITER

**START_FRAME_DELIMITER (SFD)**
  * Transmit 1 byte of 0xAB (start frame delimiter)
  * Signals end of preamble
  * **Duration**: 1 byte = 4 cycles
  * **Transition**: byte_count == 1 → MAC_DESTINATION

**MAC_DESTINATION**
  * Transmit destination MAC address (6 bytes)
  * Load from descriptor
  * Initialize CRC32 computation
  * **Duration**: 6 bytes = 24 cycles
  * **Transition**: byte_count == 6 → MAC_SOURCE

**MAC_SOURCE**
  * Transmit source MAC address (6 bytes)
  * Use device MAC_ADDRESS parameter
  * Continue CRC32 computation
  * **Duration**: 6 bytes = 24 cycles
  * **Transition**: byte_count == 6 → ETH_TYPE

**ETH_TYPE**
  * Transmit length/type field (2 bytes)
  * Load from descriptor
  * Continue CRC32 computation
  * **Duration**: 2 bytes = 8 cycles
  * **Transition**: byte_count == 2 → PAYLOAD

**PAYLOAD**
  * Transmit payload data from TX buffer
  * Read one byte per 4 cycles from FIFO
  * Continue CRC32 computation
  * Monitor payload length counter
  * **Duration**: Variable (payload_length bytes)
  * **Transition**: byte_count == payload_length → FRAME_CHECK_SEQUENCE

**FRAME_CHECK_SEQUENCE (CRC)**
  * Transmit computed CRC32 (4 bytes)
  * CRC inverted and sent LSB first
  * **Duration**: 4 bytes = 16 cycles
  * **Transition**: byte_count == 4 → INTER_PACKET_GAP

**INTER_PACKET_GAP (IPG)**
  * Transmit idle pattern (12 bytes of 0x00)
  * Deassert TXEN signal
  * Mandatory gap between packets
  * **Duration**: 12 bytes = 48 cycles
  * **Transition**: byte_count == 12 → IDLE

FSM Control Signals
~~~~~~~~~~~~~~~~~~~

* ``read_descriptor_o``: Pop descriptor from FIFO (IDLE state)
* ``read_data_o``: Pop payload byte from FIFO (PAYLOAD state)
* ``crc32_init``: Initialize CRC engine (IDLE → PREAMBLE)
* ``crc32_compute``: Compute CRC on current byte (MAC_DST through PAYLOAD)
* ``byte_increment``: Increment byte counter (every 4th bit)
* ``bit_increment``: Increment bit counter (every transmit cycle)
* ``rmii_txen_o``: PHY transmit enable (1 during PREAMBLE through CRC)
* ``rmii_txd_o``: 2-bit transmit data (LSB first)

RX FSM Description
------------------

State Machine
~~~~~~~~~~~~~

The RX FSM implements IEEE 802.3 frame reception:

.. code-block:: text

   IDLE → PREAMBLE → SDF → MAC_DST → MAC_SRC → TYPE → PAYLOAD → WAIT_IPG → WAIT_END → IDLE

State Definitions
~~~~~~~~~~~~~~~~~

**IDLE**
  * Wait for carrier sense assertion (CRSDV)
  * Reset counters and CRC engine
  * **Transition**:
    - 100Mbps: CRSDV asserted → PREAMBLE
    - 10Mbps: CRSDV asserted → SDF (skip preamble)

**PREAMBLE** (100Mbps only)
  * Receive and validate 7 bytes of 0xAA
  * Verify alternating bit pattern
  * **Duration**: 7 bytes
  * **Error Conditions**: Invalid pattern → IDLE
  * **Transition**: byte_count == 7 & valid pattern → SDF

**SDF (Start Frame Delimiter)**
  * Receive and validate 0xAB (10101011)
  * Marks start of frame
  * **Duration**: 1 byte
  * **Error Conditions**: Invalid SFD → IDLE
  * **Transition**: byte == 0xAB → MAC_DESTINATION

**MAC_DESTINATION**
  * Receive destination MAC (6 bytes)
  * Compare with device MAC_ADDRESS
  * Accept if match or broadcast (FF:FF:FF:FF:FF:FF)
  * Initialize CRC32 verification
  * **Duration**: 6 bytes
  * **Filter Action**: Drop if no match → IDLE
  * **Transition**: byte_count == 6 & address_match → MAC_SOURCE

**MAC_SOURCE**
  * Receive source MAC (6 bytes)
  * Store in descriptor
  * Continue CRC32 verification
  * **Duration**: 6 bytes
  * **Transition**: byte_count == 6 → ETH_TYPE

**ETH_TYPE**
  * Receive length/type field (2 bytes)
  * Store in descriptor
  * Continue CRC32 verification
  * **Duration**: 2 bytes
  * **Transition**: byte_count == 2 → PAYLOAD

**PAYLOAD**
  * Receive payload data
  * Write to RX buffer FIFO
  * Continue CRC32 verification
  * Monitor length field (802.3) or carrier sense (Ethernet II)
  * **Duration**: Variable (46-1500 bytes typical)
  * **Transition**:
    - 802.3: byte_count == length → verify CRC
    - Eth II: CRSDV deasserts (after CRC) → WAIT_IPG

**WAIT_IPG**
  * Wait for inter-packet gap
  * Verify received CRC32 against computed
  * Write descriptor if CRC valid
  * **Duration**: Variable (wait for CRSDV deassert)
  * **CRC Error**: Set error flag, discard packet
  * **Transition**: CRSDV deasserts → WAIT_END

**WAIT_END**
  * Final cleanup state
  * Generate RX done interrupt
  * **Duration**: 1 cycle
  * **Transition**: Unconditional → IDLE

FSM Control Signals
~~~~~~~~~~~~~~~~~~~

* ``packet_valid_o``: Write descriptor to FIFO (end of valid reception)
* ``payload_valid_o``: Write payload byte to FIFO (PAYLOAD state)
* ``packet_error_o``: CRC mismatch or framing error
* ``crc32_init``: Initialize CRC engine (IDLE → PREAMBLE/SDF)
* ``crc32_compute``: Verify CRC on current byte (MAC_DST through PAYLOAD)
* ``byte_increment``: Increment byte counter (every 4th bit)
* ``bit_increment``: Increment bit counter (every sample cycle)
* ``idle_o``: FSM in IDLE state

Error Handling
~~~~~~~~~~~~~~

**RXER Signal**: Asynchronous error from PHY → immediate return to IDLE

**CRSDV Deassert**: Loss of carrier during header → return to IDLE

**CRC Mismatch**: Packet discarded, error interrupt generated

**Address Filter Fail**: Packet dropped silently (no interrupt)

**Buffer Overflow**: New packets dropped if FIFO full

Register Map
------------

Base Address
~~~~~~~~~~~~

The Ethernet base address is determined by the SoC memory map. Default: 0x2000_6000

Address Structure
~~~~~~~~~~~~~~~~~

Bit [5]: PHY (0) / MAC (1) selection

Bits [4:0]: Register address

PHY registers (bit 5 = 0) are accessed via SMII protocol.

MAC registers (bit 5 = 1) are direct access.

MAC Register Summary
~~~~~~~~~~~~~~~~~~~~

=============  ===========  ======  =====  =====================================
Address        Name         Access  Reset  Description
=============  ===========  ======  =====  =====================================
0x00           STATUS       R/W     0x00   Status and control register
0x04           TX_DESC_L    W       0x00   TX descriptor low (dest MAC lower)
0x08           TX_DESC_H    W       0x00   TX descriptor high (dest MAC upper + length)
0x0C           TX_BUFFER    W       0x00   TX payload data (byte write)
0x10           RX_DESC_L    R       0x00   RX descriptor low (src MAC lower)
0x14           RX_DESC_H    R       0x00   RX descriptor high (src MAC upper + length)
0x18           RX_BUFFER    R       0x00   RX payload data (byte read)
0x1C           INTERRUPT    R/W     0x00   Interrupt status (write 0 to clear)
=============  ===========  ======  =====  =====================================

Register Descriptions
---------------------

STATUS Register (0x00)
~~~~~~~~~~~~~~~~~~~~~~

**Ethernet MAC Status and Control Register**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:18]        R       0x0    Reserved
[17]           R/W     0x0    **Speed**: 0=10Mbps, 1=100Mbps
[16]           R/W     0x0    **PHY Interrupt Enable**
[15]           R/W     0x0    **RX Error Interrupt Enable**
[14]           R/W     0x0    **RX Done Interrupt Enable**
[13]           R/W     0x0    **TX Done Interrupt Enable**
[12]           R/W     0x0    **Ethernet Mode**: 0=802.3, 1=Ethernet II
[11]           R/W     0x0    **TX Enable**: Enable transmit FSM
[10]           R/W     0x0    **RX Enable**: Enable receive FSM
[9]            R       0x1    **RX Idle**: RX FSM in IDLE state
[8]            R       0x1    **TX Idle**: TX FSM in IDLE state
[7]            R       0x0    **RX Payload Full**: RX buffer full
[6]            R       0x1    **RX Payload Empty**: RX buffer empty
[5]            R       0x0    **RX Packet Full**: RX descriptor full
[4]            R       0x1    **RX Packet Empty**: RX descriptor empty
[3]            R       0x0    **TX Payload Full**: TX buffer full
[2]            R       0x1    **TX Payload Empty**: TX buffer empty
[1]            R       0x0    **TX Packet Full**: TX descriptor full
[0]            R       0x1    **TX Packet Empty**: TX descriptor empty
=============  ======  =====  =============================================

TX_DESC_LOW Register (0x04, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TX Descriptor Low - Destination MAC Address [31:0]**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         W       0x0    Lower 4 bytes of destination MAC address
=============  ======  =====  =============================================

**Example**: For MAC 01:23:45:67:89:AB, write 0x67_45_23_01

TX_DESC_HIGH Register (0x08, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TX Descriptor High - Destination MAC [47:32] + Length/Type**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:16]        W       0x0    Upper 2 bytes of destination MAC address
[15:0]         W       0x0    Payload length (802.3) or Type (Eth II)
=============  ======  =====  =============================================

**Example**: For MAC 01:23:45:67:89:AB and length 64, write 0xAB_89_00_40

TX_BUFFER Register (0x0C, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**TX Payload Buffer - Byte Write**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       -      Ignored
[7:0]          W       0x0    Payload data byte
=============  ======  =====  =============================================

Write payload bytes sequentially. Each write enqueues one byte to TX FIFO.

RX_DESC_LOW Register (0x10, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**RX Descriptor Low - Source MAC Address [31:0]**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:0]         R       0x0    Lower 4 bytes of source MAC address
=============  ======  =====  =============================================

RX_DESC_HIGH Register (0x14, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**RX Descriptor High - Source MAC [47:32] + Length/Type**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:16]        R       0x0    Upper 2 bytes of source MAC address
[15:0]         R       0x0    Payload length (actual received bytes)
=============  ======  =====  =============================================

RX_BUFFER Register (0x18, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**RX Payload Buffer - Byte Read**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:8]         -       0x0    Reserved (read as 0)
[7:0]          R       0x0    Payload data byte
=============  ======  =====  =============================================

Read payload bytes sequentially. Each read dequeues one byte from RX FIFO.

INTERRUPT Register (0x1C, R/W)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Interrupt Status Register (Write 0 to clear all)**

=============  ======  =====  =============================================
Bit Field      Access  Reset  Description
=============  ======  =====  =============================================
[31:4]         -       0x0    Reserved
[3]            R       0x0    **RX Error**: CRC or framing error
[2]            R       0x0    **RX Done**: Packet received successfully
[1]            R       0x0    **TX Done**: Packet transmitted successfully
[0]            R       0x0    **PHY Interrupt**: External PHY interrupt
=============  ======  =====  =============================================

**Clear**: Write any value to clear all interrupt flags.

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial Ethernet MAC documentation
========  ==========  ============  ===============================================
