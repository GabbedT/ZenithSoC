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

    # Signal termination to HTIF
    la t0, tohost
    li t1, 1
    sw t1, 0(t0)

    # Halt after return
1:  wfi
    j 1b
