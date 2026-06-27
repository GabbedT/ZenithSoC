/* Linker script for the user program.
 * Everything is placed in the cacheable DDR region starting at 0x8000_0000.
 * tohost/fromhost are used for HTIF termination, read by Spike and the harness.
 */
ENTRY(_start)

MEMORY
{
    MEM (rwx) : ORIGIN = 0x80000000, LENGTH = 16M
}

SECTIONS
{
    .text : {
        *(.text.init)
        *(.text*)
    } > MEM

    .rodata : {
        *(.rodata*)
        *(.srodata*)
    } > MEM

    .data : {
        *(.data*)
        *(.sdata*)
    } > MEM

    .bss : ALIGN(4) {
        __bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        __bss_end = .;
    } > MEM

    .htif (NOLOAD) : ALIGN(8) {
        PROVIDE(tohost = .);
        . = . + 8;
        PROVIDE(fromhost = .);
        . = . + 8;
    } > MEM

    .stack (NOLOAD) : ALIGN(16) {
        . = . + 0x4000;
        __stack_top = .;
    } > MEM
}