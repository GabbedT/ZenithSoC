"""Shared types, architectural registers, and interesting operand values."""

from dataclasses import dataclass
from random import Random
from typing import Callable, Optional, Sequence, Tuple


# x30 is dedicated to bounded loops and x31 holds the data-area base address.
# Keeping both out of this pool prevents a random instruction from corrupting
# control-flow state or the address used by later memory instructions.
SAFE_REGS = tuple(f"x{i}" for i in range(5, 30))
LOOP_REG = "x30"
MEM_BASE_REG = "x31"

DATA_BYTES = 32768
CACHE_SIZE = 4096

INTERESTING_INTEGER_VALUES = (
    0x00000000,
    0x00000001,
    0xFFFFFFFF,
    0x80000000,
    0x7FFFFFFF,
    0x0000FFFF,
    0xFFFF0000,
    0x000000FF,
    0x00000080,
    0x40000000,
    0xCAFEBABE,
)

# IEEE-754 binary32 encodings used directly in GPRs by Zfinx.
INTERESTING_FLOAT_VALUES = (
    0x00000000,  # +0
    0x80000000,  # -0
    0x3F800000,  # +1
    0xBF800000,  # -1
    0x7F800000,  # +infinity
    0xFF800000,  # -infinity
    0x7FC00000,  # quiet NaN
    0x7F800001,  # signaling NaN payload
    0x00800000,  # smallest normal
    0x00000001,  # smallest subnormal
    0x40490FDB,  # pi
)

INTERESTING_VALUES = INTERESTING_INTEGER_VALUES + INTERESTING_FLOAT_VALUES


def initial_register_value(rng):
    """Return a deterministic mix of unconstrained and architectural edges."""

    if rng.random() < 0.5:
        return rng.choice(INTERESTING_VALUES)
    return rng.randint(0, 0xFFFFFFFF)


def random_register(rng, exclude: Sequence[str] = ()) -> str:
    """Pick a usable GPR, optionally excluding registers live in a sequence."""

    excluded = set(exclude)
    candidates = [reg for reg in SAFE_REGS if reg not in excluded]
    if not candidates:
        raise ValueError("no safe register remains after exclusions")
    return rng.choice(candidates)


GeneratorFunction = Callable[[Random, int], str]


@dataclass(frozen=True)
class GeneratorSpec:
    """Metadata used by the driver to activate an instruction generator."""

    instruction_class: str
    generate: GeneratorFunction
    required_extension: Optional[str] = None
    needs_label: bool = False
    aliases: Tuple[str, ...] = ()

    def is_enabled(self, classes, extensions) -> bool:
        requested_names = (self.instruction_class, *self.aliases)
        class_enabled = any(name in classes for name in requested_names)
        extension_enabled = (
            self.required_extension is None
            or self.required_extension in extensions
        )
        return class_enabled and extension_enabled
