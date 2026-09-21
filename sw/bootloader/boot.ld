ENTRY(boot_program)

MEMORY {
    /* The SoC boot ROM is 16 KiB; keep its upper 2 KiB for the boot stack. */
    boot (rwx) : ORIGIN = 0x00000000, LENGTH = 14K
}

SECTIONS {
    .boot : {
        KEEP(*(.boot))
        *(.text*) *(.rodata*)
        *(.data*) *(.bss*) *(COMMON)
    } > boot

    /DISCARD/ : { *(.eh_frame) *(.comment) }

    ASSERT(SIZEOF(.boot) <= LENGTH(boot), "bootloader overlaps boot stack")
}
