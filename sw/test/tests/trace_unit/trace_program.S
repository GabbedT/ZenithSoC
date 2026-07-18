/*
 * Short, deterministic control-flow region for the Trace Unit IO test.
 * Keeping RVC disabled makes every sequential PC step exactly four bytes.
 */
.section .text.trace_program, "ax"
.option push
.option norvc

.global trace_program
.global trace_start
.global trace_loop
.global trace_loop_branch
.global trace_taken_branch
.global trace_taken
.global trace_call
.global trace_forward
.global trace_finish_jump
.global trace_finish
.type trace_program, @function

/* a0: control register, a1: tracing control, a2: serializer drain control */
trace_program:
    sw      a1, 0(a0)
    fence   iorw, iorw

    /* Let the MMIO write become visible before reaching the PC trigger. */
    .rept 16
    nop
    .endr

trace_start:
    addi    t0, zero, 3
    addi    t1, zero, 0

trace_loop:
    addi    t1, t1, 1
trace_loop_branch:
    blt     t1, t0, trace_loop

trace_taken_branch:
    beq     t1, t0, trace_taken
    addi    t2, zero, 0x55

trace_taken:
    nop
trace_call:
    jal     t2, trace_forward
    addi    t3, zero, 0x66

trace_forward:
    addi    t3, zero, 0x77
trace_finish_jump:
    jal     zero, trace_finish
    addi    t4, zero, 0x88

trace_finish:
    sw      a2, 0(a0)
    fence   iorw, iorw

    /* The return must retire after tracing has been disabled. */
    .rept 4
    nop
    .endr
    ret

.size trace_program, .-trace_program
.option pop
