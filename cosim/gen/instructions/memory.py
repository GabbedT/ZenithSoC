"""RV32 load/store generators and data-cache corner cases."""

from .common import DATA_BYTES, GeneratorSpec, MEM_BASE_REG, random_register, random_operand


WIDTHS = ("w", "h", "hu", "b", "bu")
ALIGNMENT = {"w": 4, "h": 2, "hu": 2, "b": 1, "bu": 1}
LOAD_OP = {"w": "lw", "h": "lh", "hu": "lhu", "b": "lb", "bu": "lbu"}
STORE_OP = {"w": "sw", "h": "sh", "hu": "sh", "b": "sb", "bu": "sb"}


def generate_memory_access(rng, _label_id):
    width = rng.choice(WIDTHS)
    offset = rng.randrange(0, DATA_BYTES, ALIGNMENT[width])
    address = random_register(rng)
    operand = random_operand(rng)
    # Signed offsets also exercise address adders and immediate sign extension.
    displacement = rng.randrange(-2048, 2048, ALIGNMENT[width])
    op = STORE_OP[width] if rng.random() < 0.5 else LOAD_OP[width]
    return (f"li {address}, {offset - displacement}\n"
            f"add {address}, {address}, {MEM_BASE_REG}\n"
            f"{op} {operand}, {displacement}({address})")


def generate_fence(_rng, _label_id):
    """Dirty one cache line, fence it, then immediately verify it by loads.

    The lockstep checker compares every destination register, so each load is
    self-checking.  Keeping the first load adjacent to FENCE also exercises the
    request-to-busy race window in the RTL flush handshake.
    """

    return (
        "li x5, 0x13579bdf\n"
        "li x6, 0x2468ace0\n"
        f"lw x7, 0({MEM_BASE_REG})\n"
        f"sw x5, 0({MEM_BASE_REG})\n"
        f"sw x6, 4({MEM_BASE_REG})\n"
        f"sw x5, 8({MEM_BASE_REG})\n"
        f"sw x6, 12({MEM_BASE_REG})\n"
        "fence\n"
        f"lw x7, 0({MEM_BASE_REG})\n"
        f"lw x8, 4({MEM_BASE_REG})\n"
        f"lw x9, 8({MEM_BASE_REG})\n"
        f"lw x10, 12({MEM_BASE_REG})"
    )


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

    near_offset = rng.randrange(0, 2048, 64)
    address_reg = random_register(rng)
    usable_data_regs = []
    for _ in range(4):
        usable_data_regs.append(
            random_register(rng, exclude=(address_reg, *usable_data_regs))
        )
    far_store_1, near_load_1, far_store_2, near_load_2 = usable_data_regs
    return (
        f"li {address_reg}, {near_offset + getattr(rng, 'cache_bytes', 4096)}\n"
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


def generate_overlapping_stores(rng, _label_id):
    """Merge older word, younger halfword, and youngest byte at every lane."""
    offset = rng.randrange(0, 2048, 4)
    a = random_register(rng)
    b = random_register(rng, exclude=(a,))
    d = random_register(rng, exclude=(a, b))
    lines = [f"li {a}, {rng.getrandbits(32)}", f"li {b}, {rng.getrandbits(32)}",
             f"sw {a}, {offset}({MEM_BASE_REG})",
             f"sh {b}, {offset + rng.choice((0, 2))}({MEM_BASE_REG})",
             f"sb {a}, {offset + rng.randrange(4)}({MEM_BASE_REG})"]
    for lane in rng.sample(range(4), 4):
        for op in ("lb", "lbu"):
            lines.append(f"{op} {d}, {offset + lane}({MEM_BASE_REG})")
    lines.extend((f"lh {d}, {offset}({MEM_BASE_REG})",
                  f"lhu {d}, {offset + 2}({MEM_BASE_REG})",
                  f"lw {d}, {offset}({MEM_BASE_REG})"))
    return "\n".join(lines)


def generate_eviction_sweep(rng, _label_id):
    """Dirty all available tags of one index, then read them in shuffled order."""
    stride = getattr(rng, "cache_bytes", 4096)
    line = getattr(rng, "line_bytes", 16)
    offset = rng.randrange(0, stride, line)
    tags = list(range(offset, DATA_BYTES - line + 1, stride))
    address = random_register(rng)
    value = random_register(rng, exclude=(address,))
    lines = []
    for tag in tags:
        lines.extend((f"li {address}, {tag}", f"add {address}, {address}, {MEM_BASE_REG}",
                      f"li {value}, {rng.getrandbits(32)}", f"sw {value}, 0({address})",
                      f"sw {value}, {line - 4}({address})"))
    rng.shuffle(tags)
    for tag in tags:
        lines.extend((f"li {address}, {tag}", f"add {address}, {address}, {MEM_BASE_REG}",
                      f"lw {value}, {line - 4}({address})", f"lw {value}, 0({address})"))
    return "\n".join(lines)


def generate_store_pressure(rng, _label_id):
    """Exceed both store-buffer configurations with consecutive partial stores."""
    offset = rng.randrange(0, 1536, 64)
    data = random_register(rng)
    lines = [f"li {data}, {rng.getrandbits(32)}"]
    for i in range(rng.randint(12, 32)):
        width = rng.choice(("w", "h", "b"))
        lane = rng.randrange(0, 4, ALIGNMENT[width])
        lines.append(f"{STORE_OP[width]} {data}, {offset + 4*i + lane}({MEM_BASE_REG})")
    for i in range(32):
        lines.append(f"lw {data}, {offset + 4*i}({MEM_BASE_REG})")
    return "\n".join(lines)


def generate_load_use_address(rng, _label_id):
    """A loaded offset feeds an address, followed by load-to-store forwarding."""
    a = random_register(rng)
    b = random_register(rng, exclude=(a,))
    slot = rng.randrange(0, 2048, 4)
    target = rng.randrange(2048, DATA_BYTES, 4)
    return (f"li {a}, {target}\nsw {a}, {slot}({MEM_BASE_REG})\n"
            f"lw {a}, {slot}({MEM_BASE_REG})\nadd {a}, {a}, {MEM_BASE_REG}\n"
            f"lw {b}, 0({a})\nsw {b}, {slot}({MEM_BASE_REG})\n"
            f"lw {a}, {slot}({MEM_BASE_REG})")


BASIC_GENERATORS = (
    GeneratorSpec("mem", generate_memory_access),
    GeneratorSpec("mem", generate_fence, aliases=("fence",)),
)

CORNER_CASE_GENERATORS = (
    GeneratorSpec("mem", generate_overlapping_stores),
    GeneratorSpec("mem", generate_eviction_sweep),
    GeneratorSpec("mem", generate_store_pressure),
    GeneratorSpec("mem", generate_load_use_address),
    GeneratorSpec("mem", generate_same_cache_line),
    # "memalias" remains as a compatibility alias for older command lines.
    GeneratorSpec("mem", generate_cache_index_alias, aliases=("memalias",)),
    GeneratorSpec("mem", generate_store_load_forwarding),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
