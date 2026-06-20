Data Cache
==========

Overview
--------

The Data Cache provides low-latency load and store access to DDR memory for the ApogeoRV core. It implements a direct-mapped write-back cache with configurable size and block dimensions, featuring separate load and store controllers with sophisticated arbitration and lock mechanisms to prevent data hazards.

The cache design handles both cacheable DDR accesses and non-cacheable MMIO accesses, automatically routing transactions to the appropriate memory region while maintaining coherency through dirty tracking and write-back policies.

Features
~~~~~~~~

* Direct-mapped write-back cache architecture
* Configurable cache size (default 4KB - 8KB)
* Configurable block size (default 16 bytes)
* Separate load and store controllers with FSMs
* Dirty bit tracking for write-back policy
* Lock mechanism to prevent address conflicts
* MMIO bypass for peripheral access
* Byte, halfword, and word store operations
* Write-through on cache miss
* Automatic write-back on dirty eviction
* Priority arbitration (load > store)

Architecture
------------

The Data Cache consists of the following major components:

* **Data Cache Memory**: Tag, valid bit, dirty bit, and data storage
* **Load Controller**: FSM managing load operations and cache refills
* **Store Controller**: FSM managing store operations and write-through
* **Arbiter**: Priority-based arbitration for cache port access
* **Lock Logic**: Address-based locking to prevent data hazards
* **Memory Region Decoder**: Routes DDR vs MMIO transactions

Cache Organization
~~~~~~~~~~~~~~~~~~

**Address Structure:**

A 32-bit address is decomposed as follows:

+--------+-------+--------+-----------+
| Tag    | Index | Offset | Byte      |
+========+=======+========+===========+
| [31:X] | [X:Y] | [Y:2]  | [1:0]     |
+--------+-------+--------+-----------+

Where:

* **Tag**: Upper bits for cache line identification
* **Index**: Selects cache line (log2(CACHE_SIZE / BLOCK_SIZE) bits)
* **Offset**: Selects word within block (log2(BLOCK_SIZE / 4) bits)
* **Byte**: Byte offset within word (2 bits)

**Example (4KB cache, 16-byte blocks):**

* CACHE_SIZE = 4096 bytes
* BLOCK_SIZE = 16 bytes
* Number of blocks = 4096 / 16 = 256
* Words per block = 16 / 4 = 4

Address breakdown:

+----------+----------+---------+------+
| Tag      | Index    | Offset  | Byte |
+==========+==========+=========+======+
| [31:10]  | [9:4]    | [3:2]   | [1:0]|
+----------+----------+---------+------+
| 22 bits  | 6 bits   | 2 bits  | 2    |
+----------+----------+---------+------+

**Memory Components:**

* **Data Memory**: Stores data words with byte-level write (INDEX × BLOCK_WORDS × 32-bit)
* **Tag Memory**: Stores address tags (INDEX × TAG bits)
* **Valid Memory**: Valid bit per cache line (INDEX × 1-bit)
* **Dirty Memory**: Dirty bit per cache line (INDEX × 1-bit)

Cache Memory Structure
----------------------

Data Storage
~~~~~~~~~~~~

The cache data is organized with byte-level write granularity:

.. code-block:: text

   Cache Line 0: [Word 0][Word 1][Word 2][Word 3] + Tag + Valid + Dirty
   Cache Line 1: [Word 0][Word 1][Word 2][Word 3] + Tag + Valid + Dirty
   ...
   Cache Line N: [Word 0][Word 1][Word 2][Word 3] + Tag + Valid + Dirty

Each cache line stores BLOCK_SIZE / 4 words (32-bit each) plus metadata.

**Read Operation:**

1. Use INDEX to select cache line
2. Use OFFSET to select word within line
3. Tag comparison in next cycle
4. Output single word on hit

**Write Operation:**

1. Use INDEX to select cache line
2. Use OFFSET to select word within line
3. Use byte_write mask for partial writes
4. Update dirty bit on write
5. Single-cycle write operation

Status Bits
~~~~~~~~~~~

Each cache line has associated status:

* **Valid**: Line contains valid data
* **Dirty**: Line has been modified (needs write-back)
* **Tag**: Upper address bits for hit detection

**Hit Detection:**

.. code-block:: systemverilog

   hit = (stored_tag == requested_tag) && valid_bit

**Dirty Tracking:**

* Set on any write to cache line
* Cleared on write-back to memory
* Determines if eviction requires write-back

