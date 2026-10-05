"""Compile and exercise the architectural coverage collector without Verilator."""

from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

SIM = Path(__file__).resolve().parents[1]


class CoverageTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which("g++"), "requires a C++ compiler")
    def test_dependencies_branches_lanes_and_persistence(self):
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            source = directory / "test.cpp"
            source.write_text(r'''
#include "coverage.h"
#include <cassert>
int main() {
    Coverage c;
    c.instruction(0, 0x00100293, 4, 4, "li", 5, 1);
    c.instruction(4, 0x00528333, 4, 8, "add", 6, 2);
    assert(c.bins["raw.distance.1"] == 2);
    c.instruction(8, 0, 2, 10, "c.li", 5, 0);
    c.instruction(10, 0x00528333, 4, 14, "add", 6, 0);
    assert(c.bins["raw.distance.1"] == 4);
    assert(c.bins["fetch.cross_word"] == 1);
    c.instruction(14, 0x00000063, 4, 40, "beqz", 0, 0);
    c.instruction(40, 0x00000063, 4, 44, "beqz", 0, 0);
    assert(c.bins["branch.funct3.0.taken"] == 1);
    assert(c.bins["branch.funct3.0.not_taken"] == 1);
    c.memory(true, 0x8000100c, 4, 0x80000000, 32768);
    c.memory(false, 0x8000100f, 1, 0x80000000, 32768);
    assert(c.bins["load.overlaps_latest_store"] == 1);
    assert(c.bins["load.bytes.1.lane.3"] == 1);
    assert(c.bins["data.page.1"] == 2);
    assert(c.bins["store.line_end"] == 1);
    assert(c.write("result.cov"));
    assert(!c.write("missing/result.cov"));
}
''')
            subprocess.run(["g++", "-std=c++20", "-I", str(SIM), str(source), "-o", str(directory / "test")],
                           check=True, capture_output=True)
            subprocess.run([str(directory / "test")], cwd=directory, check=True, capture_output=True)
            self.assertIn("retired 6\n", (directory / "result.cov").read_text())
