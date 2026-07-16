ENTRY(_boot)

MEMORY {
    boot (rx) : ORIGIN = 0x00000000, LENGTH = 16K
}

SECTIONS {
    .text : {
        *(.text.boot)
        *(.text*)
    } > boot
}
