ENTRY(_app_start)

MEMORY {
    ddr (rwx) : ORIGIN = 0x8000000, LENGTH = 128M
}

SECTIONS {
    .text : {
        *(.text.init)
        *(.text*)
    } > ddr

    .rodata : { *(.rodata*) *(.srodata) } > ddr

    .data : {
        *(.data*) *(.sdata*)
    } > ddr

    .bss (NOLOAD) : {
        __bss_start = .;
        *(.bss*) *(.sbss*) *(COMMON)
        __bss_end = .;
    } >> ddr

    . = ALIGN(16);
    _end = .;
}