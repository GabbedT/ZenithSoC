# No interrupts are used in this co-simulation. The code initializes the stack
# pointer, clears the .bss section, calls main(), then terminates through HTIF
# by writing 1 to tohost.

.section .text.init
.global _start

_start:
    la   sp, __stack_top

/* Clear the .bss section */
    la   t0, __bss_start
    la   t1, __bss_end

1:
    bge  t0, t1, 2f
    sw   zero, 0(t0)
    addi t0, t0, 4
    j    1b

2:
    call main

/* Terminate through HTIF: tohost = 1 means pass */
    la   t0, tohost
    li   t1, 1
    sw   t1, 0(t0)

3:
    j    3b