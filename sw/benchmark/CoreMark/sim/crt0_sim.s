/* CoreMark sim startup: stack, BSS clear, C++ ctors, main, tohost exit */
.section .text.init
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

    /* Run C++ global constructors (use s0/s1: callee-saved, survives ctor calls) */
2:  la    s0, __init_array_start
    la    s1, __init_array_end
3:  bge   s0, s1, 4f
    lw    t0, 0(s0)
    jalr  ra, t0
    addi  s0, s0, 4
    j     3b

4:  call  main

    /* Signal completion to the testbench via tohost */
    la    t0, tohost
    li    t1, 1
    sw    t1, 0(t0)

5:  j     5b