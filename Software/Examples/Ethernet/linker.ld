MEMORY
{
    boot (rx) : ORIGIN = 0x00000000, LENGTH = 2048
    mmio (rw) : ORIGIN = 0x00000800, LENGTH = 128K
    text (rw) : ORIGIN = 0x00020800, LENGTH = 16K
}

SECTIONS
{
    .boot : { 
        *(.boot) 

        setup.o (.boot.boot_program)
    } > boot

    .mmio : { *(.mmio) } > mmio

    .text : { 
        *(.text) 

        ethernet.o (.text.ethernet)
    } > text
}