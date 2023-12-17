UART
====

Overview
--------

The Universal Asynchronous Receiver/Transmitter (UART) is a fundamental component in modern communication systems, facilitating the exchange of serial data between devices.
Despite being one of the oldest inter-chip communication protocols, the UART is still widely used in almost every embedded environment due to its semplicity and adoption.

**Key Features:**

1. **Asynchronous Communication:**
   - UART operates asynchronously, meaning that data is transmitted without the need for a shared clock signal between the communicating devices. This flexibility makes UART suitable for different applications and allows devices with different clock speeds to communicate.

2. **Bi-Directional Communication:**
   - One of the distinguishing features of UART is its ability to enable bidirectional communication between devices. This means that data can be transmitted and received concurrently.

3. **Start and Stop Bits:**
   - UART uses start and stop bits to frame the data being transmitted. The start bit indicates the beginning of a data frame, while the stop bit(s) signal the end. This framing mechanism ensures synchronization between the transmitting and receiving devices.

4. **Data Frame Configuration:**
   - The UART data frame typically consists of a start bit, a configurable number of data bits (commonly 8 bits), an optional parity bit for error-checking, and one or more stop bits. This flexibility allows for customization based on the specific requirements of the communication link.

5. **Baud Rate Configuration:**
   - Baud rate, measured in bits per second (bps), determines the speed at which data is transmitted between devices. UART supports a wide range of baud rates, allowing users to select the optimal speed for their application.

6. **Flow Control:**
   - UART provides flow control mechanisms, including hardware (RTS/CTS) and software (XON/XOFF) options, to manage the flow of data between devices. Flow control is crucial for preventing data overrun and ensuring reliable communication in scenarios where devices operate at different speeds.

7. **Error Detection and Handling:**
   - While UART does not inherently include error-checking features, it can be augmented with additional protocols or rely on the optional parity bit for basic error detection. Error handling strategies are often implemented at higher communication layers.

8. **Widespread Application:**
   - Due to its simplicity, versatility, and wide-ranging applicability, UART is commonly used in diverse fields, including industrial automation, telecommunications, automotive systems, and consumer electronics.


ZenithSoC's UART Features
-------------------------

1. Wide range of baud rates thanks to the 15 bits clock divider.
2. Hardware flow control.
3. Enable or disable transmission and reception.
4. FIFO status is readable by software.
5. 4 different data bit modes.
6. 2 different parity modes.
7. 2 different stop bit modes.
8. Enable or disable parity bit.
9. Interrupt capabilities.
10. Low power and footprint.

Architecture
------------

The UART is composed by 4 main modules glued together by simple logic, the 4 modules are:

* Receiver
* Transmitter
* Memory Mapped Registers
* Baud Rate Generator


Receiver
~~~~~~~~

The Receiver module in a UART (Universal Asynchronous Receiver/Transmitter) is a critical component responsible for accepting serial data from an external device or system. It is equipped with Finite State Machine (FSM) logic to efficiently manage the reception process. The receiver's functionality is closely tied to the configuration settings, which typically include parameters such as data length, parity, and stop bits.

The FSM within the Receiver module progresses through different states to synchronize with the incoming data stream, detect the start of a new frame, and accurately sample each bit. Once a complete frame is received, the Receiver extracts the data and any associated control information, making it available for further processing or storage.

Transmitter
~~~~~~~~~~~

The Transmitter module complements the Receiver by handling the transmission of serial data. Like the Receiver, the Transmitter operates based on FSM logic. It takes data from a buffer or memory location and converts it into a serial bitstream. The Transmitter also considers the configured settings, such as data length, parity, and stop bits, to format the outgoing data accurately.

The FSM within the Transmitter ensures proper synchronization with the communication protocol, including framing and bit timing. The Transmitter module is responsible for sending data to the external device or system in a reliable and standardized format.

Memory Mapped Registers
~~~~~~~~~~~~~~~~~~~~~~~

The Memory Mapped Registers serve as a bridge between the UART module and the rest of the system. These registers provide a means for software or other hardware modules to configure and control the behavior of the UART. Configuration parameters, status flags, and other control information are often stored in these registers.

Software interacts with these registers to set up the desired communication parameters, monitor the status of the UART (such as the availability of data in the receiver buffer), and initiate or control the transmission process. Memory Mapped Registers offer a standardized interface for the integration of the UART module into a larger system.

Baud Rate Generator
~~~~~~~~~~~~~~~~~~~

The Baud Rate Generator is responsible for generating the clock frequency required for accurate communication between devices. The baud rate determines the speed at which bits are transmitted or received over the serial interface. The Baud Rate Generator takes input from the system clock and divides it to produce the appropriate timing for the selected baud rate.

Accurate baud rate generation is crucial for maintaining synchronization between communicating devices. The Baud Rate Generator ensures that the transmitted and received bits are sampled at the correct intervals, facilitating reliable communication.


Transmission
------------

Transmission starts when there's data available and the device on the other side is ready to accept data (when CTS is high). A signal is brought to the transmitter FSM:

