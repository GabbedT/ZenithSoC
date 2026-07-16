/* The full-SoC simulation starts from ROM and executes tests from DDR. */
.section .text.boot, "ax"
.global _boot

_boot:
    li   t0, 0x80000000
    jr   t0

.rept 32
    nop
.endr
