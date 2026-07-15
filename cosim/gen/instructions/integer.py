"""RV32 integer, multiply/divide, and bit-manipulation generators.

Basic generators cover the ordinary instruction forms.  Corner-case
generators concentrate on immediate limits, shift limits, aliased operands,
and architecturally defined multiply/divide edge behavior.
"""

from .common import (
    GeneratorSpec,
    INTERESTING_INTEGER_VALUES,
    random_register,
)


def generate_register_arithmetic(rng, _label_id):
    op = rng.choice(
        ("add", "sub", "and", "or", "xor", "sll", "srl", "sra", "slt", "sltu")
    )
    return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"


def generate_immediate_arithmetic(rng, _label_id):
    op = rng.choice(("addi", "andi", "ori", "xori", "slti", "sltiu"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {rng.randint(-2048, 2047)}"


def generate_immediate_shift(rng, _label_id):
    op = rng.choice(("slli", "srli", "srai"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {rng.randint(0, 31)}"


def generate_lui(rng, _label_id):
    return f"lui {random_register(rng)}, {rng.randint(0, 0xFFFFF)}"


def generate_multiply_divide(rng, _label_id):
    op = rng.choice(("mul", "mulh", "mulhu", "mulhsu", "div", "divu", "rem", "remu"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"


def generate_zba(rng, _label_id):
    op = rng.choice(("sh1add", "sh2add", "sh3add"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"


def generate_zbs(rng, _label_id):
    op = rng.choice(("bset", "bclr", "binv", "bext"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"


def generate_zbb(rng, _label_id):
    unary_ops = (
        "sext.b",
        "sext.h",
        "zext.h",
        "clz",
        "ctz",
        "cpop",
        "orc.b",
        "rev8",
    )
    op = rng.choice(
        (
            "min",
            "max",
            "minu",
            "maxu",
            "andn",
            "orn",
            "xnor",
            "rol",
            "ror",
            *unary_ops,
        )
    )
    if op in unary_ops:
        return f"{op} {random_register(rng)}, {random_register(rng)}"
    return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"


def generate_immediate_boundary(rng, _label_id):
    op = rng.choice(("addi", "andi", "ori", "xori", "slti", "sltiu"))
    immediate = rng.choice((-2048, -1, 0, 1, 2047))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {immediate}"


def generate_shift_boundary(rng, _label_id):
    op = rng.choice(("slli", "srli", "srai"))
    return f"{op} {random_register(rng)}, {random_register(rng)}, {rng.choice((0, 31))}"


def generate_aliased_operands(rng, _label_id):
    op = rng.choice(("add", "sub", "and", "or", "xor", "sll", "srl", "sra"))
    first = random_register(rng)
    second = random_register(rng, exclude=(first,))
    pattern = rng.choice(("rd-rs1", "rd-rs2", "rs1-rs2"))
    if pattern == "rd-rs1":
        return f"{op} {first}, {first}, {second}"
    if pattern == "rd-rs2":
        return f"{op} {first}, {second}, {first}"
    return f"{op} {second}, {first}, {first}"


def generate_multiply_divide_corner(rng, _label_id):
    lhs = random_register(rng)
    rhs = random_register(rng, exclude=(lhs,))
    dst = random_register(rng, exclude=(lhs, rhs))
    op = rng.choice(("mulh", "mulhu", "mulhsu", "div", "divu", "rem", "remu"))
    lhs_value = rng.choice(INTERESTING_INTEGER_VALUES)
    rhs_value = rng.choice((0, 1, 0xFFFFFFFF, 0x80000000))
    return (
        f"li {lhs}, {lhs_value}\n"
        f"li {rhs}, {rhs_value}\n"
        f"{op} {dst}, {lhs}, {rhs}"
    )


BASIC_GENERATORS = (
    GeneratorSpec("arith", generate_register_arithmetic),
    GeneratorSpec("arith", generate_immediate_arithmetic),
    GeneratorSpec("arith", generate_immediate_shift),
    GeneratorSpec("arith", generate_lui),
    GeneratorSpec("arith", generate_multiply_divide, required_extension="m"),
    GeneratorSpec("arith", generate_zba, required_extension="zba"),
    GeneratorSpec("arith", generate_zbs, required_extension="zbs"),
    GeneratorSpec("arith", generate_zbb, required_extension="zbb"),
)

CORNER_CASE_GENERATORS = (
    GeneratorSpec("arith", generate_immediate_boundary),
    GeneratorSpec("arith", generate_shift_boundary),
    GeneratorSpec("arith", generate_aliased_operands),
    GeneratorSpec("arith", generate_multiply_divide_corner, required_extension="m"),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
