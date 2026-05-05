AXI Bus
=======

Overview
--------

The AXI Bus implements a simplified AXI4-Lite interconnect for connecting the RISC-V core to memory-mapped peripherals. The system provides a single master interface with configurable multiple slave devices through address-based routing.

The implementation follows ARM AMBA AXI4-Lite protocol with separated read and write channels, full handshaking, and error response propagation.

Features
~~~~~~~~

* AXI4-Lite compliant protocol
* Single master, multiple slave architecture
* Configurable slave address ranges
* Separate read and write channels
* Full handshaking with VALID/READY signals
* Error response support (OKAY, SLVERR, DECERR)
* Shared signal bus for reduced routing
* Flow control with Clear-to-Send (CTS) mechanism
* Address decode error detection

Architecture
------------

The AXI Bus consists of three main components:

* **AXI Master**: Initiates read/write transactions from CPU
* **AXI Router**: Routes transactions to appropriate slaves based on address
* **AXI Slave**: Interfaces peripherals to the bus

Signal Interface
~~~~~~~~~~~~~~~~

**AXI4-Lite Channels:**

The bus implements five independent channels:

* **Write Address Channel (AW)**: Address and control for write transactions
* **Write Data Channel (W)**: Data for write transactions
* **Write Response Channel (B)**: Write transaction completion
* **Read Address Channel (AR)**: Address and control for read transactions
* **Read Data Channel (R)**: Read data and response

**Global Signals:**

* ``axi_ACLK``: System clock
* ``axi_ARESETN``: Active-low reset

**Master Interface Signals:**

Write:

* ``write_start_i``: Initiate write transaction
* ``write_address_i[31:0]``: Write address
* ``write_data_i[31:0]``: Write data
* ``write_strobe_i[3:0]``: Byte enable
* ``write_done_o``: Write transaction completed
* ``write_cts_o``: Clear to send (master can issue new transaction)

Read:

* ``read_start_i``: Initiate read transaction
* ``read_invalid_i``: Invalidate pending read
* ``read_address_i[31:0]``: Read address
* ``read_data_o[31:0]``: Read data
* ``read_done_o``: Read transaction completed
* ``read_cts_o``: Clear to send (master can issue new transaction)

**Slave Interface Signals:**

Write:

* ``write_request_o``: Write request to peripheral
* ``write_address_o[31:0]``: Write address
* ``write_data_o[31:0]``: Write data
* ``write_strobe_o[3:0]``: Byte enable
* ``write_done_i``: Peripheral write completed
* ``write_error_i``: Write error from peripheral
* ``write_ready_i``: Peripheral ready for write
* ``write_busy_i``: Peripheral busy

Read:

* ``read_request_o``: Read request to peripheral
* ``read_address_o[31:0]``: Read address
* ``read_data_i[31:0]``: Read data from peripheral
* ``read_done_i``: Peripheral read completed
* ``read_error_i``: Read error from peripheral
* ``read_ready_i``: Peripheral ready for read
* ``read_busy_i``: Peripheral busy

Operation
---------

Write Transaction
~~~~~~~~~~~~~~~~~

**Transaction Flow:**

1. Master asserts ``write_start_i`` with address, data, and strobe
2. Master interface loads data onto AW and W channels
3. Master sets ``AWVALID`` and ``WVALID``
4. Router decodes address and selects target slave
5. Slave asserts ``AWREADY`` and ``WREADY`` when ready
6. Handshake completes on both channels
7. Slave processes write internally
8. Slave asserts ``BVALID`` with response when done
9. Master reads response and asserts ``BREADY``
10. Master signals ``write_done_o`` to CPU

**Write Strobe:**

The 4-bit ``write_strobe`` enables byte-level writes:

* Bit 0: Byte [7:0]
* Bit 1: Byte [15:8]
* Bit 2: Byte [23:16]
* Bit 3: Byte [31:24]

**Handshaking:**

A transfer occurs when both VALID and READY are high:

* Master controls VALID (data available)
* Slave controls READY (can accept data)
* Once VALID is asserted, it remains high until READY is asserted

Read Transaction
~~~~~~~~~~~~~~~~

**Transaction Flow:**

1. Master asserts ``read_start_i`` with address
2. Master interface loads address onto AR channel
3. Master sets ``ARVALID``
4. Router decodes address and selects target slave
5. Slave asserts ``ARREADY`` when ready
6. Address handshake completes
7. Slave processes read internally
8. Slave places data on R channel and asserts ``RVALID``
9. Master asserts ``RREADY`` (always ready in this design)
10. Data handshake completes
11. Master signals ``read_done_o`` with data to CPU

**Read Invalidation:**

The ``read_invalid_i`` signal allows the CPU to cancel pending reads. When asserted, the master interface deasserts ``ARVALID``, aborting the transaction.

Router Operation
~~~~~~~~~~~~~~~~

The router performs address-based arbitration:

**Address Matching:**

Each slave has a configurable address range [LOW_SLAVE_ADDRESS, HIGH_SLAVE_ADDRESS]. The router compares the master's address against all slave ranges and selects the matching slave.

**Routing Logic:**

* Only one slave can match at a time (non-overlapping ranges enforced)
* If no slave matches, a decode error (DECERR) is generated
* The router maintains connection until transaction completes

**Shared Signals:**

To reduce routing, address and data signals are broadcast to all slaves. Only the selected slave interprets them as valid based on handshaking signals.

