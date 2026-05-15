/* Linker script for ZenithSoC Virtual Platform
 *
 * Spike loads firmware at 0x80000000.
 * I/O peripherals are at 0x00004000+ (same as real ZenithSoC).
 */

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

    /* Add space for tohost and fromhost in RAM */
    .tohost : {
        . = . + 4; /* Allocate 4 bytes for the tohost symbol */
    } > RAM

    .fromhost : {
        . = . + 4; /* Allocate 4 bytes for the fromhost symbol */
    } > RAM
}
