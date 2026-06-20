CPU Complex
===========

Overview
--------

The CPU Complex integrates the ApogeoRV RISC-V core with instruction and data caches, interrupt management, and memory interfaces. It provides a complete processing subsystem with Harvard architecture, separate I-cache and D-cache with configurable sizes, and sophisticated load/store arbitration for optimal memory bandwidth utilization.

The complex handles memory region decoding, cache coherency, ROM/DDR region switching, and interrupt synchronization, presenting a unified interface to the SoC interconnect.

Features
~~~~~~~~

* ApogeoRV 32-bit RISC-V core (RV32IMF_Zfinx_Zba_Zbb_Zbs extensions)
* Separate instruction and data caches with configurable sizes
* Out-of-order execution with reorder buffer
* Branch prediction with BTB and predictor tables
* Store buffer for write coalescing
* Dual memory region support (Boot ROM + DDR)
* MMIO region for peripheral access
* Load channel arbitration with priority scheme
* Interrupt controller with vector support
* External core halting capability

Architecture
------------

The CPU Complex consists of the following major components:

* **ApogeoRV Core**: RISC-V processor with out-of-order back-end
* **Instruction Cache**: Provides low-latency instruction fetch from DDR
* **Data Cache**: Provides low-latency load/store access to DDR
* **Load Arbiter**: Arbitrates between I-cache and D-cache DDR requests
* **Memory Region Decoder**: Routes transactions to ROM, DDR, or MMIO regions

Signal Interface
~~~~~~~~~~~~~~~~

**Global Signals:**

* ``clk_i``: System clock
* ``rst_n_i``: Active-low reset
* ``halt_i``: Halt core execution (debug)

**Boot ROM Interface:**

* ``rom_fetch_channel``: Fetch interface to boot ROM (addresses ≤ BOOT_END)

**DDR Memory Interfaces:**

* ``ddr_load_channel``: Load interface to DDR controller
* ``ddr_store_channel``: Store interface to DDR controller
* ``single_strx_o``: Indicates single (non-burst) store transaction
* ``instr_load_o``: Indicates load is for instruction fetch (not data)
* ``ldr_ready_i``: DDR controller ready for load requests

**MMIO Interfaces:**

* ``io_load_channel``: Load interface to peripheral bus
* ``io_store_channel``: Store interface to peripheral bus

**Interrupt Interface:**

* ``gen_interrupt_i``: General external interrupt
* ``nmsk_interrupt_i``: Non-maskable interrupt
* ``timer_interrupt_i``: Timer interrupt
* ``interrupt_vector_i[7:0]``: Interrupt vector number
* ``interrupt_ackn_o``: Interrupt acknowledgment to controller

Configuration Parameters
-------------------------

Core Parameters
~~~~~~~~~~~~~~~

+---------------------------+----------+-------------------------------------------------------+
| Parameter                 | Default  | Description                                           |
+===========================+==========+=======================================================+
| PREDICTOR_SIZE            | 512      | Branch predictor table entries                        |
+---------------------------+----------+-------------------------------------------------------+
| BTB_SIZE                  | 512      | Branch Target Buffer entries                          |
+---------------------------+----------+-------------------------------------------------------+
| STORE_BUFFER_SIZE         | 4        | Store buffer depth for write coalescing               |
+---------------------------+----------+-------------------------------------------------------+
| INSTRUCTION_BUFFER_SIZE   | 8        | Instruction buffer entries in front-end               |
+---------------------------+----------+-------------------------------------------------------+
| ROB_DEPTH                 | 32       | Reorder Buffer entries (out-of-order window)          |
+---------------------------+----------+-------------------------------------------------------+

Cache Parameters
~~~~~~~~~~~~~~~~

