// C.ADDI with a zero immediate is a legal hint, not an illegal instruction.
static volatile unsigned data_area[16] asm("data_area")
    __attribute__((aligned(64))) = {0x12345678};

void main(void)
{
    register unsigned long base asm("x31") = (unsigned long)data_area;
    asm volatile (
        "rvgen_begin:\n"
        "lw x12, 0(x31)\n"
        ".option push\n.option rvc\n"
        "c.addi x12, 0\n"
        ".2byte 0x0005\n"  // C.ADDI x0, 1: legal hint.
        ".2byte 0x107d\n"  // C.ADDI x0, -1: legal hint.
        ".option pop\n"
        "sw x12, 4(x31)\n"
        "rvgen_end:\n"
        : : "r"(base) : "x12", "memory"
    );
}
