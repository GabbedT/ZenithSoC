"""Conditional branch, jump, call, and bounded-loop generators."""

from .common import GeneratorSpec, LOOP_REG, random_register


BRANCH_OPS = ("beq", "bne", "blt", "bge", "bltu", "bgeu")


def generate_conditional_branch(rng, label_id):
    label = f"BR{label_id}"
    op = rng.choice(BRANCH_OPS)
    lines = [f"{op} {random_register(rng)}, {random_register(rng)}, {label}"]
    for _ in range(rng.randint(1, 12)):
        lines.append(f"addi {random_register(rng)}, {random_register(rng)}, {rng.randint(-2048, 2047)}")
    lines.append(f"{label}:")
    return "\n".join(lines)


def generate_forward_jump(rng, label_id):
    label = f"JF{label_id}"
    return f"jal {random_register(rng)}, {label}\nnop\n{label}:"


def generate_known_target_jalr(rng, label_id):
    """Use a materialized target so jalr can never escape the test program."""

    label = f"JC{label_id}"
    target_reg = random_register(rng)
    link_reg = rng.choice((target_reg, "x0", random_register(rng)))
    offset = rng.choice((-2048, -1, 0, 1, 2047))
    lines = [f"la {target_reg}, {label}",
             f"addi {target_reg}, {target_reg}, {min(-offset, 2047)}"]
    if offset == -2048:
        lines.append(f"addi {target_reg}, {target_reg}, 1")
    # For even offsets this makes the effective target odd; JALR must clear bit 0.
    lines.extend((f"ori {target_reg}, {target_reg}, 1",
                  f"jalr {link_reg}, {target_reg}, {offset}",
                  f"addi {target_reg}, x0, -1", f"{label}:"))
    return "\n".join(lines)


def generate_wrong_path(rng, label_id):
    """Taken branches must squash register writes, stores and illegal opcodes."""
    a = random_register(rng)
    b = random_register(rng, exclude=(a,))
    offset = rng.randrange(0, 2048, 4)
    label = f"SQUASH{label_id}"
    return (f"li {a}, {rng.getrandbits(32)}\n"
            f"sw {a}, {offset}(x31)\n"
            f"xor {b}, {a}, {a}\nbeq {b}, x0, {label}\n"
            f"xori {a}, {a}, -1\nsw {a}, {offset}(x31)\n"
            f".word 0xffffffff\n{label}:\nlw {b}, {offset}(x31)")


def generate_diamond(rng, label_id):
    """Reconverging paths have different data and memory effects."""
    a = random_register(rng)
    b = random_register(rng, exclude=(a,))
    offset = rng.randrange(0, 2048, 4)
    return (f"{rng.choice(BRANCH_OPS)} {a}, {b}, THEN{label_id}\n"
            f"add {a}, {a}, {b}\nsw {a}, {offset}(x31)\n"
            f"jal x0, JOIN{label_id}\nTHEN{label_id}:\n"
            f"sub {a}, {a}, {b}\nsb {a}, {offset}(x31)\n"
            f"JOIN{label_id}:\nlw {b}, {offset}(x31)")


def generate_branch_operand_corner(rng, label_id):
    """Exercise signed/unsigned boundaries and equal operands explicitly."""

    label = f"BC{label_id}"
    lhs = random_register(rng)
    rhs = random_register(rng, exclude=(lhs,))
    lhs_value, rhs_value = rng.choice(
        (
            (0, 0),
            (0xFFFFFFFF, 0),
            (0x80000000, 0x7FFFFFFF),
            (0x7FFFFFFF, 0x80000000),
        )
    )
    op = rng.choice(BRANCH_OPS)
    return (
        f"li {lhs}, {lhs_value}\n"
        f"li {rhs}, {rhs_value}\n"
        f"{op} {lhs}, {rhs}, {label}\n"
        f"nop\n"
        f"{label}:"
    )


def generate_bounded_backward_loop(rng, label_id):
    """Cover backward control flow without permitting an infinite program."""

    label = f"LB{label_id}"
    destination = random_register(rng)
    source = random_register(rng)
    return (
        f"li {LOOP_REG}, {rng.randint(1, 4)}\n"
        f"{label}:\n"
        f"addi {destination}, {source}, {rng.randint(-2048, 2047)}\n"
        f"addi {LOOP_REG}, {LOOP_REG}, -1\n"
        f"bne {LOOP_REG}, x0, {label}"
    )


BASIC_GENERATORS = (
    GeneratorSpec("branch", generate_conditional_branch, needs_label=True),
    GeneratorSpec("ctrl", generate_forward_jump, needs_label=True),
    GeneratorSpec("ctrl", generate_known_target_jalr, needs_label=True),
)

CORNER_CASE_GENERATORS = (
    GeneratorSpec("branch", generate_wrong_path, needs_label=True),
    GeneratorSpec("branch", generate_diamond, needs_label=True),
    GeneratorSpec("branch", generate_branch_operand_corner, needs_label=True),
    GeneratorSpec("ctrl", generate_bounded_backward_loop, needs_label=True),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
