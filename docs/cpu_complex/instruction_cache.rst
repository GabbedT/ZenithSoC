Instruction Cache
=================

Overview
--------

The Instruction Cache provides low-latency instruction fetch from DDR memory to the ApogeoRV core. It implements a direct-mapped cache with configurable size and block dimensions, coupled with a fetch controller FSM that manages cache misses, memory refills, and instruction sequencing.

The cache design prioritizes sequential instruction access patterns with an integrated instruction sequencer that buffers cache blocks, enabling single-cycle fetch throughput on cache hits and sequential access within blocks.

Features
~~~~~~~~

* Direct-mapped cache architecture
* Configurable cache size (default 4KB - 16KB)
* Configurable block size (default 16-32 bytes)
* Single-cycle hit latency
* Automatic cache line refill on miss
* Instruction sequencer for sequential access optimization
* Support for non-sequential jumps and branches
* Pipeline flush capability via invalidate signal
* Region switching support (ROM to DDR transitions)
* Conflict handling with data cache load requests

Architecture
------------

The Instruction Cache consists of three main components:

* **Instruction Cache Memory**: Tag, valid bit, and data storage
* **Fetch Controller**: FSM managing cache operations and memory refills
* **Instruction Sequencer**: Buffers and delivers instructions sequentially

Cache Organization
~~~~~~~~~~~~~~~~~~

**Address Structure:**

A 32-bit address is decomposed as follows:

+--------+-------+--------+-----------+
| Tag    | Index | Offset | Byte (00) |
+========+=======+========+===========+
| [31:X] | [X:Y] | [Y:2]  | [1:0]     |
+--------+-------+--------+-----------+

Where:

* **Tag**: Upper bits for cache line identification
* **Index**: Selects cache line (log2(CACHE_SIZE / BLOCK_SIZE) bits)
* **Offset**: Selects word within block (log2(BLOCK_SIZE / 4) bits)
* **Byte**: Always 00 (word-aligned instructions)

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

* **Data Memory**: Stores instruction words (INDEX × BLOCK_WORDS × 32-bit)
* **Tag Memory**: Stores address tags (INDEX × TAG bits)
* **Valid Memory**: Single bit per cache line (INDEX × 1-bit)

Cache Memory Structure
----------------------

Data Storage
~~~~~~~~~~~~

The cache data is organized in banks:

.. code-block:: text

   Cache Line 0: [Word 0][Word 1][Word 2][Word 3]
   Cache Line 1: [Word 0][Word 1][Word 2][Word 3]
   ...
   Cache Line N: [Word 0][Word 1][Word 2][Word 3]

Each cache line stores BLOCK_SIZE / 4 words (32-bit each).

**Read Operation:**

1. Use INDEX to select cache line
2. Read entire cache line (all words in parallel)
3. Tag comparison in next cycle
4. Output entire bundle on hit

**Write Operation:**

1. Use INDEX to select cache line
2. Write entire block (all words)
3. Update tag and valid bit
4. Single-cycle write operation

Tag Comparison
~~~~~~~~~~~~~~

The cache performs tag comparison to determine hit/miss:

.. code-block:: systemverilog

   hit = (stored_tag == requested_tag) && valid_bit

**Timing:**

* Cycle 0: Issue read with address
* Cycle 1: Tag/data read from memory
* Cycle 2: Tag comparison completes, hit signal valid

This introduces 1 cycle of latency from request to hit determination.

Fetch Controller FSM
--------------------

The fetch controller implements a 6-state FSM managing cache operations:

States
~~~~~~

**IDLE**

* Waiting for fetch request from CPU
* ``stall_fetch_o`` deasserted (CPU can fetch)
* On ``fetch_i`` assertion:

  * Issue cache read with ``program_counter_i``
  * Transition to OUTCOME
  * Assert ``stall_fetch_o``

**OUTCOME**

* Wait for cache hit/miss determination
* On cache hit:

  * Output ``cache_instruction_i`` bundle
  * Assert ``valid_o``
  * Return to IDLE
  * Deassert ``stall_fetch_o``

