.section .text.init
.global _start

_start:
    la sp, __stack_top

    # Enable machine interrupts (MIE bit in mstatus)
    li t0, 8
    csrs mstatus, t0
    li t0, -1
    csrs mie, t0

    # Jump to main
    call main

    # Halt after return
1:  wfi
    j 1b
