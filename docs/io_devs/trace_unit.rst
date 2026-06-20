Trace Unit
==========

Overview
--------

The Trace Unit is a hardware module for real-time debugging and profiling of the RISC-V processor core. It monitors core execution by capturing events (exceptions, system calls, memory operations) and branch divergence (jumps, branches, calls), generating trace packets transmitted via UART for offline analysis.

The module operates non-intrusively with minimal overhead on core execution, supporting 24-bit timestamps, PC-based triggers, and configurable buffering to handle event bursts.

Features
~~~~~~~~

* CPU event tracing (exceptions, interrupts, system calls, memory operations)
* Branch divergence tracing (non-sequential control flow)
* 24-bit timestamp for temporal correlation
* Program Counter trigger for selective tracing start
* Configurable FIFO buffer (default 512 packets)
* Automatic serialization to UART
* Core halting on buffer full to prevent data loss
* Interrupt on buffer full/empty

Architecture
------------

The Trace Unit consists of three main modules:

* **Registers**: Manages configuration, control and status registers
* **Packetizer**: Generates trace packets from CPU events and divergence detection
* **Serializer**: Serializes packets into byte stream to UART

Signal Interface
~~~~~~~~~~~~~~~~

**Global Signals:**

* ``clk_i``: System clock
* ``rst_n_i``: Active-low reset

**CPU Interface:**

* ``interrupt_o``: Interrupt to CPU
* ``halt_core_o``: Halt signal when buffer is full
* ``trace_interface_i``: Trace interface from RISC-V core

**UART Interface:**

* ``uart_tx_full_i``: UART TX buffer full status
* ``trace_chunk_o[7:0]``: Byte to transmit
* ``write_chunk_o``: Write strobe

**Register Interface:**

* ``write_i``, ``write_address_i[1:0]``, ``write_data_i[31:0]``, ``write_strobe_i[3:0]``
* ``read_i``, ``read_address_i[1:0]``, ``read_data_o[31:0]``
* ``write_error_o``, ``read_error_o``, ``write_done_o``, ``read_done_o``

Operation
---------

Packet Types
~~~~~~~~~~~~

The Trace Unit generates three packet types:

**Event Packet (2 or 5 bytes):**

* SYNC (1 byte): 0xAA
* TYPE + EVENT_NUMBER (1 byte): EVENT_PACKET type + event number (6 bits)
* TIMESTAMP (3 bytes, optional): timestamp if enabled

**Divergence Packet (10 or 13 bytes):**

* SYNC (1 byte): 0xAA
* TYPE (1 byte): DIVERGENCE_PACKET type
* START_PC (4 bytes): PC before branch
* END_PC (4 bytes): PC after branch
* TIMESTAMP (3 bytes, optional): timestamp if enabled

**Overflow Packet (2 bytes):**

* SYNC (1 byte): 0xAA
* TYPE (1 byte): OVERFLOW_PACKET type (24-bit timestamp overflow)

Event Tracing
~~~~~~~~~~~~~

When enabled, the Trace Unit captures configurable events from the core:

* **Instruction faults**: Misaligned, access fault, illegal instruction
* **Breakpoint**: Debug breakpoint
* **Load/Store operations**: Memory operations and related faults
* **System calls**: User-mode and machine-mode ecall
* **Handler return**: MRET instruction
* **Sleep**: WFI instruction
* **Branch/Jump**: Control flow operations
* **CSR operations**: Control Status Register accesses

Branch Tracing
~~~~~~~~~~~~~~

The module compares the current PC with the previous one. If different from PC+4 or PC+2 (compressed), it generates a Divergence Packet containing start PC and end PC to reconstruct the control flow.

Divergence detection activates after the first valid PC (or after trigger PC if enabled) to avoid initial false positives.

Timestamp
~~~~~~~~~

The 24-bit counter increments every cycle when timestamping is enabled. Upon reaching 0xFFFFFF, it generates an Overflow Packet and continues from 0.

Trigger PC
~~~~~~~~~~

When enabled, branch tracing starts only when the core reaches the specified PC. Useful for tracing specific code sections.

