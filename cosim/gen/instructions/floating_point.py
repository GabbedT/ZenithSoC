"""RV32 Zfinx single-precision floating-point generators.

Zfinx keeps floating-point operands in integer registers, so all instructions
below intentionally use x-register names.  Division and square root are not
part of either pool.
"""

from .common import GeneratorSpec, INTERESTING_FLOAT_VALUES, random_register


def generate_float_operation(rng, _label_id):
    category = rng.choice(("arithmetic", "plain", "to_int", "to_float", "class"))

    if category == "arithmetic":
        op = rng.choice(("fadd.s", "fsub.s", "fmul.s"))
        return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}, rne"

    if category == "plain":
        op = rng.choice(
            ("fsgnj.s", "fsgnjn.s", "fsgnjx.s", "fmin.s", "fmax.s", "feq.s", "flt.s", "fle.s")
        )
        return f"{op} {random_register(rng)}, {random_register(rng)}, {random_register(rng)}"

    if category == "to_int":
        op = rng.choice(("fcvt.w.s", "fcvt.wu.s"))
        return f"{op} {random_register(rng)}, {random_register(rng)}, rne"

    if category == "to_float":
        op = rng.choice(("fcvt.s.w", "fcvt.s.wu"))
        return f"{op} {random_register(rng)}, {random_register(rng)}, rne"

    if category == "class":
        return f"fclass.s {random_register(rng)}, {random_register(rng)}"

    raise AssertionError(f"unknown floating-point category: {category}")


def generate_float_operand_corner(rng, _label_id):
    """Materialize IEEE-754 edges before a binary operation or comparison."""

    lhs = random_register(rng)
    rhs = random_register(rng, exclude=(lhs,))
    dst = random_register(rng, exclude=(lhs, rhs))
    op = rng.choice(
        (
            "fadd.s",
            "fsub.s",
            "fmul.s",
            "fmin.s",
            "fmax.s",
            "fsgnj.s",
            "fsgnjn.s",
            "fsgnjx.s",
            "feq.s",
            "flt.s",
            "fle.s",
        )
    )
    rounding_mode = ", rne" if op in ("fadd.s", "fsub.s", "fmul.s") else ""
    return (
        f"li {lhs}, {rng.choice(INTERESTING_FLOAT_VALUES)}\n"
        f"li {rhs}, {rng.choice(INTERESTING_FLOAT_VALUES)}\n"
        f"{op} {dst}, {lhs}, {rhs}{rounding_mode}"
    )


def generate_float_conversion_corner(rng, _label_id):
    source = random_register(rng)
    destination = random_register(rng, exclude=(source,))
    op = rng.choice(("fcvt.w.s", "fcvt.wu.s"))
    return (
        f"li {source}, {rng.choice(INTERESTING_FLOAT_VALUES)}\n"
        f"{op} {destination}, {source}, rne"
    )


BASIC_GENERATORS = (
    GeneratorSpec("float", generate_float_operation, required_extension="zfinx"),
)

CORNER_CASE_GENERATORS = (
    GeneratorSpec("float", generate_float_operand_corner, required_extension="zfinx"),
    GeneratorSpec("float", generate_float_conversion_corner, required_extension="zfinx"),
)

GENERATORS = BASIC_GENERATORS + CORNER_CASE_GENERATORS
