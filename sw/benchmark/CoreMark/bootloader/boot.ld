ENTRY(boot_program)

MEMORY {
    /* Leave the upper 2 KiB of the physical 16 KiB boot RAM for the stack. */
    boot (rwx) : ORIGIN = 0x00000000, LENGTH = 14K
}

SECTIONS {
    .boot : {
        KEEP(*(.boot))
        *(.text*) *(.rodata*)
        *(.data*) *(.bss*) *(COMMON)
    } > boot

    /DISCARD/ : { *(.eh_frame) }

    ASSERT(SIZEOF(.boot) <= LENGTH(boot), "bootloader overlaps boot stack")
}
