"""Regression tests for the modular co-simulation program generator."""

import random
import json
import re
from collections import Counter
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
from instructions.common import DATA_BYTES, SAFE_REGS  # noqa: E402


class GeneratorArchitectureTests(unittest.TestCase):
    def test_invalid_configuration_is_rejected(self):
        for argv in (("--n", "0"), ("--class", "typo"), ("--line-bytes", "17"),
                     ("--cache-bytes", "32768")):
            with self.subTest(argv=argv), self.assertRaises(SystemExit):
                rvgen.parse_args(argv)

    def test_warmup_covers_every_enabled_scenario(self):
        pool = rvgen.build_pool({"arith", "mem", "branch", "ctrl", "float"},
                                rvgen.parse_extensions("rv32imc_zfinx_zba_zbs"))
        counts, scenarios = Counter(), Counter()
        stream = rvgen.generate_instruction_stream(random.Random(12), len(pool), pool, counts,
                                                    scenarios=scenarios)
        self.assertEqual(len(scenarios), len(pool))
        self.assertTrue(all(value == 1 for value in scenarios.values()))
        self.assertNotIn(".word", counts)
        self.assertNotIn(".option", counts)
        self.assertIn("rvgen_block_0", stream)

    def test_random_arithmetic_exercises_zero_and_register_aliases(self):
        rng = random.Random(91)
        blocks = [integer.generate_register_arithmetic(rng, i) for i in range(2000)]
        operands = [re.findall(r"x\d+", block) for block in blocks]
        self.assertTrue(any(rd == "x0" for rd, _, _ in operands))
        self.assertTrue(any(a == "x0" or b == "x0" for _, a, b in operands))
        self.assertTrue(any(rd == a or rd == b for rd, a, b in operands))

    def test_float_rounding_modes_all_reachable(self):
        rng = random.Random(9)
        blocks = "\n".join(floating_point.generate_float_operation(rng, i) for i in range(1000))
        for mode in floating_point.ROUNDING_MODES:
            self.assertIn(", " + mode, blocks)

    def test_manifest_matches_source_and_scenarios(self):
        with tempfile.TemporaryDirectory() as directory:
            output, manifest = Path(directory) / "p.c", Path(directory) / "p.json"
            self.assertEqual(rvgen.main(["--seed", "81", "--n", "80", "--out", str(output),
                                         "--manifest", str(manifest), "--quiet"]), 0)
            metadata = json.loads(manifest.read_text())
            self.assertEqual(metadata["source_sha256"], rvgen.hashlib.sha256(output.read_bytes()).hexdigest())
            self.assertEqual(sum(metadata["scenarios"].values()), 80)
            self.assertIn("rvgen_begin:", output.read_text())

    def test_memory_sequences_stay_in_bounds_for_both_cache_geometries(self):
        # Execute address arithmetic and memory operations, independently of the
        # generator's address formulas, to catch corrupt live address registers.
        for cache_bytes, line_bytes in ((4096, 16), (8192, 32)):
            rng = random.Random(29)
            rng.cache_bytes, rng.line_bytes = cache_bytes, line_bytes
            rng.register_pool = ("x5", "x6", "x7")
            for spec in memory.GENERATORS:
                for iteration in range(100):
                    registers = {f"x{i}": rng.getrandbits(32) for i in range(32)}
                    registers["x31"] = 0x80000000
                    registers["x0"] = 0
                    ram = {}
                    for line in spec.generate(rng, iteration).splitlines():
                        op, *args = line.replace(",", " ").split()
                        if op == "li":
                            registers[args[0]] = int(args[1], 0) & 0xffffffff
                        elif op == "add":
                            registers[args[0]] = (registers[args[1]] + registers[args[2]]) & 0xffffffff
                        elif op != "fence":
                            displacement, base = re.fullmatch(r"(-?\d+)\((x\d+)\)", args[1]).groups()
                            address = registers[base] + int(displacement)
                            width = {"b": 1, "h": 2, "w": 4}[op[1]]
                            self.assertEqual(address % width, 0, line)
                            self.assertGreaterEqual(address, 0x80000000, line)
                            self.assertLessEqual(address + width, 0x80000000 + DATA_BYTES, line)
                            if op.startswith("s"):
                                for lane in range(width):
                                    ram[address + lane] = (registers[args[0]] >> (8*lane)) & 255
                            elif args[0] != "x0":
                                registers[args[0]] = sum(ram.get(address + lane, 0) << (8*lane) for lane in range(width))
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

    def test_fence_generator_is_enabled_by_memory_and_fence_classes(self):
        fence_generator = next(
            spec for spec in memory.GENERATORS if spec.generate.__name__ == "generate_fence"
        )

        fence_block = fence_generator.generate(random.Random(17), 0)
        self.assertIn("sw x5, 0(x31)\n", fence_block)
        self.assertIn("sw x6, 12(x31)\n", fence_block)
        self.assertIn("fence\nlw x7, 0(x31)\n", fence_block)
        self.assertIn("lw x10, 12(x31)", fence_block)
        self.assertTrue(fence_generator.is_enabled({"mem"}, {"i"}))
        self.assertTrue(fence_generator.is_enabled({"fence"}, {"i"}))

    def test_each_cli_class_generates_a_program(self):
        cases = (
            ("arith", "rv32im_zba_zbs_zbb"),
            ("mem", "rv32i"),
            ("fence", "rv32i"),
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
