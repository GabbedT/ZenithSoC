/* Linker script for ZenithSoC Virtual Platform
 *
 * Spike loads firmware at 0x80000000.
 * I/O peripherals are at 0x00004000+ (same as real ZenithSoC).
 */

ENTRY(_start)

MEMORY
{
    ROM (rx)  : ORIGIN = 0x80000000, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x80020000, LENGTH = 128K
}

SECTIONS
{
    .text : {
        *(.text.init)
        *(.text)
    } > ROM

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } > ROM

    .data : {
        *(.data)
        *(.data.*)
    } > RAM AT > ROM

    .bss : {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        __bss_end = .;
    } > RAM

    .stack (NOLOAD) : {
        . = . + 0x2000;
        __stack_top = .;
    } > RAM

    .data : {
        tohost = .;
        fromhost = .;
    }

    .htif (NOLOAD) : ALIGN(4) {
        tohost = .;
        . = . + 4;
        fromhost = .;
        . = . + 4;
    } > RAM
}
