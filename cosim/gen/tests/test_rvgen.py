"""Regression tests for the modular co-simulation program generator."""

import random
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


GEN_DIR = Path(__file__).resolve().parents[1]
RVGEN = GEN_DIR / "rvgen.py"
sys.path.insert(0, str(GEN_DIR))

import rvgen  # noqa: E402
from instructions import branch, floating_point, integer, memory  # noqa: E402
from instructions.common import SAFE_REGS  # noqa: E402


class GeneratorArchitectureTests(unittest.TestCase):
    def test_campaign_seed_is_deterministic_and_changes_the_stream(self):
        self.assertEqual(rvgen.combined_seed(17, 0), 17)
        self.assertEqual(
            rvgen.combined_seed(17, 1234),
            rvgen.combined_seed(17, 1234),
        )
        self.assertNotEqual(
            rvgen.combined_seed(17, 1234),
            rvgen.combined_seed(17, 1235),
        )

    def test_cli_repeats_a_campaign_but_changes_between_campaigns(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            outputs = [Path(temp_dir) / f"campaign-{index}.c" for index in range(3)]
            generation_seeds = (1234, 1234, 5678)
            for output, generation_seed in zip(outputs, generation_seeds):
                subprocess.run(
                    (
                        sys.executable,
                        str(RVGEN),
                        "--seed",
                        "17",
                        "--gen-seed",
                        str(generation_seed),
                        "--n",
                        "100",
                        "--out",
                        str(output),
                    ),
                    check=True,
                    capture_output=True,
                    text=True,
                )

            programs = [output.read_text().splitlines()[2:] for output in outputs]
            self.assertEqual(programs[0], programs[1])
            self.assertNotEqual(programs[0], programs[2])

    def test_every_family_exposes_basic_and_corner_case_generators(self):
        for module in (integer, memory, branch, floating_point):
            self.assertTrue(module.BASIC_GENERATORS, module.__name__)
            self.assertTrue(module.CORNER_CASE_GENERATORS, module.__name__)

    def test_reserved_registers_are_not_random_operands(self):
        self.assertNotIn("x30", SAFE_REGS)
        self.assertNotIn("x31", SAFE_REGS)

    def test_isa_extension_parser(self):
        self.assertEqual(
            rvgen.parse_extensions("rv32imc_zfinx_zba_zicsr"),
            {"i", "m", "c", "zfinx", "zba", "zicsr"},
        )

    def test_float_generators_only_emit_supported_operations(self):
        rng = random.Random(17)
        generated = []
        for spec in floating_point.GENERATORS:
            generated.extend(spec.generate(rng, 0) for _ in range(500))
        text = "\n".join(generated)
        self.assertNotIn("fdiv", text)
        self.assertNotIn("fsqrt", text)
        self.assertNotIn("fmadd", text)
        self.assertNotIn("fmsub", text)
        self.assertNotIn("fnmadd", text)
        self.assertNotIn("fnmsub", text)

    def test_each_cli_class_generates_a_program(self):
        cases = (
            ("arith", "rv32im_zba_zbs_zbb"),
            ("mem", "rv32i"),
            ("branch,ctrl", "rv32i"),
            ("float", "rv32im_zfinx"),
        )
        with tempfile.TemporaryDirectory() as temp_dir:
            for index, (classes, isa) in enumerate(cases):
                output = Path(temp_dir) / f"program-{index}.c"
                coverage = Path(temp_dir) / f"program-{index}.cov"
                subprocess.run(
                    (
                        sys.executable,
                        str(RVGEN),
                        "--seed",
                        str(index),
                        "--n",
                        "100",
                        "--class",
                        classes,
                        "--ext",
                        isa,
                        "--out",
                        str(output),
                        "--cov-out",
                        str(coverage),
                    ),
                    check=True,
                    capture_output=True,
                    text=True,
                )
                self.assertIn("void main(void)", output.read_text())
                self.assertTrue(coverage.read_text().strip())


if __name__ == "__main__":
    unittest.main()
