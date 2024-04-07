Ethernet 
======== 

Overview 
-------- 

ZenithSoC can enstablish an Ethernet network connection through the Ethernet module, it's composed by the following components:

* RMII Interface.
* MDIO / SMI Interface.
* Media Access Control (MAC).
* MAC to PHY Controller.
* Control Status Registers.
* Reset Logic.

The MAC is fully compliant with IEEE 802.3 Ethernet standard, supporting 10/100 Mbps speeds. 


RMII Interface Overview
-----------------------

The RMII (Reduced Media Independent Interface) is a standard for connecting Ethernet physical layer transceivers (PHYs) to Ethernet MAC (Media Access Control) devices. It is a simplified version of the standard MII (Media Independent Interface) and is designed to reduce the number of pins required to connect the PHY to the MAC, which makes it particularly useful for applications where board space is at a premium.

The MII standard typically requires 18 pins for the interface between the MAC and PHY, whereas RMII reduces this requirement to only 9 pins. This reduction is achieved by halving the data bus width from 4 bits to 2 bits and by sharing the clock signal between the transmit and receive data paths. To keep the bandwidth the clock frequency is doubled (from 25MHz to 50MHz). The RMII interface operates at a fixed speed of either 10 Mbps or 100 Mbps.


+----------+--------------------------------------------------------------------------------+
| Pin Name | Description                                                                    |
+==========+================================================================================+
| TXD0     | Transmit Data 0. First bit of data being transmitted.                          |
+----------+--------------------------------------------------------------------------------+
| TXD1     | Transmit Data 1. Second bit of data being transmitted.                         |
+----------+--------------------------------------------------------------------------------+
| TX_EN    | Transmit Enable. Signals the PHY to send data.                                 |
+----------+--------------------------------------------------------------------------------+
| RXD0     | Receive Data 0. First bit of data being received.                              |
+----------+--------------------------------------------------------------------------------+
| RXD1     | Receive Data 1. Second bit of data being received.                             |
+----------+--------------------------------------------------------------------------------+
| RX_ER    | Receive Error. Indicates reception error.                                      |
+----------+--------------------------------------------------------------------------------+
| CRS_DV   | Carrier Sense/Data Valid. Indicates reception of data.                         |
+----------+--------------------------------------------------------------------------------+
| REF_CLK  | Reference Clock. Provides the clock signal for RMII.                           |
+----------+--------------------------------------------------------------------------------+
| MDIO     | Management Data Input/Output. For PHY management.                              |
+----------+--------------------------------------------------------------------------------+
| MDC      | Management Data Clock. Clock for MDIO interface.                               |
+----------+--------------------------------------------------------------------------------+


MDIO / SMI Interface Overview
-----------------------------

The MDIO (Management Data Input/Output) or SMI (Serial Management Interface) interface is a serial, bidirectional communication interface used in Ethernet technology to manage and configure Ethernet PHY settings such as link speed, duplex mode, and to read status information such as link status and error counters.
The interface operates over two wires - MDC (Management Data Clock) and MDIO (Management Data Input/Output). The MDC is a clock signal controlled by the management entity (usually the MAC), and the MDIO line is used to transfer data serially between the management entity and the PHY.

The protocol specifies a frame format that includes preamble, start frame delimiter, op code (operation code for read or write), PHY address, register address, and data (for write operations) or turnaround time (for read operations).

A unique aspect of the MDIO interface is its ability to address multiple PHY devices on the same bus. Each PHY is assigned a unique address, allowing the management entity to communicate with each PHY individually even when multiple PHYs are connected to the same MDIO bus link in switches.


+----------+---------------------------------------------------------------------------------------------+
| Pin Name | Description                                                                                 |
+==========+=============================================================================================+
| MDIO     | Management Data Input/Output. This is the bidirectional data line for serial data transfer. |
+----------+---------------------------------------------------------------------------------------------+
| MDC      | Management Data Clock (MAX 2.5MHz). This is the clock signal for the MDIO interface.        |
+----------+---------------------------------------------------------------------------------------------+



Media Access Control
--------------------

The hardware Media Access Control (MAC) is a critical component in network interfaces that manages the data communications protocols sending and receiving data between network devices and a shared medium. Operating at the data link layer (Layer 2) of the OSI model, the hardware MAC plays a pivotal role in how data packets are accessed and provided to the physical layer managed by the on board PHY chip, ensuring that the data can be correctly transmitted over physical network.

The MAC is composed by two FSMs, one for the transmission (TX FSM) and one for the reception (RX FSM). They use the 100MHz system clock and derive a 50MHz pulse that enables the FSM to synchronize to the RMII interface operations. 

