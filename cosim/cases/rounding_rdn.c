// floor(1.5) is 1; silently applying round-to-nearest would produce 2.
static volatile unsigned data_area[16] asm("data_area")
    __attribute__((aligned(64))) = {0x3fc00000};

void main(void)
{
    register unsigned long base asm("x31") = (unsigned long)data_area;
    asm volatile (
        "rvgen_begin:\n"
        "lw x5, 0(x31)\n"
        "fcvt.w.s x6, x5, rdn\n"
        "sw x6, 4(x31)\n"
        "rvgen_end:\n"
        : : "r"(base) : "x5", "x6", "memory"
    );
}