Load Controller FSM
-------------------

The load controller implements a 6-state FSM managing load operations:

States
~~~~~~

**IDLE**

* Waiting for load request from LSU
* On ``request_i`` assertion:

  * Check for address lock
  * If locked: Transition to WAIT_LOCK
  * If not locked: Issue cache read, transition to OUTCOME

**WAIT_LOCK**

* Wait for store controller to release lock
* Lock prevents concurrent access to same cache line
* On lock release:

  * Request lock acquisition
  * Issue cache read
  * Transition to OUTCOME

* On invalidate (pipeline flush):

  * Return to IDLE immediately

**OUTCOME**

* Wait for cache hit/miss determination
* On cache hit:

  * Forward ``cache_data_i`` to LSU
  * Assert ``valid_o``
  * Return to IDLE

* On cache miss with clean line:

  * Issue first memory load request
  * Transition to ALLOCATION_REQ

* On cache miss with dirty line:

  * Start reading cache line for write-back
  * Transition to WRITE_BACK

**WRITE_BACK**

* Write dirty cache line back to memory
* Read cache sequentially word-by-word
* Issue store requests to memory controller
* Address: ``{old_tag, index, word_counter, 2'b0}``
* When all words written back:

  * Issue first load request for new line
  * Transition to ALLOCATION_REQ

**ALLOCATION_REQ**

* Issue sequential memory load requests
* Requests issued for each word in block
* Address incremented: ``{tag, index, word_counter, 2'b0}``
* ``word_counter`` increments each cycle
* When all requests issued:

  * Transition to ALLOCATE
  * Reset ``word_counter``

**ALLOCATE**

* Wait for memory controller to return data
* On each ``load_channel.valid``:

  * Write data to cache
  * Increment ``word_counter``
  * If word is requested word: Save for forwarding

* When all words received:

  * Write tag, valid, clean status
  * Forward requested data to LSU
  * Assert ``valid_o``
  * Return to IDLE

FSM Diagram (Load Controller)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

   IDLE
    │
    ├─ locked ──────► WAIT_LOCK ──────┐
    │                                  │
    └─ not locked ────────────────────┘
                                       │
                                       ▼
                                    OUTCOME
                                       │
    ┌──────────────────────────────────┼───────────────────────┐
    │ hit                              │ miss                  │
    ▼                                  ▼                       │
   IDLE                            clean line              dirty line
                                       │                       │
                                       ▼                       ▼
                                 ALLOCATION_REQ          WRITE_BACK
                                       │                       │
                                       │                       │
                                       ▼                       │
                                   ALLOCATE ◄──────────────────┘
                                       │
                                       ▼
                                     IDLE

Store Controller FSM
--------------------

The store controller implements a 5-state FSM managing store operations:

States
~~~~~~

**IDLE**

* Waiting for store request from STU
* On ``request_i`` assertion:

  * Check for address lock
  * If locked: Transition to WAIT_LOCK
  * If not locked: Issue cache read, transition to OUTCOME

**WAIT_LOCK**

* Wait for load controller to release lock
* On lock release:

  * Request lock acquisition
  * Issue cache read
  * Transition to OUTCOME

**OUTCOME**

* Wait for cache hit/miss determination
* On cache hit:

  * Write data to cache with byte mask
  * Set dirty and valid bits
  * Assert ``valid_o`` to STU
  * Return to IDLE

* On cache miss:

  * Write-through to memory (bypass cache)
  * If line is clean: Invalidate cache line
  * If line is dirty: Keep line (don't invalidate)
  * Transition to WRITE_THROUGH

**WRITE_THROUGH**

* Wait for memory store to complete
* On ``store_channel.done``:

  * Assert ``valid_o`` to STU
  * Return to IDLE (or WAIT if stalled)

**WAIT**

* Handle pipeline stalls during write-through
* On stall clear:

  * Assert ``valid_o`` to STU
  * Return to IDLE

FSM Diagram (Store Controller)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

   IDLE
    │
    ├─ locked ──────► WAIT_LOCK ──────┐
    │                                  │
    └─ not locked ────────────────────┘
                                       │
                                       ▼
                                    OUTCOME
                                       │
                        ┌──────────────┴──────────────┐
                        │ hit                   miss  │
                        ▼                             ▼
                       IDLE                    WRITE_THROUGH
                                                      │
                                                      ▼
                                               WAIT (if stalled)
                                                      │
                                                      ▼
                                                     IDLE

Cache Arbitration
-----------------

Since the cache has two users (load and store controllers), arbitration is required for both cache port access and memory bus access.

Cache Port Arbitration
~~~~~~~~~~~~~~~~~~~~~~

The cache has two ports:

* **Port 0 (R/W)**: Shared between load and store controllers
* **Port 1 (R)**: Dedicated to load controller

**Arbitration Policy:**

* Load controller has priority over store controller
* On conflict: Load uses Port 0, store controller halted
* Store controller asserts ``sctrl_port_halt`` when losing arbitration

**Arbiter Logic:**

.. code-block:: systemverilog

   case ({sctrl_cache_store != '0, lctrl_cache_store != '0})
       2'b11, 2'b01: begin  // Load wins
           cache_store = lctrl_cache_store;
           cache_address[0] = lctrl_cache_address;
           sctrl_port_halt = 1'b1;
       end

       2'b10: begin  // Store has access
           cache_store = sctrl_cache_store;
           cache_address[0] = sctrl_cache_address;
       end
   endcase

Memory Bus Arbitration
~~~~~~~~~~~~~~~~~~~~~~~

Both controllers may request memory stores:

* Load controller: Write-back of dirty evicted lines
* Store controller: Write-through on cache miss

**Arbitration Policy:**

* Load controller has priority (dirty write-back is critical)
* On conflict: Store controller halted via ``sctrl_memory_halt``
* ``single_trx_o`` indicates store is from store controller (not burst)

**Arbiter Logic:**

.. code-block:: systemverilog

   case ({sctrl_store_channel.request, lctrl_store_channel.request})
       2'b11, 2'b01: begin  // Load wins
           ddr_store_channel = lctrl_store_channel;
           sctrl_memory_halt = 1'b1;
       end

       2'b10: begin  // Store has access
           ddr_store_channel = sctrl_store_channel;
           single_trx_o = 1'b1;  // Single word, not burst
       end
   endcase

Lock Mechanism
--------------

Address-Based Locking
~~~~~~~~~~~~~~~~~~~~~

The lock mechanism prevents data hazards when load and store controllers access the same cache line:

**Hazard Example Without Lock:**

1. Load to address A (miss, start refill)
2. Store to address B while A refilling (hit, write cache)
3. Data from A arrives, overwrites cache line
4. Address B data is lost

**Lock Solution:**

* Lock acquired on cache INDEX during multi-cycle operations
* Other controller must wait if accessing same INDEX
* Lock released on operation completion

**Lock Acquisition:**

.. code-block:: systemverilog

   // Load controller acquires lock
   if (ld_lock_request) begin
       ld_lock_acquired <= 1'b1;
       lock_address <= lctrl_cache_address;
   end

   // Store controller acquires lock
   if (st_lock_request) begin
       st_lock_acquired <= 1'b1;
       lock_address <= sctrl_cache_address;
   end

**Lock Checking:**

.. code-block:: systemverilog

   // Check if load should be locked
   ld_lock = (ldu_channel.request) &&
             (ldu_address.index == lock_address.index) &&
             st_lock_acquired;

   // Check if store should be locked
   st_lock = (stu_channel.request) &&
             (stu_address.index == lock_address.index) &&
             ld_lock_acquired;

**Lock Release:**

* Load lock: Released on ``ldu_channel.valid`` or ``invalidate_i``
* Store lock: Released on ``stu_channel.done``

Memory Region Decoding
-----------------------

MMIO vs DDR Routing
~~~~~~~~~~~~~~~~~~~

The data cache complex distinguishes between cacheable and non-cacheable regions:

**Address Ranges:**

.. code-block:: systemverilog

   // MMIO region (non-cacheable)
   io_load_request = (address >= PRIVATE_REGION_START) &&
                     (address <= PRIVATE_REGION_END);

   // DDR region (cacheable)
   ddr_request = !io_load_request;

**Routing:**

* MMIO load: Direct to ``io_load_channel``, bypass cache
* MMIO store: Direct to ``io_store_channel``, bypass cache
* DDR load: Through load controller and cache
* DDR store: Through store controller and cache

**MMIO Characteristics:**

* Single-cycle request (no caching overhead)
* No write-back (direct write-through)
* Suitable for peripherals with side effects
* Typical latency: 3-10 cycles (peripheral dependent)