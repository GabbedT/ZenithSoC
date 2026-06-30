.section .boot
.global boot_program
.extern boot_sd

boot_program:
    # NC Memory boot rom
    li sp, 0x00012800

    # Call actual bootloader
    call boot_sd

    # Should not return
    unimp