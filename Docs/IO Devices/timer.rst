Timer
=====

Overview
--------

The Timer is a 64-bit hardware timer module designed for precise time measurement and periodic interrupt generation. It supports both continuous counting and one-shot operation modes, making it suitable for system tick generation, timeout detection, and time-based event triggering.

The timer operates as a free-running 64-bit counter with a programmable compare value. When the counter reaches the compare threshold, an interrupt can be generated, and optionally, the timer can stop automatically in one-shot mode.

Features
~~~~~~~~

* 64-bit counter with full read/write access
* 64-bit programmable compare value
* Continuous or one-shot operation modes
* Optional interrupt generation on compare match
* Software-controlled enable/disable
* Zero-latency counter access
* Memory-mapped register interface

Architecture
------------

The timer consists of the following components:

* **64-bit Counter**: Free-running counter that increments every clock cycle when enabled
* **64-bit Compare Register**: Programmable threshold for interrupt generation
* **Configuration Logic**: Controls timer behavior (enable, one-shot mode, interrupt enable)
* **Edge Detector**: Detects when counter matches compare value
* **Register Interface**: Handles read/write operations to timer registers

Signal Interface
~~~~~~~~~~~~~~~~

**Global Signals:**

* ``clk_i``: System clock input
* ``rst_n_i``: Active-low asynchronous reset

**Write Interface:**

* ``write_i``: Write enable signal
* ``write_data_i[31:0]``: 32-bit write data
* ``write_address_i[2:0]``: Write address (selects register)
* ``write_strobe_i[3:0]``: Byte-level write enable
* ``write_error_o``: Write error flag (address out of range)

**Read Interface:**

* ``read_i``: Read enable signal
* ``read_address_i[2:0]``: Read address (selects register)
* ``read_data_o[31:0]``: 32-bit read data
* ``read_error_o``: Read error flag (address out of range)

**Status Output:**

* ``interrupt_o``: Interrupt output signal

Operation Modes
---------------

Continuous Mode
~~~~~~~~~~~~~~~

In continuous mode, the timer counts continuously from its current value. When the counter reaches the compare value:

1. An interrupt is generated (if interrupt_enable is set)
2. The counter continues counting beyond the compare value
3. The interrupt remains active until the counter value changes (typically on next clock cycle)
4. The timer wraps around at 2^64-1 and continues from 0

This mode is ideal for periodic interrupts and system tick generation.

One-Shot Mode
~~~~~~~~~~~~~

In one-shot mode (``one_shot`` bit set), the timer behaves differently:

1. Timer counts until it reaches the compare value
2. An interrupt is generated (if interrupt_enable is set)
3. The timer automatically stops counting
4. To restart, software must either:

   * Write a new value to the timer counter registers, or
   * Write to the CONFIGURATION register with bit 3 set to restart

This mode is useful for timeout detection and single event timing.

Timer Operation
---------------

Counter Behavior
~~~~~~~~~~~~~~~~

The 64-bit counter increments every clock cycle when:

* The timer is enabled (``configuration.enable`` = 1)
* The timer is not stopped (``stop_timer`` = 0)
* No interrupt condition is active in one-shot mode

The counter can be:

* Read at any time without affecting its operation
* Written to set a specific start value
* Reset to 0 on system reset

Compare Match Detection
~~~~~~~~~~~~~~~~~~~~~~~

The timer continuously compares the 64-bit counter value with the 64-bit compare value. When they match:

1. An edge detector triggers a single-cycle interrupt pulse
2. If ``interrupt_enable`` is set, the interrupt is propagated to ``interrupt_o``
3. If ``one_shot`` is set, the timer stops automatically

Interrupt Generation
~~~~~~~~~~~~~~~~~~~~

Interrupts are generated when:

.. code-block:: text

   interrupt_o = (timer == timer_compare) AND interrupt_enable

The interrupt is a single clock cycle pulse due to the edge detector implementation. This ensures the interrupt is triggered exactly once per compare match, even if the timer remains at the compare value (in stopped state).

Register Map
------------

The timer has 5 32-bit registers spanning a 64-bit counter and 64-bit compare value:

+--------+-------------------+------+-------------------------------------------------------+
| Offset | Register          | R/W  | Description                                           |
+========+===================+======+=======================================================+
| 0x00   | COMPARE_LOW       | R/W  | Compare value bits [31:0]                             |
+--------+-------------------+------+-------------------------------------------------------+
| 0x04   | COMPARE_HIGH      | R/W  | Compare value bits [63:32]                            |
+--------+-------------------+------+-------------------------------------------------------+
| 0x08   | TIMER_VALUE_LOW   | R/W  | Timer counter bits [31:0]                             |
+--------+-------------------+------+-------------------------------------------------------+
| 0x0C   | TIMER_VALUE_HIGH  | R/W  | Timer counter bits [63:32]                            |
+--------+-------------------+------+-------------------------------------------------------+
| 0x10   | CONFIGURATION     | R/W  | Configuration and control register                    |
+--------+-------------------+------+-------------------------------------------------------+

