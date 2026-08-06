.section .boot
.global boot_program
.extern boot_sd

boot_program:
    # Keep the boot stack in the upper part of the local 16 KiB boot RAM.
    # The system CPU is intentionally allowed to start before DDR calibration,
    # so touching DDR here could deadlock before the first UART diagnostic.
    li sp, 0x00004000

    # Call actual bootloader
    call boot_sd

    # Should not return
    unimp
