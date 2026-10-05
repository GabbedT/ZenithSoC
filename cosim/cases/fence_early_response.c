// A 32-byte line produces two DDR acknowledgements, including during writeback.
static volatile unsigned data_area[8192] asm("data_area")
    __attribute__((aligned(64))) = {1};

void main(void)
{
    for (unsigned pass = 0; pass < 4; ++pass) {
        for (unsigned word = 0; word < 16; ++word)
            data_area[word] = 0x12345678u ^ (pass << 16) ^ word;
        asm volatile ("fence rw, rw" ::: "memory");
        for (unsigned word = 0; word < 16; ++word)
            data_area[32 + word] = data_area[word];
    }
}
