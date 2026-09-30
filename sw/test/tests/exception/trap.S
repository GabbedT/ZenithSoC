.section .text.exception_trap, "ax"
.align 4
.global exception_trap_entry
.type exception_trap_entry, @function

/* All registers used here are caller-saved by the RISC-V ABI. The fault
 * triggers are leaf functions, so ra/sp and every callee-saved register can be
 * left untouched. Avoiding a memory-backed trap frame also exercises repeated
 * traps without depending on store-to-load forwarding in the test itself. */
exception_trap_entry:
    la   t3, trapCount
    lw   t4, 0(t3)
    addi t4, t4, 1
    sw   t4, 0(t3)

    la   t3, failures
    lw   t6, 0(t3)

    csrr t0, mcause
    la   t4, expectedCause
    lw   t4, 0(t4)
    xor  t0, t0, t4
    snez t0, t0
    add  t6, t6, t0

    csrr t0, mepc
    la   t4, expectedEpc
    lw   t4, 0(t4)
    xor  t0, t0, t4
    snez t0, t0
    add  t6, t6, t0

    csrr t0, mstatus
    srli t1, t0, 11
    andi t1, t1, 3
    la   t4, expectedMpp
    lw   t4, 0(t4)
    xor  t1, t1, t4
    snez t1, t1
    add  t6, t6, t1
    srli t0, t0, 3
    andi t0, t0, 1
    add  t6, t6, t0
    sw   t6, 0(t3)

    la   t3, recoveryPc
    lw   t0, 0(t3)
    la   t3, exception_resume_in_machine
    lw   t1, 0(t3)
    bnez t1, 1f
    csrw mepc, t0
    mret
1:  jr   t0

.size exception_trap_entry, .-exception_trap_entry

.section .text.exception_triggers, "ax"
.align 2
.macro EXCEPTION_TRIGGER name, instruction
.global \name
.global \name\()_site
.global \name\()_resume
\name:
\name\()_site:
    \instruction
\name\()_resume:
    ret
.endm

/* A full-width encoding (bits[1:0]=11) with an unallocated major opcode. */
EXCEPTION_TRIGGER exception_illegal, ".word 0x0000007f"
EXCEPTION_TRIGGER exception_breakpoint, "ebreak"
EXCEPTION_TRIGGER exception_load_misaligned, "lw t0, 1(zero)"
EXCEPTION_TRIGGER exception_store_misaligned, "sw zero, 1(zero)"
EXCEPTION_TRIGGER exception_load_access, "lw t0, 0(zero)"
EXCEPTION_TRIGGER exception_store_access, "sw zero, 0(zero)"
EXCEPTION_TRIGGER exception_ecall_u, "ecall"
EXCEPTION_TRIGGER exception_ecall_m, "ecall"

.global exception_enter_user
exception_enter_user:
    csrw mepc, a0
    li   t0, (3 << 11)
    csrc mstatus, t0
    mret
