// Exercise ties, signed zero, overflow and subnormals in every static mode.
static volatile unsigned data_area[8192] asm("data_area")
    __attribute__((aligned(64))) = {1};

#define CONVERT(mode) \
    asm volatile ( \
        "fcvt.w.s %0, %1, " #mode "\n" \
        "fcvt.wu.s %0, %1, " #mode "\n" \
        "fcvt.s.w %0, %1, " #mode "\n" \
        "fcvt.s.wu %0, %1, " #mode "\n" \
        : "=&r"(result) : "r"(a));

#define ARITHMETIC(mode) \
    asm volatile ( \
        "fadd.s %0, %1, %2, " #mode "\n" \
        "fsub.s %0, %1, %2, " #mode "\n" \
        "fmul.s %0, %1, %2, " #mode "\n" \
        : "=&r"(result) : "r"(a), "r"(b));

void main(void)
{
    static const unsigned operands[] = {
        0, 0x80000000, 1, 0x80000001, 0x007fffff, 0x00800000,
        0x3effffff, 0x3f000000, 0x3f000001, 0xbf000000,
        0x3f800000, 0xbf800000, 0x3fc00000, 0xbfc00000,
        0x40200000, 0xc0200000, 0x4effffff, 0x4f000000,
        0xcf000000, 0xcf000001, 0x4f7fffff, 0x4f800000,
        0x7f7fffff, 0xff7fffff, 0x7f800000, 0xff800000,
        0x7fc00000, 0x7f800001, 0x01000001, 0x02000003,
    };
    unsigned result;

    for (unsigned i = 0; i < sizeof(operands) / sizeof(operands[0]); ++i) {
        unsigned a = operands[i];
        CONVERT(rne) CONVERT(rtz) CONVERT(rdn) CONVERT(rup) CONVERT(rmm)
        for (unsigned j = 0; j < sizeof(operands) / sizeof(operands[0]); ++j) {
            unsigned b = operands[j];
            ARITHMETIC(rne) ARITHMETIC(rtz) ARITHMETIC(rdn)
            ARITHMETIC(rup) ARITHMETIC(rmm)
            data_area[i * 32 + j] = result;
        }
    }
}
