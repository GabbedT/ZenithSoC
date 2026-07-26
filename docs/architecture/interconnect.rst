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

Configuration
-------------

Network Parameters
~~~~~~~~~~~~~~~~~~

.. code-block:: systemverilog

   axi_network #(
       .AXI_SLAVE_NUMBER   ( 4                  ),
       .LOW_SLAVE_ADDRESS  ( LOW_SLAVE_ADDRESS  ),
       .HIGH_SLAVE_ADDRESS ( HIGH_SLAVE_ADDRESS )
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


Integration Guidelines
----------------------

Address Map Planning
~~~~~~~~~~~~~~~~~~~~

Organize peripherals with aligned boundaries:

.. code-block:: text

   UART       : 0x0000_4000 window
   Timer      : 0x0000_6000 window
   GPIO       : 0x0000_8000 window
   SPI        : 0x0000_A000 window
   Ethernet   : 0x0000_C000 window
   PRNG/APU   : 0x0000_E000 / 0x0001_0000 windows
   SD/Trace   : 0x0001_4400 / 0x0001_6400 windows

See :doc:`memory_map` for the complete map. The addresses are supplied by
``soc_parameters``; the router does not use the old ``0x4000_0000`` example
map shown in earlier versions of this page.

Use power-of-2 sizes for easier decoding and future expansion.

Slave Design Rules
~~~~~~~~~~~~~~~~~~

1. **Always respond**: Never leave a transaction hanging
2. **Assert done signals**: Required for handshake completion
3. **Manage ready signals**: Deassert when not ready for new transactions
4. **Use busy correctly**: Assert during multi-cycle operations
5. **Error reporting**: Use error signals for invalid accesses
6. **Reset behavior**: Return to idle state on ``axi_ARESETN``


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
