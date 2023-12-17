Timer
=====

Overview
--------

The Timer is a general purpose 64 bit counter that provides precise timing and synchronization capabilities, making it a critical element for a wide range of applications. The timer's precision is dictated by the system 
clock as it increments its value every clock cycle if enabled. 


Register Description
--------------------

The register map is the interface between the Timer circuitry and the CPU, the user can interact with the register through simple MMIO load / store instructions. There are 5 registers in total:

* **Threshold Register (Low and High)** (0x0)
* **Value Register (Low and High)** (0x8)
* **Configuration Register** (0x8)

Threshold Register
------------------

The threshold register holds a 64 bit value that holds the maximum value that the timer can reach before generating an interrupt or stopping. Because it's

Value Register
--------------

The value register holds the 64 bit current value of the timer.

Configuration Register
----------------------

The status register holds the status and the configuration of the Timer. Here is a description of the register fields:

.. list-table:: Status Register Fields
   :widths: 10 5 20
   :header-rows: 1

   * - Name 
     - Position
     - Description
   * - Enable Timer (ENTIM)
     - 0
     - Enable incrementing the timer.
   * - Timer Mode (MODE)
     - 1
     - Specify the action to perform once the threshold is reached.
   * - Interrupt Enable (INTEN)
     - 2
     - Enable interrupt generation.
   * - Halted Timer (HALT)
     - 3
     - Signals that the timer is stopped. 

The **MODE** field contains the current mode of the timer:

.. list-table:: Timer Mode
   :widths: 6 10
   :header-rows: 1

   * - Value 
     - Description
   * - 0
     - **Free Running Mode**: The timer doesn't stop once threshold is reached.
   * - 1
     - **One Shot Mode**: The timer halts once threshold is reached.

.. note:: In one shot mode, to resume the timer, the **HALT** bit of the status register must be cleared.