.. warning:: During the system reset, the PHY needs to be resetted through a separate reset signal that must be asserted (active low) for 200us. This is managed by the Ethernet reset logic, but it must be taken into account by the user.


Transmitter
~~~~~~~~~~~

The transmitter module is responsable of sending the data packets through the RMII interface, it's connected to two buffers: the TX data buffer and the TX packet buffer or TX packet descriptor, which provides infos and data about the packet. The transmitter is implemented as an 
FSM, the states are:

.. list-table::
   :widths: 25 50 25 25
   :header-rows: 1

   * - State
     - Description
     - Next State
     - Condition
   * - IDLE
     - The FSM is in IDLE and waiting for a new packet to transmit. Once a new packet is ready, TXEN is asserted.
     - PREAMBLE
     - Packet ready & RMII clock pulse
   * - PREAMBLE
     - Transmit 7 bytes of alternating 0 and 1 by placing on the TXD line the value 2'b01.
     - START_FRAME
     - Bytes == 7
   * - START_FRAME
     - Transmit the same bit pattern of the PREAMBLE except the last two bits that are asserted.
     - MAC_DESTINATION
     - Bytes == 1
   * - MAC_DESTINATION
     - Transmit the MAC destination address from the most significant byte.
     - MAC_SOURCE
     - Bytes == 6
   * - MAC_SOURCE
     - Transmit the MAC source address from the most significant byte.
     - ETH_TYPE
     - Bytes == 6
   * - ETH_TYPE
     - Transmit the type / length field from the most significant byte if the MAC is in Ethernet II mode, transmit the first two bytes retrieved from the data TX buffer, otherwise take the two bytes (packet length) from the packet descriptor.
     - PAYLOAD
     - Bytes == 2
   * - PAYLOAD
     - Transmit the data bytes retrieved from the data TX buffer.
     - FRAME_CHECK_SEQUENCE
     - Bytes == Payload Length
   * - FRAME_CHECK_SEQUENCE
     - Transmit the CRC bytes calculated during the preceding transmission states.
     - INTER_PACKET_GAP
     - Bytes == 4
   * - INTER_PACKET_GAP
     - Wait a time corresponding to 12 bytes.
     - IDLE
     - Bytes == 12


The FSM calculate the CRC each byte transmitted from the MAC destination address to the last data byte using the polynomial:

:math:`1 + x^1 + x^2 + x^4 + x^5 + x^7 + x^8 + x^10 + x^11 + x^12 + x^16 + x^22 + x^23 + x^26 + x^32`

Packet infos like addresses and size are transmitted from the MSByte


Receiver
~~~~~~~~

The receiver module is responsable of receiving the bits through the RMII interface from the PHY and converting them into the right Ethernet packet fields, like the transmitter FSM, the receiver is connected to two buffer (this time the write side): the RX data buffer and the RX packet buffer or RX packet descriptor.
The transmitter is implemented as an FSM, the states are:


.. list-table::
   :widths: 20 40 20 20
   :header-rows: 1

   * - State
     - Description
     - Next State
     - Condition
   * - IDLE
     - The FSM is in IDLE and waiting for the CRS_DV signal to be asserted.
     - PREAMBLE
     - CRS_DV == 1
   * - PREAMBLE
     - Check for 7 bytes of alternating 0 and 1.
     - START_FRAME
     - Bytes == 7
   * - START_FRAME
     - Check for the same bit pattern of the PREAMBLE except the last two bits that are asserted.
     - MAC_DESTINATION
     - Bytes == 1
   * - MAC_DESTINATION
     - Receive the MAC destination address (Most Significant Byte comes first).
     - MAC_SOURCE
     - Bytes == 6 & MAC_ADDRESS == Received_MAC
   * - MAC_SOURCE
     - Receive the MAC source address (Most Significant Byte comes first).
     - ETH_TYPE
     - Bytes == 6
   * - ETH_TYPE
     - Receive the type / length field (Most Significant Byte comes first). If the MAC is in Ethernet II mode, write the two bytes into the RX data buffer as if it was part of the payload, otherwise write to the packet descriptor.
     - PAYLOAD
     - Bytes == 2
   * - PAYLOAD
     - Receive the data bytes and store them into the RX data buffer. CRC is part of the data and it's the last 4 bytes received
     - FRAME_CHECK_SEQUENCE
     - Bytes == Payload Length
   * - INTER_PACKET_GAP
     - Wait a time corresponding to 12 bytes and check if the CRC received matches the CRC calculated as data was received.
     - IDLE
     - Bytes == 12

