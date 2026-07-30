.section .text.init
.global _app_start

.macro UART_MARKER character
    li    t2, 0x00004000
.Luart_wait_\@:
    lw    t3, 0(t2)
    andi  t3, t3, 8
    bnez  t3, .Luart_wait_\@
    li    t3, \character
    sb    t3, 4(t2)
.endm

_app_start:
    li    sp, 0x88000000
    UART_MARKER 'A'

    la    t0, __bss_start
    la    t1, __bss_end
1:  bgeu  t0, t1, 2f
    sw    zero, 0(t0)
    addi  t0, t0, 4
    j     1b

2:  UART_MARKER 'B'

    la    s0, __init_array_start
    la    s1, __init_array_end
3:  bgeu  s0, s1, 4f
    lw    t0, 0(s0)
    jalr  ra, t0
    addi  s0, s0, 4
    j     3b

4:  UART_MARKER 'C'

    call  main

5:  wfi
    j     5b
