.section .text.init
.global _app_start

_app_start:
    li    sp, 0x88000000

    la    t0, __bss_start
    la    t1, __bss_end
1:  bgeu  t0, t1, 2f
    sw    zero, 0(t0)
    addi  t0, t0, 4
    j     1b

2:  la    s0, __init_array_start
    la    s1, __init_array_end
3:  bgeu  s0, s1, 4f
    lw    t0, 0(s0)
    jalr  ra, t0
    addi  s0, s0, 4
    j     3b

4:  call  main

5:  wfi
    j     5b