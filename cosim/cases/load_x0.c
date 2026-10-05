// Legal load: the access must occur even though its result is discarded.
static volatile unsigned data_area[16] asm("data_area")
    __attribute__((aligned(64))) = {0x89abcdef};

void main(void)
{
    register unsigned long base asm("x31") = (unsigned long)data_area;
    asm volatile (
        "rvgen_begin:\n"
        "lw x0, 0(x31)\n"
        "lb x0, 0(x31)\n"
        "lbu x0, 1(x31)\n"
        "lh x0, 0(x31)\n"
        "lhu x0, 2(x31)\n"
        "lw x5, 0(x31)\n"
        "rvgen_end:\n"
        : : "r"(base) : "x5", "memory"
    );
}
