"""Instruction generator registry.

Family modules keep instruction knowledge local.  The command-line driver only
needs this flattened registry to select generators by class and ISA extension.
"""

from .branch import GENERATORS as BRANCH_GENERATORS
from .floating_point import GENERATORS as FLOAT_GENERATORS
from .integer import GENERATORS as INTEGER_GENERATORS
from .memory import GENERATORS as MEMORY_GENERATORS


ALL_GENERATORS = (
    *INTEGER_GENERATORS,
    *MEMORY_GENERATORS,
    *BRANCH_GENERATORS,
    *FLOAT_GENERATORS,
)
