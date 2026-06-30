.section .text.init
.global _app_start

_app_start:
    li   sp, 0x88000000

    la   t0, __bss_start
    la   t1, __bss_end
1:  bgeu t0, t1, 2f
    sw   zero, 0(t0)
    addi t0, t0, 4
    j    1b

2:
    call main

3:  wfi
    j 3b