Register Descriptions
~~~~~~~~~~~~~~~~~~~~~

**COMPARE_LOW (Offset: 0x00)**

Lower 32 bits of the 64-bit compare value.

+--------+-------+------+---------------------------------------+
| Bit    | Field | R/W  | Description                           |
+========+=======+======+=======================================+
| 31:0   | VALUE | R/W  | Compare value bits [31:0]             |
+--------+-------+------+---------------------------------------+

*Reset Value:* 0xFFFFFFFF

**COMPARE_HIGH (Offset: 0x04)**

Upper 32 bits of the 64-bit compare value.

+--------+-------+------+---------------------------------------+
| Bit    | Field | R/W  | Description                           |
+========+=======+======+=======================================+
| 31:0   | VALUE | R/W  | Compare value bits [63:32]            |
+--------+-------+------+---------------------------------------+

*Reset Value:* 0xFFFFFFFF

**TIMER_VALUE_LOW (Offset: 0x08)**

Lower 32 bits of the 64-bit timer counter. Can be written to set the timer to a specific value.

+--------+-------+------+---------------------------------------+
| Bit    | Field | R/W  | Description                           |
+========+=======+======+=======================================+
| 31:0   | VALUE | R/W  | Timer counter bits [31:0]             |
+--------+-------+------+---------------------------------------+

*Reset Value:* 0x00000000

**TIMER_VALUE_HIGH (Offset: 0x0C)**

Upper 32 bits of the 64-bit timer counter. Can be written to set the timer to a specific value.

+--------+-------+------+---------------------------------------+
| Bit    | Field | R/W  | Description                           |
+========+=======+======+=======================================+
| 31:0   | VALUE | R/W  | Timer counter bits [63:32]            |
+--------+-------+------+---------------------------------------+

*Reset Value:* 0x00000000

**CONFIGURATION (Offset: 0x10)**

Configuration and control register.

+--------+------------------+------+---------------------------------------+
| Bit    | Field            | R/W  | Description                           |
+========+==================+======+=======================================+
| 0      | ENABLE           | R/W  | Enable timer counting                 |
|        |                  |      | 0 = Timer disabled                    |
|        |                  |      | 1 = Timer enabled                     |
+--------+------------------+------+---------------------------------------+
| 1      | ONE_SHOT         | R/W  | One-shot mode enable                  |
|        |                  |      | 0 = Continuous mode                   |
|        |                  |      | 1 = One-shot mode (stop on compare)  |
+--------+------------------+------+---------------------------------------+
| 2      | INTERRUPT_ENABLE | R/W  | Interrupt generation enable           |
|        |                  |      | 0 = No interrupt generated            |
|        |                  |      | 1 = Generate interrupt on compare     |
+--------+------------------+------+---------------------------------------+
| 3      | STOP_TIMER       | R    | Timer stopped status (read-only)      |
|        |                  |      | 0 = Timer running                     |
|        |                  |      | 1 = Timer stopped                     |
|        |                  | W    | Restart control (write-only)          |
|        |                  |      | Write 0 to restart timer              |
|        |                  |      | Write 1 to keep stopped               |
+--------+------------------+------+---------------------------------------+
| 31:4   | Reserved         | R    | Reserved (read as 0)                  |
+--------+------------------+------+---------------------------------------+

*Reset Value:* 0x00000008 (timer stopped, all features disabled)


Timing Characteristics
----------------------

* **Resolution**: 1 system clock cycle
* **Maximum count**: 2^64 - 1 cycles
* **Interrupt latency**: 1 clock cycle from compare match to interrupt output
* **Register access**: Single-cycle read/write (no wait states)
* **Counter update**: Increments every clock cycle when enabled

Integration Guidelines
----------------------

The timer operates in a single clock domain (``clk_i``). All inputs and outputs are synchronous to this clock.

Reset Behavior
~~~~~~~~~~~~~~

On reset (``rst_n_i`` = 0):

* Timer counter is cleared to 0
* Compare value is set to 0xFFFFFFFFFFFFFFFF (maximum)
* Timer is disabled and stopped
* All configuration bits are cleared

Interrupt Handling
~~~~~~~~~~~~~~~~~~

The ``interrupt_o`` signal is a single clock cycle pulse. The interrupt controller should:

* Latch the interrupt signal
* Clear the latch after the interrupt is serviced
* In one-shot mode, restart the timer if needed

In continuous mode, the interrupt naturally clears on the next clock cycle as the counter increments past the compare value.

64-bit Access Considerations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When reading or writing 64-bit values split across two 32-bit registers:

* Always read HIGH register first, then LOW register (to detect overflow)
* Write LOW register first, then HIGH register (to maintain atomicity)
* Disable the timer during 64-bit writes if timing is critical