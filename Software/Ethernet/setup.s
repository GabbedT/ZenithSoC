.section .boot

.global boot_program 

.extern eth 

boot_program:
    li sp, 0x24800

    # Enable interrupt 
    li t0, 8
    csrs mstatus, t0 
    li t0, -1
    csrs mie, t0 
    
    # Jump to main function 
    jal ra, eth 
    unimp 
    