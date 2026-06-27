ENTRY(_boot)

MEMORY
{
    BOOT (rx) : ORIGIN = 0x00000000, LENGTH = 16K
}

SECTIONS
{
    .text : {
        *(.text.boot)
        *(.text*)
    } > BOOT
}