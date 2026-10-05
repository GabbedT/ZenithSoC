// Prologue stores compete with instruction refills while DDR ready is low.
static volatile unsigned data_area[16] asm("data_area")
    __attribute__((aligned(64))) = {0x12345678};

void main(void)
{
    register unsigned long base asm("x31") = (unsigned long)data_area;
    asm volatile (
        "rvgen_begin:\n"
        "lw x8, 0(x31)\n"
        "sw x8, 4(x31)\n"
        ".rept 64\nnop\n.endr\n"
        "lw x9, 4(x31)\n"
        "rvgen_end:\n"
        : : "r"(base)
        : "x8", "x9", "x18", "x19", "x20", "x21", "x22", "x23",
          "x24", "x25", "x26", "x27", "memory"
    );
}
