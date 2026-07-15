"""Conditional branch, jump, call, and bounded-loop generators."""

from .common import GeneratorSpec, LOOP_REG, random_register


BRANCH_OPS = ("beq", "bne", "blt", "bge", "bltu", "bgeu")


def generate_conditional_branch(rng, label_id):
    label = f"BR{label_id}"
    op = rng.choice(BRANCH_OPS)
    return (
        f"{op} {random_register(rng)}, {random_register(rng)}, {label}\n"
        f"nop\n"
        f"{label}:"
    )


def generate_forward_jump(rng, label_id):
    label = f"JF{label_id}"
    return f"jal {random_register(rng)}, {label}\nnop\n{label}:"


def generate_known_target_jalr(rng, label_id):
    """Use a materialized target so jalr can never escape the test program."""

    label = f"JC{label_id}"
    target_reg = random_register(rng)
    link_reg = random_register(rng, exclude=(target_reg,))
    return (
        f"la {target_reg}, {label}\n"
        f"jalr {link_reg}, {target_reg}, 0\n"
        f"nop\n"
        f"{label}:"
    )


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
    GeneratorSpec("branch", generate_branch_operand_corner, needs_label=True),
    GeneratorSpec("ctrl", generate_bounded_backward_loop, needs_label=True),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
