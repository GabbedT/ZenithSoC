GPIO - General Purpose Input/Output
====================================

Overview
--------

The General-Purpose Input/Output (GPIO) module is a fundamental digital interface that connects the CPU to external devices through a simple, protocol-free interface. Each GPIO pin can be independently configured as an input or output, with additional interrupt generation capabilities for event-driven applications.

The ZenithSoC GPIO implementation provides a flexible and efficient way to interface with external hardware while minimizing interconnection congestion through thoughtful grouping and module association. This design enhances overall system performance and ensures reliable operation across a wide range of applications.

Key Features
------------

* **Bidirectional I/O**: Each pin configurable as input or output
* **Interrupt Generation**: Configurable edge-triggered interrupts in input mode
* **Interrupt Control**: Individual interrupt enable/disable per pin
* **Edge Selection**: Programmable rising or falling edge detection
* **Synchronization**: Built-in input synchronization for metastability protection
* **Tri-state Output**: High-impedance state when configured as input
* **Memory-Mapped Interface**: Simple register-based control via AXI bus

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The GPIO module consists of the following main components:

* **Configuration Registers**: Control pin direction, interrupt settings, and pin values
* **Synchronizer**: Two-stage synchronizer for external input signals
* **Tri-state Buffer**: Enables high-impedance state for input mode
* **Interrupt Logic**: Edge detection and interrupt generation
* **AXI Interface**: Memory-mapped register access

Signal Description
~~~~~~~~~~~~~~~~~~

=============================  =========  ===========  ====================================
Signal Name                    Direction  Width        Description
=============================  =========  ===========  ====================================
``clk_i``                      Input      1            System clock
``rst_n_i``                    Input      1            Active-low asynchronous reset
``pin_io``                     Inout      1            External bidirectional GPIO pin
``write_i``                    Input      1            Write enable signal
``write_address_i[1:0]``       Input      2            Register address for write operations
``write_data_i``               Input      1            Data to write to selected register
``read_address_i[1:0]``        Input      2            Register address for read operations
``read_data_o``                Output     1            Data read from selected register
``interrupt_o``                Output     1            Interrupt request output
=============================  =========  ===========  ====================================

Functional Description
----------------------

Pin Direction Control
~~~~~~~~~~~~~~~~~~~~~

Each GPIO pin can operate in two modes:

**Output Mode** (``direction = 0``)
  * Pin is actively driven by the system
  * Tri-state buffer is enabled
  * Pin value controlled by writing to Value Register
  * Pin reflects logic level set in Value Register
  * Used for: driving LEDs, controlling relays, sending digital signals

**Input Mode** (``direction = 1``)
  * Pin is in high-impedance state (tri-stated)
  * Tri-state buffer is disabled
  * Pin can be driven by external devices
  * External signal is synchronized to system clock
  * Value Register reflects external pin state
  * Used for: reading switches, sensors, receiving digital signals

Input Synchronization
~~~~~~~~~~~~~~~~~~~~~

When configured as input, external signals pass through a two-stage synchronizer to:

* Prevent metastability issues
* Synchronize asynchronous inputs to system clock
* Provide stable, glitch-free signals to internal logic

The synchronizer introduces a 2-clock cycle latency for input signals.

Interrupt Generation
~~~~~~~~~~~~~~~~~~~~

The GPIO module supports edge-triggered interrupts in input mode:

**Interrupt Conditions**
  An interrupt is generated when:

  1. The pin is configured as input (``direction = 1``)
  2. Interrupt is enabled (``interrupt_enable = 1``)
  3. The selected edge is detected on the pin

**Edge Selection**
  The ``interrupt_level`` register determines which edge triggers an interrupt:

  * ``interrupt_level = 1``: Rising edge (0 → 1 transition)
  * ``interrupt_level = 0``: Falling edge (1 → 0 transition)