When the PHY deasserts the CRS_DV signal in states other than PAYLOAD and WAIT_IPG, the Finite State Machine (FSM) immediately transitions to the IDLE state. However, if CRS_DV is deasserted while in the PAYLOAD state, the FSM moves into the WAIT_IPG state. During this time, it continues to collect incoming bits in anticipation of CRS_DV being reasserted. If the count of these bits reaches 12 bytes without CRS_DV reassertion, the FSM then transitions to the IDLE state.

To check the CRC correctly since the CRC received is considered as part of the payload, the last 4 CRC calculated are saved into a shift register. The received CRC is then compared with the oldest computed CRC.


MAC to PHY Controller
---------------------

This module has the responsability of managing the MDIO timings to enable the MAC communicate with the PHY, as the TX and RX modules this is also implemented as an FSM:

.. list-table::
   :widths: 20 40 20 20
   :header-rows: 1

   * - State
     - Description
     - Next State
     - Condition
   * - IDLE
     - The FSM is in IDLE and waiting for the command to be received from host CPU.
     - PREAMBLE
     - Write | Read
   * - PREAMBLE
     - Hold the line for 32 MDC pulses.
     - START_OF_FRAME
     - BitCounter == 32
   * - START_OF_FRAME
     - Send the sequence of two bits '0' and '1'.
     - OPCODE
     - BitCounter == 2
   * - OPCODE
     - Depending on the operation, send the sequence '01' if it's a write, '10' if it's a read.
     - PHY_ADDRESS
     - BitCounter == 2
   * - PHY_ADDRESS
     - Send the 5 bit PHY address from the MSB to the LSB.
     - REG_ADDRESS
     - BitCounter == 5
   * - REG_ADDRESS
     - Send the 5 bit register address from the MSB to the LSB.
     - TURN_AROUND
     - BitCounter == 5
   * - TURN_AROUND
     - Put the MDIO line in high impedence if the operation is a read. If the operation is a write, send '10'. 
     - REG_ADDRESS
     - BitCounter == 2
   * - DATA
     - Put the MDIO line in high impedence if the operation is a read and save the bits as they arrive. If the operation is a write, send the data.
     - IDLE
     - BitCounter == 16

The MDC clock is easily derived from the 100MHz system clock through a mod-40 counter that divides the clock and generate a 2.5MHz clock. The sampling during reads is done during the 
falling edge of MDC to ensure maximum stability of MDIO line. A new bit is send exactly in the middle of the MDC signal is high.


Control Status Registers
------------------------

The register map is the interface between the Ethernet MAC circuitry and the CPU, the user can interact with the register through simple MMIO load / store instructions. There are 40 registers in total:

.. list-table::
   :align: center
   :widths: 20 10

   * - Register Name
     - Offset
   * - PHY Registers
     - 0 - 31
   * - Ethernet MAC Control / Status Register
     - 32
   * - TX Descriptor Low
     - 33
   * - TX Descriptor High
     - 34
   * - TX Data Buffer
     - 35
   * - RX Descriptor Low
     - 36
   * - RX Descriptor High
     - 37
   * - RX Data Buffer
     - 38
   * - Interrupt Source Register
     - 39

PHY registers
~~~~~~~~~~~~~

Those are accessed throught the MDIO interface, once the CPU issue a memory operation that has an address offset that falls between 0 and 31, the command along with the offset and data (during writes)
is passed to the MAC to PHY controller. The content of those registers is entirely dependent on the PHY used.

Ethernet MAC CSR
~~~~~~~~~~~~~~~~

The status register holds the status and the configuration of the Ethernet MAC. Here is a description of the register fields:

.. list-table:: Status Register Fields
   :widths: 10 5 20
   :header-rows: 1

   * - Name 
     - Position
     - Description
   * - RX Payload Empty
     - 0
     - Empty flag for RX Payload buffer.
   * - RX Payload Full
     - 1
     - Full flag for RX Payload buffer.
   * - TX Payload Empty
     - 2
     - Empty flag for TX Payload buffer.
   * - TX Payload Full
     - 3
     - Full flag for TX Payload buffer.
   * - RX Packet Empty
     - 4
     - Empty flag for RX Packet Descriptor buffer.
   * - RX Packet Full
     - 5
     - Full flag for RX Packet Descriptor buffer.
   * - TX Packet Empty
     - 6
     - Empty flag for TX Packet Descriptor buffer.
   * - TX Packet Full
     - 7
     - Full flag for TX Packet Descriptor buffer.
   * - RX Idle
     - 8
     - RX FSM is idle.
   * - TX Idle
     - 9
     - TX FSM is idle.
   * - RX Enable
     - 10
     - RX FSM is enabled and the MAC can receive Ethernet packets.
   * - TX Enable
     - 11
     - TX FSM is enabled and the MAC can transmit Ethernet packets.