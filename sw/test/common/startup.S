/* Test startup: initialize C++, install the trap handler and report through tohost. */
.section .text.init, "ax"
.global _start

_start:
    la    sp, __stack_top

    /* Clear BSS */
    la    t0, __bss_start
    la    t1, __bss_end
1:  bge   t0, t1, 2f
    sw    zero, 0(t0)
    addi  t0, t0, 4
    j     1b

    /* Run C++ global constructors */
2:  la    s0, __init_array_start
    la    s1, __init_array_end
3:  bge   s0, s1, 4f
    lw    t0, 0(s0)
    jalr  ra, t0
    addi  s0, s0, 4
    j     3b

#ifdef INTERRUPT_VECTORED
    /* Vectored mode selects one table entry for every IO source */
4:  la    t0, interrupt_vector_table
    ori   t0, t0, 1
    csrw  mtvec, t0
#else
    /* Direct mode keeps every device vector in a single C++ dispatcher */
4:  la    t0, interrupt_entry
    csrw  mtvec, t0
#endif

    /* Enable machine external and timer interrupts */
    li    t0, (1 << 11) | (1 << 7)
    csrw  mie, t0

    /* Enable interrupts globally */
    li    t0, (1 << 3)
    csrs  mstatus, t0

    call  main

    /* tohost = (exit_code << 1) | 1 */
    slli  a0, a0, 1
    ori   a0, a0, 1
    la    t0, tohost
    sw    a0, 0(t0)

5:  j     5b


/* Save the interrupted context before entering the C++ dispatcher. */
.section .text.trap, "ax"
.align 4
.global interrupt_entry
.type interrupt_entry, @function

interrupt_entry:
    addi  sp, sp, -128

    sw    x1,   4(sp)
    sw    x3,  12(sp)
    sw    x4,  16(sp)
    sw    x5,  20(sp)
    sw    x6,  24(sp)
    sw    x7,  28(sp)
    sw    x8,  32(sp)
    sw    x9,  36(sp)
    sw    x10, 40(sp)
    sw    x11, 44(sp)
    sw    x12, 48(sp)
    sw    x13, 52(sp)
    sw    x14, 56(sp)
    sw    x15, 60(sp)
    sw    x16, 64(sp)
    sw    x17, 68(sp)
    sw    x18, 72(sp)
    sw    x19, 76(sp)
    sw    x20, 80(sp)
    sw    x21, 84(sp)
    sw    x22, 88(sp)
    sw    x23, 92(sp)
    sw    x24, 96(sp)
    sw    x25, 100(sp)
    sw    x26, 104(sp)
    sw    x27, 108(sp)
    sw    x28, 112(sp)
    sw    x29, 116(sp)
    sw    x30, 120(sp)
    sw    x31, 124(sp)

    csrr  a0, mcause
#ifdef INTERRUPT_VECTORED
    mv    a1, t0
    call  interruptDispatchVectored
#else
    call  interruptDispatch
#endif

    lw    x1,   4(sp)
    lw    x3,  12(sp)
    lw    x4,  16(sp)
    lw    x5,  20(sp)
    lw    x6,  24(sp)
    lw    x7,  28(sp)
    lw    x8,  32(sp)
    lw    x9,  36(sp)
    lw    x10, 40(sp)
    lw    x11, 44(sp)
    lw    x12, 48(sp)
    lw    x13, 52(sp)
    lw    x14, 56(sp)
    lw    x15, 60(sp)
    lw    x16, 64(sp)
    lw    x17, 68(sp)
    lw    x18, 72(sp)
    lw    x19, 76(sp)
    lw    x20, 80(sp)
    lw    x21, 84(sp)
    lw    x22, 88(sp)
    lw    x23, 92(sp)
    lw    x24, 96(sp)
    lw    x25, 100(sp)
    lw    x26, 104(sp)
    lw    x27, 108(sp)
    lw    x28, 112(sp)
    lw    x29, 116(sp)
    lw    x30, 120(sp)
    lw    x31, 124(sp)

    addi  sp, sp, 128
    mret

    /* The current core may fetch the sequential words while MRET is retiring.
     * Keep the alignment padding executable until the redirect is visible. */
    nop
    nop
    nop

.size interrupt_entry, .-interrupt_entry


#ifdef INTERRUPT_VECTORED
/*
 * Each mtvec slot contains exactly one 32-bit jump. The selected shim records
 * the physical slot before entering the context-saving handler above.
 */
.section .text.vector, "ax"
.align 8
.option push
.option norvc
.global interrupt_vector_table

interrupt_vector_table:
    j     interrupt_vector_0
    j     interrupt_vector_1
    j     interrupt_vector_2
    j     interrupt_vector_3
    j     interrupt_vector_4
    j     interrupt_vector_5
    j     interrupt_vector_6
    j     interrupt_vector_7
    j     interrupt_vector_8

interrupt_vector_0:
    li    t0, 0
    j     interrupt_entry

interrupt_vector_1:
    li    t0, 1
    j     interrupt_entry

interrupt_vector_2:
    li    t0, 2
    j     interrupt_entry

interrupt_vector_3:
    li    t0, 3
    j     interrupt_entry

interrupt_vector_4:
    li    t0, 4
    j     interrupt_entry

interrupt_vector_5:
    li    t0, 5
    j     interrupt_entry

interrupt_vector_6:
    li    t0, 6
    j     interrupt_entry

interrupt_vector_7:
    li    t0, 7
    j     interrupt_entry

interrupt_vector_8:
    li    t0, 8
    j     interrupt_entry

.option pop
#endif
