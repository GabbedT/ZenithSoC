Timer
=====

The timer is a 64-bit counter with compare, one-shot, wrap-around, and PWM
support. It is integrated at ``0x0000_6000``; see
:doc:`../architecture/memory_map` for the complete MMIO map.

Features
--------

* 64-bit counter and 64-bit compare value;
* free-running, one-shot, and wrap-around modes;
* latched compare-match interrupt with software clear;
* two 32-bit PWM toggle values and a PWM output;
* byte-write strobes on all 32-bit registers.

Signal interface
----------------

Port directions are relative to the ``timer`` module. The register signals
are the local SoC memory-mapped interface; the top-level bus wrapper supplies
the completion signals because this module exposes no ``*_done_o`` ports.
Addresses are 3-bit word indices and ``write_strobe_i`` enables individual
bytes of ``write_data_i``.

.. list-table:: Timer module signals
   :header-rows: 1
   :widths: 32 12 14 42

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
   * - ``write_i``
     - Input
     - 1
     - Register write request.
   * - ``write_address_i[2:0]``
     - Input
     - 3
     - Word index of the register being written.
   * - ``write_data_i[31:0]``
     - Input
     - 32
     - Register write data.
   * - ``write_strobe_i[3:0]``
     - Input
     - 4
     - Byte write enables.
   * - ``write_error_o``
     - Output
     - 1
     - Write error; currently tied low by the RTL.
   * - ``read_i``
     - Input
     - 1
     - Register read request.
   * - ``read_address_i[2:0]``
     - Input
     - 3
     - Word index of the register being read.
   * - ``read_data_o[31:0]``
     - Output
     - 32
     - Register read data.
   * - ``read_error_o``
     - Output
     - 1
     - Read error; currently tied low by the RTL.
   * - ``pwm_o``
     - Output
     - 1
     - PWM output driven by the timer comparator.
   * - ``interrupt_o``
     - Output
     - 1
     - Asserted while the interrupt-pending bit is set.

Operation
---------

The counter advances when ``CONFIGURATION.ENABLE`` is set and ``STOP_TIMER``
is clear. ``TIMER_VALUE_LOW`` and ``TIMER_VALUE_HIGH`` can be written to set a
starting value. A compare match occurs when the complete 64-bit counter equals
the complete 64-bit compare register.

The ``TIMER_MODE`` field selects the counter behavior:

* ``00`` — free running; the counter continues past a match;
* ``01`` — one shot; the counter stops at a match;
* ``10`` — wrap around; the counter returns to zero at a match;
* ``11`` — reserved.

When compare interrupts are enabled, a match sets
``INTERRUPT_CONFIG.INTERRUPT_PENDING``. The pending bit remains asserted until
software writes a one to its clear bit, so an interrupt handler must clear it
after servicing the event.

PWM mode compares the counter with ``PWM_TOGGLE_LOW/HIGH`` and toggles
``pwm_o`` on a match. PWM is disabled when ``CONFIGURATION.PWM_ENABLE`` is
clear.

Register map
------------

All offsets are relative to ``0x0000_6000``.

.. list-table:: Timer registers
   :header-rows: 1
   :widths: 18 30 12 40

   * - Offset
     - Name
     - Access
     - Description
   * - ``0x00``
     - COMPARE_LOW
     - R/W
     - Compare value [31:0]
   * - ``0x04``
     - COMPARE_HIGH
     - R/W
     - Compare value [63:32]
   * - ``0x08``
     - TIMER_VALUE_LOW
     - R/W
     - Counter value [31:0]
   * - ``0x0C``
     - TIMER_VALUE_HIGH
     - R/W
     - Counter value [63:32]
   * - ``0x10``
     - PWM_TOGGLE_LOW
     - R/W
     - PWM toggle value [31:0]
   * - ``0x14``
     - PWM_TOGGLE_HIGH
     - R/W
     - PWM toggle value [63:32]
   * - ``0x18``
     - CONFIGURATION
     - R/W
     - Counter and PWM control
   * - ``0x1C``
     - INTERRUPT_CONFIG
     - R/W
     - Interrupt enable and clear

Configuration register
~~~~~~~~~~~~~~~~~~~~~~

Only the low five bits are defined:

================  ==============================================
Bits              Field
================  ==============================================
``[0]``           ENABLE
``[1]``           PWM_ENABLE
``[3:2]``         TIMER_MODE
``[4]``           STOP_TIMER (write control; also reported on reads)
================  ==============================================

The reset value stops the counter and disables PWM. The compare and PWM toggle
registers reset to all ones; the counter resets to zero.

Interrupt configuration register
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* ``[0]`` — ``INTERRUPT_ENABLE``;
* ``[1]`` — write one to clear ``INTERRUPT_PENDING``;
* remaining bits are reserved.

Programming sequence
--------------------

1. Write the low and high compare words.
2. Optionally write the PWM toggle words and set ``PWM_ENABLE``.
3. Set ``TIMER_MODE`` and ``ENABLE`` in ``CONFIGURATION``.
4. Set ``INTERRUPT_ENABLE`` in ``INTERRUPT_CONFIG`` if an interrupt is needed.
5. Clear ``INTERRUPT_PENDING`` by writing bit 1 after handling a match.
