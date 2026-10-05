"""Optional assembler/Spike checks: generated programs must terminate legally."""

import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
COMPILER = shutil.which(os.environ.get("RISCV_GCC", "riscv32-unknown-elf-g++"))
SPIKE = shutil.which(os.environ.get("SPIKE", "spike"))


@unittest.skipUnless(COMPILER and SPIKE, "set PATH/RISCV_GCC and SPIKE for assembler + oracle checks")
class ToolchainTests(unittest.TestCase):
    def test_profiles_terminate_on_spike_with_and_without_compressed(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            for compressed in ("", "c"):
                isa = f"rv32im{compressed}_zfinx_zba_zbs"
                for seed, profile in enumerate(("mixed", "dependencies", "memory", "control", "float")):
                    with self.subTest(isa=isa, seed=seed, profile=profile):
                        subprocess.run([sys.executable, str(ROOT / "gen/rvgen.py"), "--seed", str(seed),
                                        "--n", "100", "--ext", isa, "--class", "arith,mem,branch,ctrl,float",
                                        "--profile", profile, "--cache-bytes", "8192", "--line-bytes", "32",
                                        "--out", str(directory / "prog.c"), "--quiet"], check=True, capture_output=True)
                        subprocess.run([COMPILER, f"-march={isa}", "-mabi=ilp32", "-nostartfiles", "-O2",
                                        "-ffreestanding", "-T", str(ROOT / "sw/link_user.ld"),
                                        str(ROOT / "sw/crt0_user.s"), str(directory / "prog.c"),
                                        "-o", str(directory / "fw.elf")], check=True, capture_output=True)
                        subprocess.run([SPIKE, f"--isa={isa}", "--priv=mu", str(directory / "fw.elf")],
                                       check=True, capture_output=True, timeout=10)
