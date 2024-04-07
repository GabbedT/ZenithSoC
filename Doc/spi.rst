SPI
======== 

Overview 
-------- 


The Serial Peripheral Interface (SPI) module is a synchronous serial communication protocol widely used for short-distance communication, primarily in embedded systems. Unlike other communication protocols that require a large number of pins to connect multiple devices, SPI efficiently handles data exchange over four primary lines: Master Out Slave In (MOSI), Master In Slave Out (MISO), Serial Clock (SCLK), and Slave Select (SS).

SPI operates on a master-slave architecture, where the master device initiates the communication, controls the clock, and selects the slave devices. This structure allows for multiple slaves to be controlled by a single master, facilitating a versatile and scalable communication network within a system.

**Key Features**

- **High-Speed Data Transfer**: SPI is capable of fast data rates, typically in the range of several MHz, making it suitable for high-speed data transmission scenarios.
- **Full-Duplex Communication**: SPI enables simultaneous two-way data transfer between the master and the slave, allowing for efficient use of the communication channel.
- **Scalability**: The SPI protocol can manage multiple slave devices through the use of separate Slave Select (SS) lines, enabling scalable designs without significant complexity increase.
- **Simple Interface**: With just a few signal lines needed for operation, SPI is both easy to implement and resource-efficient, requiring less board space and fewer GPIO pins than many alternative protocols.


Data Rate
~~~~~~~~~

The SPI protocol can achieve data transmission rates of several MHz, depending on the system's clock speed and operational conditions.

Operating Modes
~~~~~~~~~~~~~~~

SPI supports four primary modes of operation, differentiated by the clock polarity (CPOL) and clock phase (CPHA) settings:

- **Mode 0 (CPOL = 0, CPHA = 0):** Clock is idle low, data captured on rising edge.
- **Mode 1 (CPOL = 0, CPHA = 1):** Clock is idle low, data captured on falling edge.
- **Mode 2 (CPOL = 1, CPHA = 0):** Clock is idle high, data captured on falling edge.
- **Mode 3 (CPOL = 1, CPHA = 1):** Clock is idle high, data captured on rising edge.

Applications
~~~~~~~~~~~~

SPI's versatility is evident in its wide range of applications across various sectors of the electronics industry:

- **Sensors and Actuators:** SPI is commonly used for interfacing with temperature sensors, accelerometers, and other sensor devices.
- **Memory Devices:** Flash memory chips and EEPROMs often utilize SPI for data writing and reading.
- **Display Modules:** SPI serves as a communication protocol for LCD and OLED displays, enabling dynamic visual output.
- **Embedded Systems:** Microcontrollers frequently employ SPI to communicate with peripheral devices, enhancing system functionality.

ZenithSoC's SPI Features
-------------------------

The SPI design is really simple and basic to mantain low complexity thus reducing the power consumption and the area usage. The main features are:

* Wide range of clock speeds thanks to the 16 bit clock divisor.
* Communicate to up to 32 different slaves.
* Support for every SPI mode.
* Support for 2 bit order modes (MSB first or LSB first).
* Configurable buffers for transmission and reception.
* Interrupt capable.

The whole design is composed by three main components:

* Registers
* Transmitter
* Receiver

The last two are implemented as very basic shift registers, so there's not much to say about them.

Registers
---------

Here are the registers with the offsets:

.. list-table::
   :align: center
   :widths: 20 10

   * - Register Name
     - Offset
   * - SPI Control Status Register
     - 0
   * - TX Buffer
     - 1
   * - RX Buffer
     - 2
   * - Event Register
     - 3
   * - Slave Select Register
     - 4

Control Status Register
~~~~~~~~~~~~~~~~~~~~~~~

.. table:: SPI Control Status Register Fields
   :widths: 20 2 50 10

   +------------------+----------+-------------------------------------------------+------------+
   | Field Name       | Position | Description                                     | Operations |
   +==================+==========+=================================================+============+
   | RX Empty         | 0        | RX buffer empty flag                            | RO         |
   +------------------+----------+-------------------------------------------------+------------+
   | RX Full          | 1        | RX buffer full flag                             | RO         |
   +------------------+----------+-------------------------------------------------+------------+
   | TX Empty         | 2        | TX buffer empty flag                            | RO         |
   +------------------+----------+-------------------------------------------------+------------+
   | TX Full          | 3        | TX buffer full flag                             | RO         |
   +------------------+----------+-------------------------------------------------+------------+
   | Idle             | 4        | SPI idle flag                                   | RO         |
   +------------------+----------+-------------------------------------------------+------------+
   | Interrupt Enable | 5        | SPI global interrupt enable                     | RW         |
   +------------------+----------+-------------------------------------------------+------------+
   | CPHA             | 6        | SPI clock phase                                 | RW         |
   +------------------+----------+-------------------------------------------------+------------+
   | CPOL             | 7        | SPI clock polarity                              | RW         |
   +------------------+----------+-------------------------------------------------+------------+
   | Bit Order        | 9        | TX and RX bit order (MSB first = 0)             | RW         |
   +------------------+----------+-------------------------------------------------+------------+
   | Clock Divisor    | 25:10    | Divide the system clock to derive the SPI clock | RW         |
   +------------------+----------+-------------------------------------------------+------------+



Event Register
~~~~~~~~~~~~~~

Interrupt event gives infos about the interrupt source. The bit must be cleared otherwise no other interrupt of each source will generate since the logic requires a positive edge of the event bit.

.. table:: Event Register Fields
   :widths: 20 2 40 10

   +-----------------+----------+--------------------------------+------------+
   | Field Name      | Position | Description                    | Operations |
   +=================+==========+================================+============+
   | Transaction Done| 0        | Data transaction finished      | RW         |
   +-----------------+----------+--------------------------------+------------+


Slave Select Register
~~~~~~~~~~~~~~~~~~~~~~

The width of this registers depend on the SPI hardware configuration, specifically the maximum nuber of selectable slaves (from 1 to 32). Each bit is connected to the CS[i] active low signal. Writing 1 to the i-th bit will select the i-th slave. At the start of the transaction then CS[i] will be set low and pulled high at the end of it.