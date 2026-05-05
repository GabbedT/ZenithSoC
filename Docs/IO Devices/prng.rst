PRNG - Pseudo Random Number Generator
======================================

Overview
--------

The Pseudo Random Number Generator (PRNG) module provides a hardware-based source of pseudo-random numbers for applications requiring randomness, such as cryptographic operations, simulations, testing, and security features. The module implements a 64-bit Linear Feedback Shift Register (LFSR) with a carefully chosen polynomial to ensure a maximal-length sequence with good statistical properties.

The PRNG offers a simple memory-mapped interface allowing software to seed the generator and read random values on demand. The hardware implementation provides significantly better performance than software-based random number generation while maintaining deterministic behavior when seeded with known values.

Key Features
------------

* **64-bit LFSR**: Maximal-length sequence with period of 2^64 - 1
* **Hardware Acceleration**: Single-cycle random number generation
* **Seedable**: Programmable seed value for reproducible sequences
* **Dual 32-bit Access**: Read upper and lower halves independently
* **High Throughput**: New random value every clock cycle
* **Low Resource Usage**: Minimal logic footprint
* **Memory-Mapped Interface**: Simple register-based control via AXI bus

Architecture
------------

Block Diagram
~~~~~~~~~~~~~

The PRNG module consists of the following main components:

* **64-bit LFSR Register**: Shift register with feedback taps
* **Feedback Polynomial**: XOR network implementing polynomial x^64 + x^63 + x^61 + x^60 + 1
* **Seed Registers**: Two 32-bit registers for initialization (lower and upper halves)
* **Output Multiplexer**: Selects between lower and upper 32 bits for reading
* **Control Logic**: Manages read/write operations and acknowledgments

LFSR Implementation
~~~~~~~~~~~~~~~~~~~

The module uses a Galois-style LFSR with the following characteristics:

* **Polynomial**: x^64 + x^63 + x^61 + x^60 + 1
* **Taps**: Bits 63, 62, 60, 59 (XORed together)
* **Period**: 2^64 - 1 (maximum length sequence)
* **Update**: Shifts left by one bit per clock cycle
* **Feedback**: XOR of tap bits fed back to bit 0

Signal Description
~~~~~~~~~~~~~~~~~~

=============================  =========  ===========  ====================================
Signal Name                    Direction  Width        Description
=============================  =========  ===========  ====================================
``clk_i``                      Input      1            System clock
``rst_n_i``                    Input      1            Active-low asynchronous reset
``write_i``                    Input      1            Write enable signal
``write_data_i[31:0]``         Input      32           Data to write to seed register
``write_address_i``            Input      1            Seed register selection (0=lower, 1=upper)
``write_done_o``               Output     1            Write operation acknowledgment
``read_i``                     Input      1            Read enable signal
``read_address_i``             Input      1            Output register selection (0=lower, 1=upper)
``read_done_o``                Output     1            Read operation acknowledgment
``read_data_o[31:0]``          Output     32           Random number output
=============================  =========  ===========  ====================================

Functional Description
----------------------

LFSR Operation
~~~~~~~~~~~~~~

The Linear Feedback Shift Register operates as follows:

1. **Normal Operation (no write)**

   * LFSR shifts left by one bit each clock cycle
   * New bit 0 = bit[63] ⊕ bit[62] ⊕ bit[60] ⊕ bit[59]
   * Generates maximal-length pseudo-random sequence
   * Period: 2^64 - 1 cycles before repeating

2. **Seeding Operation (write_i = 1)**

   * Write halts LFSR shifting
   * New seed value loaded into selected half (upper or lower 32 bits)
   * Other half remains unchanged
   * Next cycle resumes normal LFSR operation

Seed Loading
~~~~~~~~~~~~

The 64-bit LFSR can be seeded in two 32-bit write operations:

**Lower 32 bits** (``write_address_i = 0``)
  * Loads ``write_data_i`` into ``lfsr[31:0]``
  * Upper 32 bits (``lfsr[63:32]``) remain unchanged

**Upper 32 bits** (``write_address_i = 1``)
  * Loads ``write_data_i`` into ``lfsr[63:32]``
  * Lower 32 bits (``lfsr[31:0]``) remain unchanged

**Important Notes:**

* Seeding with all zeros (0x0000_0000_0000_0000) results in a stuck state
* Recommended to seed with non-zero values
* Two consecutive writes required to initialize full 64-bit state
* LFSR continues shifting between seed operations

Random Number Reading
~~~~~~~~~~~~~~~~~~~~~

Random numbers can be read in two ways:

**Lower 32 bits** (``read_address_i = 0``)
  * Returns ``lfsr[31:0]``
  * Good for applications needing 32-bit random values

**Upper 32 bits** (``read_address_i = 1``)
  * Returns ``lfsr[63:32]``
  * Can be combined with lower read for 64-bit values

