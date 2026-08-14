Instruction Cache
=================

Overview
--------

The Instruction Cache provides low-latency instruction fetch from DDR memory
to the ApogeoRV core. It implements a direct-mapped cache with configurable
size and block dimensions, coupled with a fetch controller that manages cache
misses, memory refills, and instruction delivery.

The fetch path is pipelined: a request FIFO absorbs every fetch, so the
frontend keeps requesting instructions through cache misses and refills and
stalls only when the FIFO is full. Sequential code is served from a retained
copy of the last delivered block or from a speculative pre-read of the next
block, which makes sequential block transitions cost zero stall cycles. A
returning refill delivers words to matching queued requests as soon as they
arrive (early restart) instead of waiting for the whole line to be written
back.

Features
~~~~~~~~

* Direct-mapped cache architecture
* Configurable cache size and block size (SoC default: 4KB, 16-byte blocks)
* 16-entry request FIFO for pipelined request acceptance
* Retained-block serving: one word per cycle without cache accesses
* Speculative next-block pre-read: zero-stall sequential block transitions
* Automatic cache line refill on miss
* Early restart: refill words delivered to matching requests before allocation
* Support for non-sequential jumps and branches
* Pipeline flush capability via invalidate signal
* Region switching support (ROM to DDR transitions)
* Conflict handling with data cache load requests

Architecture
------------

The Instruction Cache complex consists of two main components:

* **Instruction Cache Memory**: Tag, valid bit, and data storage
* **Fetch Controller**: Request FIFO, FSM managing cache operations and
  memory refills, and the serving paths

The previous instruction sequencer is gone. Its job is split between the
retained bundle, which serves the remaining words of the current block, and
the speculative pre-read, which covers the transition to the next block.
This is what lets the controller accept and queue requests while a refill is
in flight.

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

**Example (SoC configuration: 4KB cache, 16-byte blocks):**

* CACHE_SIZE = 4096 bytes
* BLOCK_SIZE = 16 bytes
* Number of blocks = 4096 / 16 = 256
* Words per block = 16 / 4 = 4

Address breakdown:

+----------+----------+---------+------+
| Tag      | Index    | Offset  | Byte |
+==========+==========+=========+======+
| [31:12]  | [11:4]   | [3:2]   | [1:0]|
+----------+----------+---------+------+
| 20 bits  | 8 bits   | 2 bits  | 2    |
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
2. Write entire block (all words) in a single cycle
3. Update tag and valid bit in the same write

Tag and valid are written together with the data, so a partially written
line can never produce a false hit.

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

Fetch Controller
----------------

The fetch controller (``hw/memory/cache/icache/fetch_controller.sv``) owns
the request queue, the cache read/write ports, and the DDR load channel.

Request FIFO
~~~~~~~~~~~~

Requests are queued so the frontend keeps fetching while a refill is in
flight:

* Depth is 16, deeper than the frontend's 8-entry instruction buffer, so the
  registered full flag can never race a fetch into a dropped push (a dropped
  push would desync the frontend address/instruction streams).
* The FIFO is fall-through: a request arriving in an empty FIFO is served in
  the same cycle.
* A push happens on every ``fetch_i``; the pop happens at delivery, not at
  access start.
* On ``invalidate_i`` the pointers are cleared and the fetch issued together
  with the invalidate (the redirect target) is written at index 0, so the
  redirect stream is never lost.
* The FIFO can never overflow in practice: the frontend is bounded by its
  own instruction buffer and stops fetching when the full flag asserts.

Serving Paths
~~~~~~~~~~~~~

In the IDLE state the head request is served by one of three paths:

1. **Retained bundle** — the last delivered block is kept in a register.
   If the head belongs to that block, the word is multiplexed out directly:
   one word per cycle, no cache access, no stall.
2. **Speculative pre-read** — while the controller serves the current block
   at offset 1 or later, it issues a read for the next block
   (``head + 16``). By the time the head reaches the next block the bundle
   is already registered. The serve is validated by an address match, so a
   wrong guess (a taken branch) falls back to the normal cache read. The
   pre-read is dropped on invalidate and when a refill writes a line (the
   write may replace the line the speculative read returned).
3. **Cache read** — the normal path: a read is issued and the word is
   delivered in the OUTCOME state (1 stall cycle per block transition).

With paths 1 and 2, sequential code inside a block and across block
boundaries is served without stalling the frontend at all.

FSM States
~~~~~~~~~~

The fetch controller implements a 5-state FSM:

**IDLE**

* Only state with ``stall_fetch_o`` deasserted
* On a non-empty FIFO:

  * Serve the head from the retained bundle or the speculative pre-read,
    or issue a cache read and transition to OUTCOME
* While serving at offset 1 or later, issue the speculative pre-read of the
  next block

**OUTCOME**

* Wait for cache hit/miss determination
* On cache hit:

  * Deliver the requested word, validated against ``read_block`` — the
    block the read was issued for; an invalidate can replace the head while
    the read is in flight, and a mismatch re-enters IDLE to serve the new
    head instead of delivering the old line
  * Retain the bundle, pop the request, return to IDLE