* On cache miss:

  * Check for conflicts (D-cache using memory bus)
  * Check for invalidation (pipeline flush)
  * If clear: Issue first memory load request
  * Transition to ALLOCATION_REQ
  * Initialize ``word_counter`` to 1

**ALLOCATION_REQ**

* Issue sequential memory load requests to fetch cache block
* Requests issued for each word in block
* Address incremented: ``{tag, index, word_counter, 2'b0}``
* ``word_counter`` increments each cycle
* When all requests issued (``word_counter[OFFSET]`` set):

  * Transition to WAIT_BUNDLE
  * Reset ``word_counter``

**WAIT_BUNDLE**

* Wait for memory controller to return data
* On each ``load_channel.valid``:

  * Shift data into ``instruction_bundle``
  * Increment ``word_counter``

* When all words received (``word_counter[OFFSET]`` set):

  * Check for invalidation
  * If valid: Transition to ALLOCATE
  * If invalidated: Return to IDLE, discard data

**ALLOCATE**

* Write fetched bundle to cache
* Assert ``cache_write_o`` with full bundle
* Update tag and valid bit
* Output bundle to sequencer
* Assert ``valid_o``
* Return to IDLE

FSM Diagram
~~~~~~~~~~~

.. code-block:: text

   IDLE
    │
    │ fetch_i
    ▼
   OUTCOME
    │
    ├─ hit ────────────────────────► IDLE
    │
    └─ miss ──► ALLOCATION_REQ
                     │
                     │ all requests sent
                     ▼
                WAIT_BUNDLE
                     │
                     │ all data received
                     ▼
                ALLOCATE ──────────────► IDLE

Instruction Sequencer
---------------------

The sequencer optimizes sequential instruction fetch by buffering cache blocks and serving instructions without additional cache accesses.

Operation
~~~~~~~~~

**Buffering:**

When a cache bundle arrives:

1. Identify requested word using ``block_index`` (PC offset bits)
2. Load bundle into shift register
3. Shift to align first instruction at position 0
4. Store remaining instructions for sequential access

**Sequential Access:**

On each fetch:

1. Check if next PC is sequential (PC + 4)
2. If sequential and sequencer not empty:

   * Return instruction from sequencer[0]
   * Shift sequencer right by one word
   * Decrement ``sequencer_size``
   * No cache access required

3. If non-sequential or sequencer empty:

   * Request new bundle from cache/controller
   * Set ``request_bundle`` flag

**Sequencer Size Tracking:**

``sequencer_size`` tracks available instructions:

* On bundle arrival: ``size = (BLOCK_WORDS - 1) - block_index``
* On fetch: ``size = size - 1``
* On new bundle request: ``size = 0``

**Example (4-word block):**

.. code-block:: text

   Bundle arrives: [I0][I1][I2][I3], PC points to I1

   block_index = 1
   sequencer_size = (4 - 1) - 1 = 2
   sequencer = [I2][I3][--][--]

   Next fetch (PC+4):
     output = sequencer[0] = I2
     sequencer = [I3][--][--][--]
     sequencer_size = 1

   Next fetch (PC+8):
     output = sequencer[0] = I3
     sequencer = [--][--][--][--]
     sequencer_size = 0

   Next fetch (PC+12):
     sequencer_size = 0, request new bundle

Non-Sequential Access
~~~~~~~~~~~~~~~~~~~~~

On branch/jump (non-sequential PC):

* ``request_bundle`` asserted
* Sequencer flushed (size = 0)
* New bundle requested from cache
* Higher latency (cache access required)

This design optimizes for common case (sequential) while supporting control flow changes.

Pipeline Integration
--------------------

Fetch Interface
~~~~~~~~~~~~~~~

**Signals:**

* ``fetch_channel.fetch``: CPU requests instruction
* ``fetch_channel.address[31:0]``: Program counter
* ``fetch_channel.instruction[31:0]``: Returned instruction
* ``fetch_channel.valid``: Instruction valid
* ``fetch_channel.stall``: Stall CPU (cache miss)
* ``fetch_channel.invalidate``: Flush pipeline (branch mispredict)

**Timing Diagram (Cache Hit):**

.. code-block:: text

   Cycle:     0    1    2    3    4
              │    │    │    │    │
   fetch:     ──┐  ┌─────────┐  ┌──
              __│__│_________│__│__
   address:   A0   A1        A2
   stall:     ──┐  └──┐  ┌───────
              __│_____│__│_______
   valid:     ──────┐  └──┐  ┌───
              ______│_____│__│___
   instruction:     I0       I1

Hit latency = 0 cycles (instruction available same cycle as fetch for sequential access via sequencer)

**Timing Diagram (Cache Miss):**

.. code-block:: text

   Cycle:     0    1    2    ...  N    N+1
              │    │    │          │    │
   fetch:     ──┐  └─────────────────────
              __│_______________________
   address:   A0
   stall:     ──┐  ┌─────────────┐  └───
              __│__│_____________│______
   valid:     ──────────────────┐  └────
              __________________│_______
   instruction:                 I0

Miss latency = N cycles (depends on DDR latency, typically 20-100 cycles)

Memory Interface
~~~~~~~~~~~~~~~~

**Load Channel Signals:**

* ``load_channel.request``: Request memory read
* ``load_channel.address[31:0]``: Word address
* ``load_channel.data[31:0]``: Returned data
* ``load_channel.valid``: Data valid
* ``load_channel.invalidate``: Cancel pending requests

**Burst Read Sequence:**

.. code-block:: text

   4-word block refill (16 bytes):

   request:   ─┐  ┌┐  ┌┐  ┌┐  └─────────
   address:   A0  A1  A2  A3
   valid:     ────────────────┐  ┌┐  ┌┐  ┌
   data:                      D0  D1  D2  D3

The controller issues all requests first, then waits for responses. This allows the memory controller to optimize DDR burst access.

Special Conditions
------------------

Region Switching
~~~~~~~~~~~~~~~~

When transitioning from ROM to DDR (bootloader to main program):

* CPU Complex asserts ``region_switch_i``
* Fetch controller delays IDLE state entry
* Prevents false cache access to ROM address range
* Ensures clean transition between memory regions

**Behavior:**

.. code-block:: systemverilog

   if (invalidate_i & !region_switch_i) begin
       invalidate_pending <= 1'b1;
   end

Region switch invalidation is not pending, allowing immediate IDLE entry.

Cache Conflicts
~~~~~~~~~~~~~~~

When D-cache requests DDR access during I-cache refill:

* CPU Complex asserts ``conflict_i``
* Fetch controller defers memory requests
* D-cache given priority (data dependencies more critical)
* I-cache resumes when conflict clears

**FSM Handling:**

.. code-block:: systemverilog

   OUTCOME: begin
       if (!cache_hit_i && !conflict_i) begin
           load_channel.request = 1'b1;
       end
   end

Pipeline Invalidation
~~~~~~~~~~~~~~~~~~~~~

On branch misprediction or exception:

* CPU asserts ``invalidate_i``
* Fetch controller:

  * Aborts ongoing memory requests
  * Discards fetched but not yet allocated data
  * Returns to IDLE
  * Clears sequencer

* Prevents invalid instructions from entering pipeline

**Invalidation Handling:**

.. code-block:: systemverilog

   if (invalidate_i & (state_CRT != IDLE)) begin
       invalidate_pending <= 1'b1;
   end

   // In WAIT_BUNDLE state:
   if (invalidate_i | invalidate_pending) begin
       state_NXT = IDLE;  // Discard bundle
   end

Software Considerations
-----------------------

1. **Code Layout:**

   * Align frequently executed loops to cache block boundaries
   * Group related functions to improve spatial locality
   * Minimize instruction cache working set

2. **Branch Optimization:**

   * Use static branch prediction hints
   * Reduce branch frequency in hot paths
   * Inline small functions

3. **Cache Warming:**

   * Pre-load critical code into cache before use
   * Consider explicit prefetch mechanisms for predictable access patterns