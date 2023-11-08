AXI4 Overview
=============

AXI, which stands for Advanced eXtensible Interface, is a type of bus interface developed by ARM as part of the AMBA (Advanced Microcontroller Bus Architecture) specification. It is designed for high-performance, high-bandwidth applications, serving as a bridge for different components within a chip, such as processors, on-chip memories, peripherals, and interfaces.

AXI is notable for its support of high-speed data transfers and efficient communication through features like separate address/control and data phases, unaligned data transfers, and out-of-order transaction completion. This flexibility allows for concurrent data streams and multiple outstanding transactions, enhancing throughput and system performance.
The protocol defines several channels for read, write, and address operations, each with a handshake mechanism that ensures robust data exchange. It is highly scalable and adaptable, accommodating a wide range of performance requirements in complex SoC (System on Chip) designs.

AXI supports a wide range of features which are not required in this system, because of this the AXI4-Lite subset is used:

* Basic handshaking and data exchange
* Pipelined transactions
* Independent write and read channels
* A 32-bit bus width for data transmission

The AXI4-Lite interface is composed by two main autonomous channels: **read channel** and **write channel**.

The read_channel itself is structured into two segments: the address bus for locating data, and the data/response bus for transferring the data and providing transaction status. Below is an outline of all the bus connections:

.. list-table:: Read Channel
   :widths: 5 10 30
   :header-rows: 1

   * - Name 
     - Driver
     - Description
   * - ARADDR 
     - Master 
     - Read address of the transaction.
   * - ARVALID 
     - Master 
     - Read address is valid and can be accepted.
   * - ARREADY 
     - Slave 
     - Slave is ready to accept the address thus master can proceed to execute another transaction.
   * - RDATA 
     - Slave 
     - Read data.
   * - RVALID 
     - Slave 
     - The required data and the response are on the interface and are valid.
   * - RREADY 
     - Master 
     - Master is ready to accept the data read by the slave.
   * - RRESP 
     - Slave 
     - Transaction status response, indicate if any error occourred.

Likewise, the write channel comprise the address bus, the data bus for conveying information, and the response bus for acknowledging the transactions. Below is a detailed compilation of all the associated bus connections:

.. list-table:: Write Channel
   :widths: 5 10 30
   :header-rows: 1

   * - Name 
     - Driver
     - Description
   * - AWADDR 
     - Master 
     - Write address of the transaction.
   * - AWVALID 
     - Master 
     - Write address is valid and can be accepted.
   * - AWREADY 
     - Slave 
     - Slave is ready to accept the address thus master can proceed to execute another transaction.
   * - WDATA 
     - Master 
     - Write data.
   * - WSTRB
     - Master 
     - Valid data bytes.
   * - WVALID 
     - Master 
     - Write data is valid and can be accepted.
   * - WREADY 
     - Slave 
     - Slave is ready to accept the data thus master can proceed to execute another transaction.
   * - BVALID 
     - Slave 
     - The response is on the interface and it's valid.
   * - BREADY 
     - Master 
     - Master is ready to accept the response.
   * - BRESP 
     - Slave 
     - Transaction status response, indicate if any error occourred.


The following sections describe the components of an AXI network.

AXI Master
----------

The **master** acts as the initiator of any bus transaction. It is linked to the broader AXI network via the **master interface**, which is specified in the `axi_master.sv` file. This interface is designed to process transaction details and carry them out in compliance with the AXI protocol's standards and timing sequences. Additionally, it offers flow control features to prevent the dispatch of packets that might not be received due to various reasons. Alongside the standard AXI4-Lite signals, the interface includes additional ones for communication between the master and the router:

.. list-table:: Master Interface Signals
   :widths: 5 10 30
   :header-rows: 1

   * - Name 
     - Direction
     - Description
   * - `router_write_cts_i` 
     - Input 
     - In case of a slave switch caused by a change of the addressing slave, the router assert this signal to stall incoming write transactions from master.
   * - `write_start_i` 
     - Input 
     - Master is starting a new transaction, data, address and strobe on the interface are valid.
   * - `write_address_i` 
     - Input 
     - Write transaction address.
   * - `write_data_i` 
     - Input 
     - Write transaction data.
   * - `write_strobe_i` 
     - Input 
     - Indicate which bytes of the data are valid.
   * - `write_done_o` 
     - Output 
     - Slave has sent a response, transaction has ended.
   * - `write_cts_o` 
     - Output 
     - Clear to send another write transaction.
   * - `write_response_o` 
     - Output 
     - Transaction status response, indicate if any error occourred.
   * - `router_read_cts_i` 
     - Input 
     - In case of a slave switch caused by a change of the addressing slave, the router assert this signal to stall incoming read transactions from master.
   * - `read_start_i` 
     - Input 
     - Master is starting a new transaction, address on the interface is valid.
   * - `read_address_i` 
     - Input 
     - Address transaction address.
   * - `read_done_o` 
     - Output 
     - Slave has sent a response, transaction has ended.
   * - `read_cts_o` 
     - Output 
     - Clear to send another read transaction.
   * - `read_response_o` 
     - Output 
     - Transaction status response, indicate if any error occourred.

When the transaction begins, indicated by the activation of the start signal, the interface gathers the necessary transaction details and initiates the xVALID signal, even without the slave’s xREADY confirmation. To avoid direct combinational interactions between master and slave, all outputs are registered.

If the router grants permission for the master to initiate a new transaction and both xVALID and xREADY are active, the master has the option to withdraw the xVALID signal. Thanks to the support for pipelined transactions, start signals take precedence, allowing xVALID to remain high to facilitate multiple concurrent transactions.

However, if the slave hasn't indicated readiness by asserting xREADY, the master must refrain from sending additional transactions. The transaction is considered complete when the response signals — xVALID and xREADY — are simultaneously active.


