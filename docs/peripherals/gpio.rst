GPIO
====

The GPIO block controls one bidirectional pin. The current SoC configuration
instantiates eight copies at ``0x0000_8000`` and exposes them as
``pin_io[0][7:0]``.

Signal interface
----------------

Port directions are relative to one ``gpio`` instance. The register signals
are the local SoC memory-mapped interface; the top level instantiates eight
one-bit instances for each GPIO device. This module has no byte strobes or
completion outputs; the SoC wrapper supplies the bus completion response.

.. list-table:: GPIO module signals
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
   * - ``write_data_i``
     - Input
     - 1
     - Value written to the selected one-bit register.
   * - ``read_i``
     - Input
     - 1
     - Register read request.
   * - ``read_address_i[2:0]``
     - Input
     - 3
     - Word index of the register being read.
   * - ``read_data_o``
     - Output
     - 1
     - Value read from the selected register.
   * - ``write_error_o`` / ``read_error_o``
     - Output
     - 1 each
     - Asserted for an address above register 5.
   * - ``pin_io``
     - Inout
     - 1
     - Bidirectional external GPIO pin.
   * - ``interrupt_o``
     - Output
     - 1
     - Selected level or edge interrupt request.

Signal behavior
---------------

* ``direction = 0`` drives the pin with the stored value;
* ``direction = 1`` places the pin in high impedance and samples the external
  value through a two-stage synchronizer;
* ``interrupt_enable`` is effective only in input mode;
* ``interrupt_level`` selects ``HIGH`` (00), ``POSEDGE`` (01), ``NEGEDGE``
  (10), or ``BOTH`` (11).

The GPIO interrupt output follows the selected condition. Edge selections
produce a pulse when the synchronized input changes; ``HIGH`` follows the
synchronized input level. Writing ``INTERRUPT_PENDING`` can clear or set the
pending state directly.

Register map
------------

All offsets are relative to the GPIO device base ``0x0000_8000``.

.. list-table:: GPIO registers
   :header-rows: 1
   :widths: 18 30 12 40

   * - Offset
     - Name
     - Access
     - Description
   * - ``0x00``
     - VALUE
     - R/W
     - Pin value
   * - ``0x04``
     - DIRECTION
     - R/W
     - 0=output, 1=input
   * - ``0x08``
     - INTERRUPT_ENABLE
     - R/W
     - Enable input interrupt generation
   * - ``0x0C``
     - INTERRUPT_LEVEL_LOW
     - R/W
     - Trigger selection bit 0
   * - ``0x10``
     - INTERRUPT_LEVEL_HIGH
     - R/W
     - Trigger selection bit 1
   * - ``0x14``
     - INTERRUPT_PENDING
     - R/W
     - Interrupt pending/control bit

Only the following fields are implemented by ``hw/io/gpio/gpio.sv``:

* ``VALUE[0]`` — output value or synchronized input value;
* ``DIRECTION[0]`` — pin direction;
* ``INTERRUPT_ENABLE[0]`` — interrupt enable;
* ``INTERRUPT_LEVEL_LOW`` and ``INTERRUPT_LEVEL_HIGH`` — the two trigger-mode bits;
* ``INTERRUPT_PENDING[0]`` — pending value.

The current reset state is output mode, with the pin value and interrupt
controls cleared and the trigger mode set to ``HIGH``.
