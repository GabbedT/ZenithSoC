"""Instruction generator registry.

Family modules keep instruction knowledge local.  The command-line driver only
needs this flattened registry to select generators by class and ISA extension.
"""

from .branch import GENERATORS as BRANCH_GENERATORS
from .floating_point import GENERATORS as FLOAT_GENERATORS
from .integer import GENERATORS as INTEGER_GENERATORS
from .memory import GENERATORS as MEMORY_GENERATORS
from .compressed import GENERATORS as COMPRESSED_GENERATORS


ALL_GENERATORS = (
    *INTEGER_GENERATORS,
    *MEMORY_GENERATORS,
    *BRANCH_GENERATORS,
    *FLOAT_GENERATORS,
    *COMPRESSED_GENERATORS,
)

# The planner can balance independent instructions against directed sequences.
from . import integer, memory, branch, floating_point, compressed

BASIC_GENERATORS = tuple(spec for module in (integer, memory, branch, floating_point, compressed)
                         for spec in module.BASIC_GENERATORS)
