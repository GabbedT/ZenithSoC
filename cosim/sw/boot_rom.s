# This stub simply jumps to the user program entry at 0x8000_0000.

.section .text.boot, "ax"
.global _boot

_boot:
    li   t0, 0x80000000
    jr   t0

.rept 32
nop
.endr