.. code-block:: systemverilog

  assign transmit = (flow_control ? uart_cts_i : 1'b1) & tx_idle & !tx_empty;

Here's the FSM diagram:

// ADD FSM IMAGE 

As soon as the transmit signal arrives, the FSM lowers the `tx` wire, signaling a start of transaction. Meanwhile a counter synchronized with the desired baud rate, count the amount of samples (16 times the baud rate), once the counter arrives at 15 (all bits set), the FSM load the data from the TX buffer 
and start the data phase. Every 16 counter ticks the data loaded get shifted, so the transmission starts from the LSB and ends at the MSB (based on the data bits number configuration). Each shift the parity is computed, with a feedback XOR: the parity register at the start of the transaction get loaded with 
0 if the parity mode is even, 1 if it's odd, as the data get shifted the old value of the parity register gets updated by XORing the register with the data bit that is getting transmitted. 
If the parity is enabled, after the data phase ends, the parity bit gets transmitted as well. Finally the stop bit get transmitted by keeping the `tx` line low, once this ends the FSM assert the done signal and goes into IDLE state.

Reception
---------

The FSM normally stays in idle state when no communication is happening. Once the other device starts transmitting and the receiver sees the `rx` line low, the FSM starts counting the samples until 8. At this point the middle of the start bit is reached and the counter is resetted. From now the counter will be resetted 
once it normally arrives at 15, every time it reaches the maximum count, the FSM is at the middle of the transmitted bit. The phases are the same of the transmitter FSM, here's the diagram:

// ADD FSM IMAGE

Data is shifed from LSB to MSB every time a new data bit arrives, as the data gets shifted, the parity bit is updated in the same manner of the transmitter one. At the end of the frame the calculated parity bit is compared against the received parity bit if it's enabled.


Register Description
--------------------

The register map is the interface between the UART circuitry and the CPU, the user can interact with the register through simple MMIO load / store instructions. There are 4 registers:

* **Status Register** (0x0)
* **TX Buffer** (0x4)
* **RX Buffer** (0x8)
* **Event Register** (0xC)

.. warning:: MMIO UART registers only support word stores, any half-word or byte store could have unexpected results.


Status Register
~~~~~~~~~~~~~~~

The status register holds the status and the configuration of the UART, in the UART register map, it has an **offset of 0**. Here is a description of 
the register fields:

.. list-table:: Status Register Fields
   :widths: 10 5 20
   :header-rows: 1

   * - Name 
     - Position
     - Description
   * - RX Empty (RXE)
     - 0
     - RX buffer empty status.
   * - RX Full (RXF)
     - 1
     - TX buffer full status.
   * - TX Empty (TXE)
     - 2
     - RX buffer empty status.
   * - TX Full (TXF)
     - 3
     - TX buffer full status.
   * - Interrupt Enable (INTEN)
     - 8:4
     - Interrupt enable.
   * - Enable RX (RXEN)
     - 9
     - Enable reception.
   * - Enable TX (TXEN) 
     - 10
     - Enable transmission.
   * - Parity Enable (PAREN) 
     - 11
     - Enable parity bit transmission.
   * - Parity Mode (PARMD) 
     - 12
     - Parity mode configuration.
   * - Stop Bits (STBMD)
     - 13
     - Number of stop bits.
   * - Data Bits (DBMD) 
     - 15:14
     - Number of data bit each UART frame.
   * - Flow Control (FWCTRL) 
     - 16
     - Enable hardware flow control.
   * - Clock Divider (CKDIV) 
     - 31:17
     - Divide the main clock to obtain right the baud rate.

.. warning:: The user must disable temporary the transmission and reception before changing the UART configuration.


The **INTEN** field contain the bits associated to each possible interrupt source, setting a bit will enable the generation of 
the corresponding interrupt. Here's a list of the possible interrupt sources:

.. list-table:: Interrupt Enable Sources
   :widths: 6 10
   :header-rows: 1

   * - Bit 
     - Description
   * - INTEN[0]
     - Data received.
   * - INTEN[1]
     - Data transmitted.
   * - INTEN[2]
     - RX Buffer full.
   * - INTEN[3]
     - TX Buffer empty.
   * - INTEN[4]
     - Parity error.


For the communication configuration, this UART provides those parameters:

.. list-table:: DBMD Field Configuration
   :widths: 5 10
   :header-rows: 1

   * - Value 
     - Description
   * - 0
     - 5 Bits per frame.
   * - 1
     - 6 Bits per frame.
   * - 2
     - 7 Bits per frame.
   * - 3
     - 8 Bits per frame.

.. list-table:: PARMD Field Configuration
   :widths: 5 10
   :header-rows: 1

   * - Value 
     - Description
   * - 0
     - Even parity.
   * - 1
     - Odd parity.

.. list-table:: STBMD Field Configuration
   :widths: 5 10
   :header-rows: 1

   * - Value 
     - Description
   * - 0
     - 1 Stop bit.
   * - 1
     - 2 Stop bit.

TX Buffer
~~~~~~~~~

The **TX Buffer** holds the data to be transmitted, the user can push multiple bytes into this buffer by storing words at the **offset 1**.
The buffer holds 1 byte per entry, as the maximum data width per frame is 8 bit, when the buffer is not empty and the transmitter is idle, a 
pop operation is done, the data is retrieved and then transmitted.

RX Buffer
~~~~~~~~~

The **RX Buffer** holds the data that has been received, the user can pull data from here as soon as the receiver complete the reception of the uart frame, or 
can wait until the buffer get full, then retrieve all the data. The buffer holds 1 byte per entry, as the maximum data width per frame is 8 bit.

Event Register
~~~~~~~~~~~~~~

The **event register** is a 5 bit register holding the history of the interrupt events, if the corresponding bit inside the INTEN field of the status register is set, and the condition is satisfied, 
the i-th bit inside the event register is set. Consequently, an edge detector detect the positive edge of each register bit generating an interrupt if any of the bits have a transition from low to high.

.. list-table:: Event Register Fields
   :widths: 10 5 20
   :header-rows: 1

   * - Position
     - Description
   * - 0
     - Data is received and valid inside the RX Buffer.
   * - 1
     - Data has been transmitted correctly.
   * - 2
     - RX Buffer is at full capacity.
   * - 3
     - TX Buffer is empty.
   * - 3
     - Parity error on data just received.

.. note:: The register is writable as the user can choose to clear the events to have a precise description of the subsequent interrupts.