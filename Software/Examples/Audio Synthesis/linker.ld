MEMORY
{
    boot (rx) : ORIGIN = 0x00000000, LENGTH = 16K
    mmio (rw) : ORIGIN = 0x00002000, LENGTH = 128M
    text (rw) : ORIGIN = 0x80000000, LENGTH = 128M
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

        audio_synthesizer.o (.text.audio_synthesizer)
    } > text
}