* On cache miss:

  * Latch the head block as ``refill_block``
  * Fire the first memory load request (word 0), frozen by the D-cache
    conflict signal and by invalidation
  * Transition to REFILL_REQ with ``word_counter`` = 1

**REFILL_REQ**

* Issue the remaining load requests for the block, one per cycle, at
  incrementing addresses ``{refill_block, word_counter, 2'b0}``
* Frozen by ``stall_i`` and ``conflict_i``: a D-cache refill has priority
  on the shared DDR port
* Once word 0 has been fired, words 1-3 are always issued: a partial batch
  would corrupt the DDR bridge's two-entry pair buffer
* When all requests are out, transition to REFILL and reset ``word_counter``

**REFILL**

* Count the four returning ``load_channel.valid`` beats
* Each beat is shifted into the instruction bundle, newest word at the MSB
* Early restart: each beat is delivered directly to the queued request that
  matches it (see below)
* After the fourth beat, transition to WRITE

**WRITE**

* Write the whole bundle to the cache with tag and valid in a single write
* Retain the bundle for the serving paths
* Return to IDLE
* If an invalidate arrived during the refill, the write is skipped and the
  line stays invalid

The controller also keeps simulation-only performance taps: ``fetch_access_CRT``
counts delivered requests and ``fetch_hit_CRT`` counts those served without a
refill (``stall_fetch_o`` is asserted in every non-IDLE state).

Early Restart
~~~~~~~~~~~~~

While the refill beats stream back, the controller checks the FIFO head on
every beat. If the head belongs to the block being refilled, the word is
delivered immediately instead of waiting for the WRITE state:

* **Direct match** — the head's offset equals the incoming beat: deliver
  ``load_channel.data`` and pop the request.
* **Buffered match** — the head's offset is behind the incoming beat (the
  request was queued late): deliver the word from the instruction bundle.
  The bundle shifts newest-first (``{data, bundle[3:1]}``), so after *k*
  beats word *j* sits at slot *j + 4 - k*.

Delivery is gated by invalidation: a redirect mid-refill discards the beats
and the line is never written.

This cuts the miss penalty by the number of words the frontend is waiting
for: it continues as soon as the first matching word arrives rather than
after the whole line is allocated.

Pipeline Integration
--------------------

Fetch Interface
~~~~~~~~~~~~~~~

**Signals:**

* ``fetch_channel.fetch``: CPU requests instruction
* ``fetch_channel.address[31:0]``: Program counter
* ``fetch_channel.instruction[31:0]``: Returned instruction
* ``fetch_channel.valid``: Instruction valid
* ``fetch_channel.stall``: Frontend stall — asserted only when the request
  FIFO is full or an I-cache flush is in progress (``fifo_full |
  flush_busy_o``). Unlike the previous design, cache reads and refills do
  not stall the frontend directly.
* ``fetch_channel.invalidate``: Flush pipeline (branch mispredict)

Memory Interface
~~~~~~~~~~~~~~~~

**Load Channel Signals:**

* ``load_channel.request``: Request memory read
* ``load_channel.address[31:0]``: Word address
* ``load_channel.data[31:0]``: Returned data
* ``load_channel.valid``: Data valid
* ``load_channel.invalidate``: Cancel pending requests

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

   if (invalidate_i & (state_CRT != IDLE)) begin
       invalidate_pending <= !region_switch_i;
   end

Region switch invalidation is not pending, allowing immediate IDLE entry.

Cache Conflicts
~~~~~~~~~~~~~~~

When D-cache requests DDR access during I-cache refill:

* CPU Complex asserts ``conflict_i``
* Fetch controller defers memory requests
* D-cache given priority (data dependencies more critical)
* I-cache resumes when conflict clears

Pipeline Invalidation
~~~~~~~~~~~~~~~~~~~~~

On branch misprediction or exception:

* CPU asserts ``invalidate_i``
* Fetch controller:

  * Clears the request FIFO, keeping the redirect-target fetch
  * Drops the retained bundle and the speculative pre-read
  * Gates early-restart delivery
  * An in-flight refill finishes issuing its batch (the DDR bridge requires
    it), but the returning data is discarded and the line is not written

* Prevents invalid instructions from entering pipeline

An invalidate that arrives while the FSM is outside IDLE is latched in
``invalidate_pending`` so the in-flight state machine can finish cleanly.
The pending flag defers new refill requests and gates delivery until the
FSM reaches IDLE; region switches skip the pending latch.

Whole-Cache Flush
~~~~~~~~~~~~~~~~~

The complex has a flush port (``flush_i``/``flush_busy_o``/``flush_done_o``)
that invalidates every cache line while the fetch controller is stalled.
The flush waits for the controller to return to IDLE, then walks the line
index on the cache write port, clearing valid bits one line per cycle.

At the SoC level this port is currently tied off (``cpu_complex.sv``), so
FENCE.I does not invalidate the I-cache RAMs.
