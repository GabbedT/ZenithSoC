"""RV32 load/store generators and data-cache corner cases."""

from .common import CACHE_SIZE, GeneratorSpec, MEM_BASE_REG, random_register


WIDTHS = ("w", "h", "hu", "b", "bu")
ALIGNMENT = {"w": 4, "h": 2, "hu": 2, "b": 1, "bu": 1}
LOAD_OP = {"w": "lw", "h": "lh", "hu": "lhu", "b": "lb", "bu": "lbu"}
STORE_OP = {"w": "sw", "h": "sh", "hu": "sh", "b": "sb", "bu": "sb"}


def generate_memory_access(rng, _label_id):
    width = rng.choice(WIDTHS)
    offset = rng.randrange(0, 1024, ALIGNMENT[width])
    if rng.random() < 0.5:
        return f"{STORE_OP[width]} {random_register(rng)}, {offset}({MEM_BASE_REG})"
    return f"{LOAD_OP[width]} {random_register(rng)}, {offset}({MEM_BASE_REG})"


def generate_same_cache_line(rng, _label_id):
    """Interleave widths and byte lanes inside one 16-byte cache block."""

    block = rng.randrange(0, 1024, 16)
    instructions = []
    for _ in range(rng.randint(3, 6)):
        width = rng.choice(WIDTHS)
        offset = block + rng.randrange(0, 16, ALIGNMENT[width])
        op = STORE_OP[width] if rng.random() < 0.5 else LOAD_OP[width]
        instructions.append(f"{op} {random_register(rng)}, {offset}({MEM_BASE_REG})")
    return "\n".join(instructions)


def generate_cache_index_alias(rng, _label_id):
    """Alternate addresses with the same cache index and different tags."""

    near_offset = rng.randrange(0, 2048, 16)
    address_reg = random_register(rng)
    usable_data_regs = []
    for _ in range(4):
        usable_data_regs.append(
            random_register(rng, exclude=(address_reg, *usable_data_regs))
        )
    far_store_1, near_load_1, far_store_2, near_load_2 = usable_data_regs
    return (
        f"li {address_reg}, {near_offset + CACHE_SIZE}\n"
        f"add {address_reg}, {address_reg}, {MEM_BASE_REG}\n"
        f"sw {far_store_1}, 0({address_reg})\n"
        f"lw {near_load_1}, {near_offset}({MEM_BASE_REG})\n"
        f"sw {far_store_2}, 0({address_reg})\n"
        f"lw {near_load_2}, {near_offset}({MEM_BASE_REG})"
    )


def generate_store_load_forwarding(rng, _label_id):
    """Read an address immediately after stores of different widths."""

    word_offset = rng.randrange(0, 1024, 4)
    store_reg = random_register(rng)
    load_reg = random_register(rng, exclude=(store_reg,))
    byte_lane = rng.randint(0, 3)
    return (
        f"sw {store_reg}, {word_offset}({MEM_BASE_REG})\n"
        f"lb {load_reg}, {word_offset + byte_lane}({MEM_BASE_REG})\n"
        f"lw {load_reg}, {word_offset}({MEM_BASE_REG})"
    )


BASIC_GENERATORS = (
    GeneratorSpec("mem", generate_memory_access),
)

CORNER_CASE_GENERATORS = (
    GeneratorSpec("mem", generate_same_cache_line),
    # "memalias" remains as a compatibility alias for older command lines.
    GeneratorSpec("mem", generate_cache_index_alias, aliases=("memalias",)),
    GeneratorSpec("mem", generate_store_load_forwarding),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