**Bus Busy Handling:**

When a slave is busy:

* ``master_cts`` is deasserted if master tries to switch slaves
* ``master_cts`` remains asserted if targeting the same busy slave
* Other slaves see ``bus_taken`` signal to ignore transactions

Flow Control
~~~~~~~~~~~~

**Clear-to-Send (CTS):**

The master interface provides CTS signals to the CPU:

* ``write_cts_o``: High when master can accept new write
* ``read_cts_o``: High when master can accept new read

CTS is low when:

* Master is switching between slaves while one is busy
* Router is still processing previous transaction

**Backpressure:**

Slaves can apply backpressure by:

* Deasserting READY signals to stall handshake
* Asserting busy signals during multi-cycle operations

AXI Response Codes
------------------

+----------+-------+--------------------------------------------------------------+
| Response | Value | Description                                                  |
+==========+=======+==============================================================+
| OKAY     | 0b00  | Normal access success                                        |
+----------+-------+--------------------------------------------------------------+
| EXOKAY   | 0b01  | Exclusive access success (not used in this implementation)   |
+----------+-------+--------------------------------------------------------------+
| SLVERR   | 0b10  | Slave error - peripheral detected error during transaction   |
+----------+-------+--------------------------------------------------------------+
| DECERR   | 0b11  | Decode error - no slave at target address                    |
+----------+-------+--------------------------------------------------------------+

**Error Propagation:**

* Slave errors (``write_error_i``, ``read_error_i``) map to SLVERR
* Address decode failures map to DECERR
* Errors are signaled via ``axi_write_error_o`` and ``axi_read_error_o``

**Error Handling:**

On SLVERR response, the master flushes pending transactions:

* Deasserts VALID signals
* Ignores subsequent ``start`` requests until error is cleared
* CPU must handle error and restart transaction

Configuration
-------------

Network Parameters
~~~~~~~~~~~~~~~~~~

.. code-block:: systemverilog

   axi_network #(
       .AXI_SLAVE_NUMBER ( 4 ),
       .LOW_SLAVE_ADDRESS ( '{32'h4000_0000, 32'h4000_1000,
                               32'h4000_2000, 32'h4000_3000} ),
       .HIGH_SLAVE_ADDRESS ( '{32'h4000_0FFF, 32'h4000_1FFF,
                                32'h4000_2FFF, 32'h4000_3FFF} )
   ) axi_bus (
       // connections
   );

**Parameters:**

* ``AXI_SLAVE_NUMBER``: Number of slave devices
* ``LOW_SLAVE_ADDRESS[N]``: Array of low addresses for each slave
* ``HIGH_SLAVE_ADDRESS[N]``: Array of high addresses for each slave

**Address Range Rules:**

* Ranges must not overlap
* LOW_SLAVE_ADDRESS ≤ HIGH_SLAVE_ADDRESS for each slave
* Addresses are inclusive on both ends
* Gaps between ranges are allowed (decode error if accessed)

Slave Integration
~~~~~~~~~~~~~~~~~

To connect a peripheral as an AXI slave:

1. Instantiate ``axi_slave`` with address range
2. Connect AXI channels
3. Implement peripheral logic responding to:

   * ``write_request_o``: Handle write transaction
   * ``read_request_o``: Handle read transaction
   * ``write_ready_i``: Assert when ready for writes
   * ``read_ready_i``: Assert when ready for reads
   * ``write_done_i``: Assert when write completes
   * ``read_done_i``: Assert when read completes
   * ``write_error_i``: Assert on write errors
   * ``read_error_i``: Assert on read errors


Integration Guidelines
----------------------

Address Map Planning
~~~~~~~~~~~~~~~~~~~~

Organize peripherals with aligned boundaries:

.. code-block:: text

   0x4000_0000 - 0x4000_0FFF : GPIO (4KB)
   0x4000_1000 - 0x4000_1FFF : UART (4KB)
   0x4000_2000 - 0x4000_2FFF : SPI (4KB)
   0x4000_3000 - 0x4000_3FFF : Timer (4KB)
   0x4000_4000 - ... : etc.

Use power-of-2 sizes for easier decoding and future expansion.

Slave Design Rules
~~~~~~~~~~~~~~~~~~

1. **Always respond**: Never leave a transaction hanging
2. **Assert done signals**: Required for handshake completion
3. **Manage ready signals**: Deassert when not ready for new transactions
4. **Use busy correctly**: Assert during multi-cycle operations
5. **Error reporting**: Use error signals for invalid accesses
6. **Reset behavior**: Return to idle state on ``axi_ARESETN``

Reset Behavior
~~~~~~~~~~~~~~

On reset (``axi_ARESETN`` = 0):

* All VALID signals deasserted
* All READY signals asserted (slaves ready)
* All response channels idle
* Router state cleared
* No ongoing transactions

Bus Arbitration
~~~~~~~~~~~~~~~

Current implementation: Single master only

For multi-master designs, add arbitration layer before router to:

* Grant bus access to one master at a time
* Queue pending master requests
* Implement priority or round-robin scheme

Limitations
-----------

* Single master only (no arbitration)
* No burst transfers (single beat transactions only)
* No exclusive access support
* No locked transfers
* No Quality of Service (QoS) support
* No pipelining (single outstanding transaction per channel)
* 32-bit address and data bus only
* AXI4-Lite subset (no full AXI4 features)
