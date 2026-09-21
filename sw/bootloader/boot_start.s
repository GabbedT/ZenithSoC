.section .boot
.global boot_program
.global boot_trap_entry
.extern boot_sd
.extern boot_trap_handler

boot_program:
    # The upper 2 KiB of the 16 KiB boot RAM are reserved for this stack.
    # DDR is intentionally not touched until the SD image is ready.
    li sp, 0x00004000

    # Keep traps out of the reset vector.  Without this, mtvec remains zero
    # and an application exception silently restarts the bootloader.
    la t0, boot_trap_entry
    csrw mtvec, t0

    call boot_sd

    # boot_sd transfers control to the application and must not return.
    unimp

.balign 4
boot_trap_entry:
    # Capture the architectural diagnostics before using any C code.  Switch
    # back to the private boot stack because the application stack may be the
    # reason for the exception.
    csrr a0, mcause
    csrr a1, mepc
    li   sp, 0x00004000
    call boot_trap_handler

1:  wfi
    j 1b
