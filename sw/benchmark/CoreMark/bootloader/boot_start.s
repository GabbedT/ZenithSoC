.section .boot
.global boot_program
.extern boot_sd

boot_program:
    # Use DDR for the stack: 0x00012800 is in the MMIO address space.
    li sp, 0x88000000

    # Call actual bootloader
    call boot_sd

    # Should not return
    unimp