**Interrupt Behavior**
  * Interrupt is edge-triggered, not level-triggered
  * Interrupt pulses high for one clock cycle when edge is detected
  * Multiple edges generate multiple interrupt pulses
  * Software must read the pin value to determine current state
  * No interrupt acknowledge required (pulse is self-clearing)

Register Map
------------

The GPIO module contains four 32-bit aligned registers, with each bit controlling a single GPIO pin. In the current implementation, only bit [0] is used per register as each module instance controls one pin.

Base Address
~~~~~~~~~~~~

The GPIO base address is determined by the SoC memory map. See the Memory Map section for specific address allocation.

Register Summary
~~~~~~~~~~~~~~~~

===========  =========  ======  ======  ===============================================
Address      Name       Access  Reset   Description
===========  =========  ======  ======  ===============================================
0x00         VALUE      R/W     0x0     Pin value register
0x04         DIR        R/W     0x0     Direction control register
0x08         INTEN      R/W     0x0     Interrupt enable register
0x0C         INTLVL     R/W     0x1     Interrupt trigger level register
===========  =========  ======  ======  ===============================================

Register Descriptions
---------------------

VALUE Register (Offset 0x00)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Pin Value Register**

===========  =========  ======  =======================================================
Bit Field    Access     Reset   Description
===========  =========  ======  =======================================================
[31:1]       Reserved   0x0     Reserved (read as 0)
[0]          R/W        0x0     Pin value

                                **Write Operation:**
                                - Output mode: Sets pin output level (0=Low, 1=High)
                                - Input mode: Write has no effect

                                **Read Operation:**
                                - Output mode: Returns current output value
                                - Input mode: Returns synchronized external pin state
===========  =========  ======  =======================================================

**Usage Notes:**

* In output mode, write to this register to drive the pin
* In input mode, read this register to sample the external signal
* Reading in output mode returns the value being driven, not the actual pin state
* Input values are synchronized (2 clock cycle latency)

DIR Register (Offset 0x04)
~~~~~~~~~~~~~~~~~~~~~~~~~~

**Direction Control Register**

===========  =========  ======  =======================================================
Bit Field    Access     Reset   Description
===========  =========  ======  =======================================================
[31:1]       Reserved   0x0     Reserved (read as 0)
[0]          R/W        0x0     Pin direction control

                                - 0: Output mode (pin is driven)
                                - 1: Input mode (pin is high-impedance)
===========  =========  ======  =======================================================

**Usage Notes:**

* Reset default is output mode (direction = 0)
* Changing direction takes effect immediately on next clock cycle
* When switching from output to input, pin goes to high-impedance
* External pull-up/pull-down resistors determine pin level when in input mode

INTEN Register (Offset 0x08)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Interrupt Enable Register**

===========  =========  ======  =======================================================
Bit Field    Access     Reset   Description
===========  =========  ======  =======================================================
[31:1]       Reserved   0x0     Reserved (read as 0)
[0]          R/W        0x0     Interrupt enable control

                                - 0: Interrupt disabled
                                - 1: Interrupt enabled (when configured edge detected)
===========  =========  ======  =======================================================

**Usage Notes:**

* Interrupt only functional when pin is in input mode
* Setting this bit to 0 immediately disables interrupt generation
* Setting this bit to 1 enables interrupt on the edge specified by INTLVL
* No effect in output mode

INTLVL Register (Offset 0x0C)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Interrupt Trigger Level Register**

===========  =========  ======  =======================================================
Bit Field    Access     Reset   Description
===========  =========  ======  =======================================================
[31:1]       Reserved   0x0     Reserved (read as 0)
[0]          R/W        0x1     Interrupt trigger edge selection

                                - 0: Falling edge (1 → 0 transition)
                                - 1: Rising edge (0 → 1 transition)
===========  =========  ======  =======================================================

**Usage Notes:**

* Reset default is rising edge detection (interrupt_level = 1)
* Only has effect when INTEN[0] = 1 and pin is in input mode
* Can be changed at any time
* Edge detection occurs on synchronized input signal

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial GPIO documentation
========  ==========  ============  ===============================================