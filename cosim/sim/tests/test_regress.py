"""Fault-injection checks for campaign status, isolation and retained artifacts."""

import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
import regress


class RegressionTests(unittest.TestCase):
    def test_zero_exit_without_completion_is_not_pass(self):
        self.assertFalse(regress.passed(0, "[COSIM] STOP - reached max_retire=10"))
        self.assertFalse(regress.passed(1, "[COSIM] PASS - 10 instructions"))
        self.assertTrue(regress.passed(0, "[COSIM] PASS - 10 instructions"))

    def test_timeout_and_missing_executable_fail(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            self.assertEqual(regress.execute(["/nonexistent/cosim-tool"], directory, "missing.log", 1), 127)
            self.assertEqual(regress.execute([sys.executable, "-c", "import time; time.sleep(10)"],
                                             directory, "timeout.log", 0.05), 124)
            self.assertIn("INCOMPLETE", (directory / "timeout.log").read_text())

    def test_campaign_propagates_failures_and_preserves_only_failure_firmware(self):
        for mode in ("pass", "compile-fail", "incomplete", "mismatch"):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as temp:
                root = Path(temp)
                compiler = root / "compiler.py"
                compiler.write_text("from pathlib import Path\nimport sys\n"
                                    + ("sys.exit(2)\n" if mode == "compile-fail" else "Path('fw.elf').write_text('test ELF')\n"))
                simulator = root / "simulator"
                simulator.write_text(f"#!{sys.executable}\nfrom pathlib import Path\nimport sys\n"
                                     "Path('executed.cov').write_text('retired 7\\nopcode.add 7\\n')\n"
                                     + ("print('[COSIM] PASS - 7 instructions')\n" if mode == "pass" else
                                        "print('[COSIM] STOP')\n" if mode == "incomplete" else "sys.exit(1)\n"))
                simulator.chmod(0o755)
                (root / "boot.elf").write_text("test boot")
                result = subprocess.run([sys.executable, str(ROOT / "regress.py"), "--seeds", "2", "--jobs", "2",
                                         "--length", "2", "--cc", f"{sys.executable} {compiler}",
                                         "--objdump", "/bin/true", "--simulator", str(simulator),
                                         "--boot", str(root / "boot.elf"), "--out", str(root / "runs"),
                                         "--coverage-dir", str(root / "cov")], capture_output=True, text=True)
                self.assertEqual(result.returncode, 0 if mode == "pass" else 1, result.stderr)
                campaign, = (root / "runs").iterdir()
                summary = json.loads((campaign / "summary.json").read_text())
                self.assertEqual(summary["passed"], 2 if mode == "pass" else 0)
                self.assertEqual((campaign / "seed0/fw.elf").exists(), mode in ("incomplete", "mismatch"))
                if mode == "compile-fail":
                    self.assertFalse((campaign / "seed0/simulate.log").exists())
                if mode == "pass":
                    self.assertEqual(regress.read_counts(root / "cov/regress.cov")["retired"], 14)
                else:
                    self.assertEqual(regress.read_counts(root / "cov/regress.cov")["retired"], 0)


if __name__ == "__main__":
    unittest.main()
