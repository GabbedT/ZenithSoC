ENTRY(_app_start)

MEMORY {
    ddr (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

SECTIONS {
    .text : {
        KEEP(*(.text.init))
        *(.text*)
    } > ddr

    .rodata : { *(.rodata*) *(.srodata*) } > ddr

    .init_array : {
        __init_array_start = .;
        KEEP(*(.init_array*))
        __init_array_end = .;
    } > ddr

    .data : { *(.data*) *(.sdata*) } > ddr

    .tohost : ALIGN(8) {
        KEEP(*(.tohost))
    } > ddr

    .bss (NOLOAD) : ALIGN(4) {
        __bss_start = .;
        *(.bss*) *(.sbss*) *(COMMON)
        __bss_end = .;
    } > ddr

    . = ALIGN(16);
    _end = .;

    /DISCARD/ : { *(.eh_frame) *(.comment) }
}