+---------------------------+----------+-------------------------------------------------------+
| Parameter                 | Default  | Description                                           |
+===========================+==========+=======================================================+
| DCACHE_SIZE               | 4096     | Data cache size in bytes (default 4KB)                |
+---------------------------+----------+-------------------------------------------------------+
| DBLOCK_SIZE_BYTE          | 16       | Data cache block size in bytes                        |
+---------------------------+----------+-------------------------------------------------------+
| ICACHE_SIZE               | 4096     | Instruction cache size in bytes (default 4KB)         |
+---------------------------+----------+-------------------------------------------------------+
| IBLOCK_SIZE_BYTE          | 16       | Instruction cache block size in bytes                 |
+---------------------------+----------+-------------------------------------------------------+

**Cache Organization:**

* Direct-mapped cache structure
* Physically indexed, physically tagged
* Write-back policy for data cache
* Automatic cache line refill on miss
* Byte-level write enable via store buffer

ApogeoRV Core
-------------

Architecture Overview
~~~~~~~~~~~~~~~~~~~~~

The ApogeoRV core implements a high-performance RISC-V architecture with:

* **Front-End**: Fetch, decode, register renaming, instruction scheduling
* **Back-End**: Out-of-order execution, reorder buffer, commit stage
* **Execution Units**: Integer ALU, Load/Store Unit, Floating-Point Unit (Zfinx)
* **Branch Prediction**: 2-level predictor with BTB for target prediction

**ISA Support:**

* RV32I: Base integer instruction set
* RV32M: Integer multiplication and division
* RV32F: Single-precision floating-point (Zfinx variant - FP in integer registers - No FDIV/FSQRT)
* RV32B: Bit manipulation extension (Zba, Zbb, Zbs)
* Machine and User privilege modes
* M-mode interrupts and exceptions

Pipeline Stages
~~~~~~~~~~~~~~~

**Front-End Stages:**

1. **Fetch**: Requests instructions from I-cache or ROM
2. **Decode**: Decodes instructions and extracts operands
3. **Register Read**: Allocates ROB index for instruction tracking and read registers
4. **Issue**: Issues instructions to execution units when operands ready

**Back-End Stages:**

1. **Execute**: Parallel execution units (ITU, LSU, FPU)
2. **Commit**: Manage parallel valid results from execution units
3. **Reorder Buffer**: Reorder OoO instructions
4. **Writeback**: Updates architectural register file

**Branch Resolution:**

* Speculative execution with branch prediction
* Branch resolved in execute stage
* Pipeline flush on misprediction
* Return address stack for function calls

Memory System
-------------

Memory Map
~~~~~~~~~~

The CPU Complex operates on three memory regions:

+----------------------+---------------------------+--------------------------------+
| Region               | Address Range             | Description                    |
+======================+===========================+================================+
| Boot ROM             | 0x0000_0000 - BOOT_END    | Non-volatile boot code         |
+----------------------+---------------------------+--------------------------------+
| User Memory (DDR)    | USER_MEMORY_REGION_START  | Main cacheable memory          |
|                      | - (PRIVATE_REGION_START-1)| Accessed via caches            |
+----------------------+---------------------------+--------------------------------+
| MMIO (Peripherals)   | PRIVATE_REGION_START      | Memory-mapped peripherals      |
|                      | - PRIVATE_REGION_END      | Non-cacheable, direct access   |
+----------------------+---------------------------+--------------------------------+

**Address Routing:**

* Fetch from ROM: ``address <= BOOT_END``
* Fetch from DDR: ``address >= USER_MEMORY_REGION_START``
* Load/Store from DDR: ``PRIVATE_REGION_START > address >= USER_MEMORY_REGION_START``
* Load/Store from MMIO: ``PRIVATE_REGION_START <= address <= PRIVATE_REGION_END``

Instruction Cache
~~~~~~~~~~~~~~~~~

**Operation:**

1. Core requests fetch from program counter
2. If PC in ROM region, bypass I-cache (direct ROM access)
3. If PC in DDR region:

   * Check I-cache for hit
   * On hit: Return instruction immediately
   * On miss: Request cache line from DDR via load channel
   * Refill entire cache block (IBLOCK_SIZE_BYTE)
   * Resume fetch after refill completes