**Reading Behavior:**

* Read does NOT halt LFSR shifting
* LFSR continues to advance every clock cycle
* Each read returns the current LFSR state
* Reading is non-destructive (LFSR state unchanged)
* One clock cycle latency from ``read_i`` to valid ``read_data_o``

Statistical Properties
~~~~~~~~~~~~~~~~~~~~~~

The LFSR polynomial provides:

* **Uniform Distribution**: All 2^64 - 1 non-zero states visited exactly once per period
* **Balance**: Approximately equal number of 0s and 1s
* **Run Properties**: Good distribution of bit run lengths
* **Correlation**: Low autocorrelation for most lag values
* **Passes Basic Tests**: Chi-square, runs test, serial correlation

**Limitations:**

* NOT cryptographically secure (predictable if state is known)
* Zero state (all zeros) is not part of the sequence
* Deterministic - same seed produces same sequence
* Should not be used for security-critical applications without additional processing

Register Map
------------

Base Address
~~~~~~~~~~~~

The PRNG base address is determined by the SoC memory map. See the Memory Map section for specific address allocation.

Register Summary
~~~~~~~~~~~~~~~~

===========  ===========  ======  ===========  ===============================================
Address      Name         Access  Reset        Description
===========  ===========  ======  ===========  ===============================================
0x00         SEED_LOW     W       0x0          Seed register - lower 32 bits
0x04         SEED_HIGH    W       0x0          Seed register - upper 32 bits
0x00         RAND_LOW     R       0x0          Random number output - lower 32 bits
0x04         RAND_HIGH    R       0x0          Random number output - upper 32 bits
===========  ===========  ======  ===========  ===============================================

**Note:** The same addresses are used for both seeding (write) and reading random numbers (read). The operation type (read vs. write) determines the function.

Register Descriptions
---------------------

SEED_LOW Register (Offset 0x00, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Lower 32-bit Seed Register**

===========  =========  ===========  ===============================================
Bit Field    Access     Reset        Description
===========  =========  ===========  ===============================================
[31:0]       W          0x0          Lower 32 bits of LFSR seed value

                                     Writing to this register loads the lower
                                     half of the 64-bit LFSR state. The upper
                                     32 bits remain unchanged.
===========  =========  ===========  ===============================================

**Usage Notes:**

* Write operation halts LFSR for one clock cycle
* Recommended to write both SEED_LOW and SEED_HIGH to fully initialize
* Avoid seeding entire LFSR with all zeros (stuck state)
* Writing during operation reseeds and restarts the sequence

SEED_HIGH Register (Offset 0x04, Write Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Upper 32-bit Seed Register**

===========  =========  ===========  ===============================================
Bit Field    Access     Reset        Description
===========  =========  ===========  ===============================================
[31:0]       W          0x0          Upper 32 bits of LFSR seed value

                                     Writing to this register loads the upper
                                     half of the 64-bit LFSR state. The lower
                                     32 bits remain unchanged.
===========  =========  ===========  ===============================================

**Usage Notes:**

* Write operation halts LFSR for one clock cycle
* Typically written after SEED_LOW to complete initialization
* Can be written independently to change only upper 32 bits
* LFSR resumes shifting in next clock cycle after write

RAND_LOW Register (Offset 0x00, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Lower 32-bit Random Number Output**

===========  =========  ===========  ===============================================
Bit Field    Access     Reset        Description
===========  =========  ===========  ===============================================
[31:0]       R          0x0          Current lower 32 bits of LFSR state

                                     Reading returns lfsr[31:0]. The LFSR
                                     continues to advance every cycle, so
                                     consecutive reads return different values.
===========  =========  ===========  ===============================================

**Usage Notes:**

* Read does not halt LFSR operation
* One clock cycle latency from read request to data valid
* Returns current LFSR state at time of read
* LFSR advances every cycle, providing new random values

RAND_HIGH Register (Offset 0x04, Read Only)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**Upper 32-bit Random Number Output**

===========  =========  ===========  ===============================================
Bit Field    Access     Reset        Description
===========  =========  ===========  ===============================================
[31:0]       R          0x0          Current upper 32 bits of LFSR state

                                     Reading returns lfsr[63:32]. The LFSR
                                     continues to advance every cycle, so
                                     consecutive reads return different values.
===========  =========  ===========  ===============================================

**Usage Notes:**

* Read does not halt LFSR operation
* One clock cycle latency from read request to data valid
* Can be combined with RAND_LOW for 64-bit random values
* LFSR state changes between consecutive reads of HIGH and LOW

Revision History
----------------

========  ==========  ============  ===============================================
Version   Date        Author        Changes
========  ==========  ============  ===============================================
1.0       2026-05-04  G. Tripi      Initial PRNG documentation
========  ==========  ============  ===============================================
