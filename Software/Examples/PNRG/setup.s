.section .boot

.global boot_program 

.extern prng 
.extern blink 

boot_program:
    li sp, 0x88000000

    # Enable interrupt 
    li t0, 8
    csrs mstatus, t0 
    li t0, -1
    csrs mie, t0 
    
    la t0, 0x00000054    # Load address of text section start in ROM
    la t1, 0x80000000    # Load address of text section start in RAM
    la t2, 0x00000E3C    # Load address of text section end in ROM

copy_loop:
    lw a0, 0(t0)              # Load a word from ROM
    sw a0, 0(t1)              # Store it in RAM
    addi t0, t0, 4            # Increment ROM pointer
    addi t1, t1, 4            # Increment RAM pointer
    blt t0, t2, copy_loop     # Loop until all text is copied

    # Signal through LED (2)
    la t0, 0x00006000 
    li t1, 2
    sw t1, 0(t0)

    # Jump to main function 
    la t1, prng
    jalr ra, t1, 0 
    unimp 
    