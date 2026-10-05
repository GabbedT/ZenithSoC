"""Explicit RV32C forms, including legal hints and mixed-width fetch boundaries."""

from .common import GeneratorSpec, random_register


def generate_compressed(rng, label_id):
    a, b = rng.sample(tuple(f"x{i}" for i in range(8, 16)), 2)
    op = rng.choice(("c.add", "c.mv", "c.sub", "c.xor", "c.or", "c.and",
                     "c.addi", "c.li", "c.slli", "c.srli", "c.srai", "c.andi"))
    if op in ("c.addi", "c.li", "c.andi"):
        operand = rng.randint(-32, 31)
    elif op in ("c.slli", "c.srli", "c.srai"):
        operand = rng.randint(1, 31)
    else:
        operand = b
    return f".option push\n.option rvc\n{op} {a}, {operand}\n.option pop"


def generate_fetch_boundary(rng, label_id):
    a = random_register(rng)
    # A 32-bit instruction at PC % 4 == 2 exercises cross-word instruction fetch.
    return (f".balign 4\n.option push\n.option rvc\nc.nop\n.option norvc\n"
            f"addi {a}, {a}, {rng.randint(-2048, 2047)}\n.option rvc\n"
            f"c.j CF{label_id}\nc.li {a}, -1\nCF{label_id}:\n.option pop")


BASIC_GENERATORS = (GeneratorSpec("arith", generate_compressed, required_extension="c"),)
CORNER_CASE_GENERATORS = (
    GeneratorSpec("ctrl", generate_fetch_boundary, required_extension="c", needs_label=True),
)
GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
