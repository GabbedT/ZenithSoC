ENTRY(_start)

MEMORY {
    ddr (rwx) : ORIGIN = 0x80000000, LENGTH = 64M
}

SECTIONS {
    .text : {
        *(.text.init)
        *(.text.trap)
        *(.text*)
    } > ddr

    .rodata : { *(.rodata*) *(.srodata*) } > ddr

    .init_array : {
        __init_array_start = .;
        *(.init_array*)
        __init_array_end = .;
    } > ddr

    .data : { *(.data*) *(.sdata*) } > ddr

    .bss (NOLOAD) : ALIGN(4) {
        __bss_start = .;
        *(.bss*) *(.sbss*) *(COMMON)
        __bss_end = .;
    } > ddr

    /* HTIF termination symbols read by the Verilator harness */
    .htif (NOLOAD) : ALIGN(8) {
        PROVIDE(tohost = .); . = . + 8;
        PROVIDE(fromhost = .); . = . + 8;
    } > ddr

    .stack (NOLOAD) : ALIGN(16) {
        . = . + 0x8000;
        __stack_top = .;
    } > ddr
}