AXI Slave
---------

The **slave** is designated as the receiver of transactions initiated by the master. Its module connects to the AXI network via the **slave interface**, detailed in the `axi_slave.sv` file. The role of this interface is to accept the incoming transaction, process it in accordance with the AXI protocol's guidelines and timing, and return a response to the originating master.

This interface not only includes the standard set of AXI4-Lite signals but also integrates additional ones to facilitate communication between the slave, the master, and the router:

.. list-table:: Slave Interface Signals
   :widths: 5 10 30
   :header-rows: 1

   * - Name 
     - Direction
     - Description
   * - `write_bus_taken_i` 
     - Input 
     - The write channel is currently taken by another slave that is executing a transaction.
   * - `write_match_o` 
     - Output 
     - The write address presented at the interface match the slave.
   * - `write_busy_o` 
     - Output 
     - The slave is currently executing a write transaction.
   * - `read_bus_taken_i` 
     - Input 
     - The read channel is currently taken by another slave that is executing a transaction.
   * - `read_match_o` 
     - Output 
     - The read address presented at the interface match the slave.
   * - `read_busy_o` 
     - Output 
     - The slave is currently executing a read transaction.
   * - `write_error_i` 
     - Input 
     - Something went wrong during the execution of the write transaction.
   * - `write_done_i` 
     - Input 
     - The slave executed a write transaction.
   * - `write_busy_i` 
     - Input 
     - The slave is currently executing a write transaction.
   * - `write_ready_i` 
     - Input 
     - The slave is ready to accept another write transaction.
   * - `write_address_o` 
     - Output 
     - Write address.
   * - `write_data_o` 
     - Output 
     - Write data.
   * - `write_strobe_o` 
     - Output 
     - Valid bytes of write data.
   * - `write_request_o` 
     - Output 
     - Write request to the slave.
   * - `read_data_i` 
     - Input 
     - Data supplied by the slave.
   * - `read_error_i` 
     - Input 
     - Something went wrong during the execution of the read transaction.
   * - `read_done_i` 
     - Input 
     - The slave executed a read transaction.
   * - `read_busy_i` 
     - Input 
     - The slave is currently executing a read transaction.
   * - `read_ready_i` 
     - Input 
     - The slave is ready to accept another read transaction.
   * - `read_address_o` 
     - Output 
     - Read address.
   * - `read_request_o` 
     - Output 
     - Read request to the slave.

Upon the assertion of the AxVALID signal, the slave promptly informs the router whether the address falls within its assigned address space. If there is a match, the slave proceeds with the transaction, and the router then links the requesting master to this slave, provided that the bus is not already engaged with another slave in the midst of execution. After a certain number of clock cycles, which may vary, the slave communicates back via the AXI interface, delivering both a transaction response and a status update.

The transaction is concluded when the slave's response is validated and the master is prepared to receive it.


AXI Router
----------

The **router** is the most important component of the AXI network. While the master and slave interfaces offer straightforward access to the bus and manage the dialogue and timing between pairs, 
the router plays an indispensable role by enabling the connection of multiple slaves to the main bus. Given that our system specifications do not call for a multi-master environment, 
the router is designed to support a single master and various slaves. Nonetheless, this design is scalable, allowing for potential expansion to accommodate more masters. 
The details of the router's design are encapsulated in the `axi_router.sv` file.

.. list-table:: Slave Interface Signals
   :widths: 5 10 30 10
   :header-rows: 1

   * - Name 
     - Direction
     - Description
     - Vector Width
   * - `master_write_channel` 
     - Bidirectional 
     - AXI4-Lite Master write interface.
     - 1
   * - `master_read_channel` 
     - Bidirectional 
     - AXI4-Lite Master read interface.
     - 1
   * - `slave_write_channel` 
     - Bidirectional 
     - AXI4-Lite Slave write interfaces (the number is configurable).
     - N_SLAVE
   * - `slave_read_channel` 
     - Bidirectional 
     - AXI4-Lite Slave read interfaces (the number is configurable).
     - N_SLAVE
   * - `slave_write_match_i` 
     - Input 
     - Slave write address match.
     - N_SLAVE
   * - `slave_write_bus_taken_o` 
     - Output 
     - The write channel is currently taken by another slave.
   * - `slave_write_match_i` 
     - Input 
     - Slave write address match.
     - N_SLAVE
   * - `slave_read_match_i` 
     - Input 
     - Slave read address match.
     - N_SLAVE
   * - `slave_read_busy_i` 
     - Input 
     - Slave is currently busy executing a read transaction.
   * - `slave_read_bus_taken_o` 
     - Output 
     - The read channel is currently taken by another slave.
   * - `axi_write_error_o` 
     - Output 
     - If the master send a write address that doesn't match any slave.
   * - `axi_read_error_o` 
     - Output 
     - If the master send a read address that doesn't match any slave.
   * - `master_write_cts_o` 
     - Output 
     - Halt the master to send write transactions if a slave switch is executing.
   * - `axi_read_error_o` 
     - Output 
     - Halt the master to send read transactions if a slave switch is executing.

When the AxVALID signal is asserted, the router quickly checks for a one-hot match signal, indicating the presence of the intended slave. If no such slave is found, a bus error is triggered. Upon identifying a valid match, the router proceeds to broadcast the 'bus take' signal to all the slaves and translates the one-hot match signal into a binary representation. This binary value identifies the specific slave involved in the transaction and serves as a selector for the multiplexers within the router. Consequently, these multiplexers direct the appropriate slave channel to communicate with the master.

The signals AxVALID and AxADDR are considered shared because they must be simultaneously presented to all slaves. This enables each slave to determine whether to send back a match response, ensuring that the routing process is both swift and accurate.