Core Halting
~~~~~~~~~~~~

When the buffer is full, the ``halt_core_o`` signal is asserted to stop the core and prevent packet loss. The core resumes when buffer space becomes available.

Serializer FSM
--------------

The serializer manages packet transmission to UART through a 3-state FSM:

**IDLE**

* Waits for packets in buffer and UART not full
* Reads packet from buffer
* Transition to SYNC

**SYNC**

* Transmits SYNC byte (0xAA)
* Extracts packet type from packet
* Transition to DATA

**DATA**

* Transmits payload bytes sequentially
* Byte count depends on packet type and timestamp enable:

  * OVERFLOW: 1 byte
  * EVENT: 1 byte (no timestamp) or 4 bytes (with timestamp)
  * DIVERGENCE: 9 bytes (no timestamp) or 12 bytes (with timestamp)

* Returns to IDLE after last byte

Register Map
------------

+--------+----------------+------+-----------------------------------------------+
| Offset | Register       | R/W  | Description                                   |
+========+================+======+===============================================+
| 0x00   | STATUS         | R    | Status register (buffer full/empty)           |
+--------+----------------+------+-----------------------------------------------+
| 0x04   | CONTROL        | R/W  | Control register (enable tracing, interrupts) |
+--------+----------------+------+-----------------------------------------------+
| 0x08   | EVENT_ENABLE   | R/W  | Event enable mask                             |
+--------+----------------+------+-----------------------------------------------+
| 0x0C   | PC_TRIGGER     | R/W  | Trigger PC value                              |
+--------+----------------+------+-----------------------------------------------+

Register Descriptions
~~~~~~~~~~~~~~~~~~~~~

**STATUS (Offset: 0x00)**

+--------+-------------+-----+-----------------------------------------------+
| Bit    | Field       | R/W | Description                                   |
+========+=============+=====+===============================================+
| 0      | BUFFER_EMPTY| R   | Buffer empty (1 = empty, 0 = data present)    |
+--------+-------------+-----+-----------------------------------------------+
| 1      | BUFFER_FULL | R   | Buffer full (1 = full, 0 = space available)   |
+--------+-------------+-----+-----------------------------------------------+
| 31:2   | Reserved    | R   | Reserved (read as 0)                          |
+--------+-------------+-----+-----------------------------------------------+

*Reset Value:* 0x00000001 (buffer empty)

**CONTROL (Offset: 0x04)**

+--------+------------------------+-----+-----------------------------------------------+
| Bit    | Field                  | R/W | Description                                   |
+========+========================+=====+===============================================+
| 0      | ENABLE_EVENT_TRACING   | R/W | Enable event tracing                          |
+--------+------------------------+-----+-----------------------------------------------+
| 1      | ENABLE_BRANCH_TRACING  | R/W | Enable branch divergence tracing              |
+--------+------------------------+-----+-----------------------------------------------+
| 2      | ENABLE_TIMESTAMP_EVENT | R/W | Include timestamp in event packets            |
+--------+------------------------+-----+-----------------------------------------------+
| 3      | ENABLE_TIMESTAMP_BRANCH| R/W | Include timestamp in divergence packets       |
+--------+------------------------+-----+-----------------------------------------------+
| 4      | ENABLE_TRIGGER         | R/W | Enable PC trigger for branch tracing          |
+--------+------------------------+-----+-----------------------------------------------+
| 5      | CLEAR_TIMESTAMP        | W   | Clear timestamp counter (self-clearing)       |
+--------+------------------------+-----+-----------------------------------------------+
| 6      | ENABLE_INT_FULL        | R/W | Enable interrupt on buffer full               |
+--------+------------------------+-----+-----------------------------------------------+
| 7      | ENABLE_INT_EMPTY       | R/W | Enable interrupt on buffer empty              |
+--------+------------------------+-----+-----------------------------------------------+
| 31:8   | Reserved               | R   | Reserved (read as 0)                          |
+--------+------------------------+-----+-----------------------------------------------+

*Reset Value:* 0x00000000

**EVENT_ENABLE (Offset: 0x08)**

18-bit mask to enable specific events. Each bit corresponds to an event:

+--------+-----------------------+-----+---------------+-----------------------------------------------+
| Bit    | Event                 | R/W | Event Number  | Description                                   |
+========+=======================+=====+===============+===============================================+
| 0      | INSTR_MISALIGNED      | R/W | 0             | Instruction address misaligned                |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 1      | INSTR_ACCESS_FAULT    | R/W | 1             | Instruction access fault                      |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 2      | INSTR_ILLEGAL         | R/W | 2             | Illegal instruction                           |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 3      | BREAKPOINT            | R/W | 3             | Breakpoint exception                          |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 4      | LOAD_OPERATION        | R/W | 19            | Load operation executed                       |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 5      | LOAD_MISALIGNED       | R/W | 4             | Load address misaligned                       |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 6      | LOAD_ACCESS_FAULT     | R/W | 5             | Load access fault                             |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 7      | STORE_OPERATION       | R/W | 18            | Store operation executed                      |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 8      | STORE_MISALIGNED      | R/W | 6             | Store address misaligned                      |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 9      | STORE_ACCESS_FAULT    | R/W | 7             | Store access fault                            |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 10     | U_SYSTEM_CALL         | R/W | 8             | User-mode system call (ecall)                 |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 11     | M_SYSTEM_CALL         | R/W | 11            | Machine-mode system call (ecall)              |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 12     | HANDLER_RETURN        | R/W | 17            | Interrupt handler return (mret)               |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 13     | SLEEP                 | R/W | 16            | Core sleep (WFI)                              |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 14     | BRANCH                | R/W | 20            | Branch instruction executed                   |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 15     | JUMP                  | R/W | 21            | Jump instruction executed                     |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 16     | CSR                   | R/W | 22            | CSR operation                                 |
+--------+-----------------------+-----+---------------+-----------------------------------------------+
| 31:17  | Reserved              | R   | -             | Reserved (read as 0)                          |
+--------+-----------------------+-----+---------------+-----------------------------------------------+

*Reset Value:* 0x00000000

**PC_TRIGGER (Offset: 0x0C)**

+--------+-------+-----+-----------------------------------------------+
| Bit    | Field | R/W | Description                                   |
+========+=======+=====+===============================================+
| 31:0   | VALUE | R/W | Program Counter trigger value                 |
+--------+-------+-----+-----------------------------------------------+

*Reset Value:* 0x00000000

Timing Characteristics
----------------------

* **Timestamp resolution**: 1 clock cycle
* **Timestamp range**: 0 to 16,777,215 cycles (24-bit)
* **Packet generation latency**: 1 clock cycle
* **Serialization rate**: Limited by UART TX speed
* **Buffer depth**: Configurable (default 512 packets)

For a 50 MHz clock:

* **Timestamp resolution**: 20 ns
* **Timestamp overflow**: ~335 ms
* **Max sustained event rate**: Depends on UART baud rate

Integration Guidelines
----------------------

UART Connection
~~~~~~~~~~~~~~~

The Trace Unit connects directly to the UART TX buffer. The ``uart_tx_full_i`` signal handles backpressure when UART cannot accept data.

Core Halting
~~~~~~~~~~~~

When the buffer is full, ``halt_core_o`` stops the core to prevent packet loss. Ensure the core supports halting via this signal.

Buffer Sizing
~~~~~~~~~~~~~

Buffer size must be chosen based on:

* Expected event rate
* UART baud rate
* Core halting tolerance

A larger buffer reduces core halt probability but increases latency and area.

Packet Parsing
~~~~~~~~~~~~~~

Packets transmitted via UART must be parsed offline:

1. Identify SYNC byte (0xAA)
2. Read packet type from 2 MSB bits of next byte
3. Extract payload based on packet type and timestamp enable
4. Reconstruct control flow from divergence packets
5. Correlate events via timestamp

Limitations
-----------

* 24-bit timestamp (overflow after ~335 ms at 50 MHz)
* Fixed buffer size at synthesis time
* No packet compression
* Single trace interface (only one core supported)
* Requires dedicated UART for trace output
* Core halting on buffer full may impact real-time timing
