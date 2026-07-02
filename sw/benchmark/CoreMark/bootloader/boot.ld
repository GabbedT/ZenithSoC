ENTRY(boot_program)

MEMORY {
    boot (rx) : ORIGIN = 0x00000000, LENGTH = 16K
}

SECTIONS {
    .boot : {
        KEEP(*(.boot))
        *(.text*) *(.rodata*)
        *(.data*) *(.bss*) *(COMMON)
    } > boot

    /DISCARD/ : { *(.eh_frame) }
}