**Region Switching:**

When transitioning from ROM to DDR (e.g., jumping from bootloader to main program):

* ``region_switch`` signal asserted
* I-cache controller delays fetch until switch completes
* Prevents false cache misses during region transition

**Cache Conflicts:**

If D-cache issues DDR load while I-cache is refilling:

* ``conflict_i`` signal from D-cache to I-cache
* I-cache stalls to allow D-cache priority
* Maintains forward progress on data loads

Data Cache
~~~~~~~~~~

**Operation:**

Load:

1. Core issues load request with address
2. If address in MMIO region: Bypass cache, direct peripheral access
3. If address in DDR region:

   * Check D-cache for hit
   * On hit: Return data immediately
   * On miss: Request cache line from DDR
   * Refill cache block and forward requested word
   * Future loads to same block hit in cache

Store:

1. Core issues store request with address, data, width
2. If address in MMIO region: Direct write to peripheral
3. If address in DDR region:

   * Write through store buffer
   * Store buffer coalesces writes to same cache line
   * On cache hit: Update cache and buffer
   * On cache miss: Allocate line, then update
   * Background writeback from buffer to DDR

**Store Buffer:**

* FIFO buffer with STORE_BUFFER_SIZE entries
* Allows core to continue execution while stores commit
* Handles byte, halfword, and word stores
* Maintains store ordering (TSO memory model)

Load Channel Arbitration
~~~~~~~~~~~~~~~~~~~~~~~~~

Both I-cache and D-cache share a single DDR load channel. The arbiter implements priority-based selection:

**Arbitration Policy:**

* D-cache has higher priority than I-cache
* Prevents core stalls due to data dependencies
* I-cache stalls during D-cache refill (instructions can wait)

**Arbiter FSM:**

1. **IDLE**: No requests pending

   * If D-cache requests: Grant to D-cache
   * Else if I-cache requests: Grant to I-cache

2. **BUSY**: Request in progress

   * Latch priority bit (indicates which cache owns channel)
   * Route incoming DDR data to selected cache
   * Hold priority until ``ddr_load_channel.valid`` deasserts
   * Prevents switching mid-transaction

**Stall Signals:**

* ``stall_fetch``: Asserted when I-cache loses arbitration
* ``stall_data``: Asserted when D-cache loses arbitration (rare)

Interrupt Handling
------------------

Interrupt Types
~~~~~~~~~~~~~~~

**External Interrupt:**

* Source: ``gen_interrupt_i``
* Maskable via CSR ``MIE.MEIE`` bit
* Vector provided by ``interrupt_vector_i``

**Timer Interrupt:**

* Source: ``timer_interrupt_i``
* Maskable via CSR ``MIE.MTIE`` bit
* Fixed vector: ``TIMER_INTERRUPT`` (7)

**Non-Maskable Interrupt:**

* Source: ``nmsk_interrupt_i``
* Cannot be masked (overrides global interrupt enable)
* Fixed vector: ``NON_MASKABLE_INTERRUPT`` (0)

Interrupt Flow
~~~~~~~~~~~~~~

1. Interrupt source asserts interrupt line
2. CPU Complex registers interrupt signal
3. Core checks interrupt enable in CSR (for maskable interrupts)
4. On interrupt acceptance:

   * Pipeline flushes
   * PC saved to ``MEPC`` CSR
   * Jump to trap handler (``MTVEC`` + vector * 4)
   * ``MCAUSE`` CSR records interrupt vector
   * Core asserts ``interrupt_ackn_o``

5. Trap handler services interrupt
6. Handler executes ``MRET`` to return
7. PC restored from ``MEPC``

Core Halt
---------

The ``halt_i`` signal provides a mechanism to stop core execution for debugging:

* When asserted: Core freezes pipeline, no new instructions fetched
* Memory system remains responsive
* Caches continue servicing refills in progress
* Interrupts remain pending but not serviced
* When deasserted: Core resumes from halted state