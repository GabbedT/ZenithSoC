#!/usr/bin/env python3
"""Report retired coverage and explicit holes; never equate generation with execution."""

import argparse
from pathlib import Path
from regress import read_counts


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("path", type=Path)
    args = parser.parse_args()
    if not args.path.exists():
        parser.error(f"coverage file missing: {args.path}")
    counts = read_counts(args.path)
    print(f"Successfully compared test-body instructions from passing runs: {counts['retired']}")
    print("Opcode names are Spike disassembly mnemonics (including aliases).")
    for key, value in sorted(counts.items()):
        print(f"{key:44s} {value}")
    expected = {"source.x0", "destination.x0", "alias.rd_rs1", "alias.rd_rs2",
                "fetch.cross_word", "jump.indirect", "load.overlaps_latest_store",
                "load.line_end", "store.line_end"}
    expected.update(f"raw.distance.{distance}" for distance in range(1, 5))
    expected.update(f"branch.funct3.{funct}.{outcome}" for funct in (0, 1, 4, 5, 6, 7)
                    for outcome in ("taken", "not_taken"))
    expected.update(f"data.page.{page}" for page in range(8))
    expected.update(f"{op}.bytes.{size}.lane.{lane}" for op in ("load", "store")
                    for size in (1, 2, 4) for lane in range(0, 4, size))
    print("\nUnhit targets (some do not apply to restricted classes/ISAs):")
    for key in sorted(expected):
        if not counts[key]:
            print(f"  {key}")


if __name__ == "__main__":
    main()
