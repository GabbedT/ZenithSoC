.section .text.priv_triggers, "ax"
.align 2

.macro PRIV_TRIGGER name, instruction
.global \name
.global \name\()_site
.global \name\()_resume
\name:
\name\()_site:
    \instruction
\name\()_resume:
    ret
.endm

PRIV_TRIGGER priv_machine_csr, "csrr t0, mstatus"
PRIV_TRIGGER priv_wfi, "wfi"
PRIV_TRIGGER priv_mret, "mret"
PRIV_TRIGGER priv_cycle_blocked, "csrr t0, cycle"

.global priv_read_cycle
priv_read_cycle:
    csrr a0, cycle
    ret
