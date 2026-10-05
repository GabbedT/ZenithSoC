#!/usr/bin/env python3
"""Run isolated, reproducible campaigns; retain failures and aggregate real coverage."""

import argparse
from collections import Counter
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import shlex
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent


def memory_seed(seed, generation_seed):
    material = f"memory:{generation_seed}:{seed}".encode()
    return int.from_bytes(hashlib.sha256(material).digest()[:4], "little")


def execute(command, directory, log_name, timeout):
    """No shell: a failed stage cannot fall through to a stale executable."""
    with (directory / log_name).open("w") as log:
        try:
            result = subprocess.run(command, cwd=directory, stdout=log,
                                    stderr=subprocess.STDOUT, timeout=timeout)
            return result.returncode
        except subprocess.TimeoutExpired:
            log.write(f"\n[COSIM] INCOMPLETE: wall timeout ({timeout}s)\n")
            return 124
        except OSError as error:
            log.write(f"\nCannot execute {command[0]}: {error}\n")
            return 127


def read_counts(path):
    counts = Counter()
    if path.exists():
        for line in path.read_text().splitlines():
            key, value = line.split()
            counts[key] += int(value)
    return counts


def write_counts(path, counts):
    path.write_text("".join(f"{key} {value}\n" for key, value in sorted(counts.items())))


def passed(returncode, log):
    return returncode == 0 and any(line.startswith("[COSIM] PASS - ") for line in log.splitlines())


def run_seed(args, campaign, seed):
    directory = campaign / f"seed{seed}"
    directory.mkdir()
    # Independent stimulus and timing seeds allow a failing ELF to be replayed
    # under different memory schedules without changing its instruction stream.
    timing_seed = memory_seed(seed, args.gen_seed)
    generator = [sys.executable, str(ROOT / "gen/rvgen.py"), "--seed", str(seed),
                 "--gen-seed", str(args.gen_seed), "--n", str(args.length),
                 "--class", args.classes, *shlex.split(args.gen_flags),
                 "--out", "prog.c", "--manifest", "generator.json", "--cov-out", "generated.cov", "--quiet"]
    compiler = [*shlex.split(args.cc), *shlex.split(args.cflags), "-fno-use-cxa-atexit",
                "-fno-exceptions", "-nostartfiles", "-O2", "-ffreestanding", "-T",
                str(ROOT / "sw/link_user.ld"), str(ROOT / "sw/crt0_user.s"), "prog.c", "-o", "fw.elf"]
    simulator = [str(Path(args.simulator).resolve()), "+firmware=fw.elf", "+boot=boot.elf", "+notrace",
                 "+coverage=executed.cov", "+rtl_coverage=coverage.dat",
                 f"+mem_seed={timing_seed}", *shlex.split(args.sim_flags)]
    metadata = {"seed": seed, "gen_seed": args.gen_seed, "memory_seed": timing_seed,
                "commands": [generator, compiler, simulator], "timeout": args.timeout}
    shutil.copyfile(args.boot, directory / "boot.elf")
    stages = [("generate", generator), ("compile", compiler)]
    if args.spike:
        flags = shlex.split(args.gen_flags)
        isa = flags[flags.index("--ext") + 1]
        privilege = flags[flags.index("--priv") + 1] if "--priv" in flags else "mu"
        metadata["reference_command"] = [args.spike, f"--isa={isa}", f"--priv={privilege}", "fw.elf"]
        stages.append(("reference", metadata["reference_command"]))
    stages.append(("simulate", simulator))
    for stage, command in stages:
        code = execute(command, directory, stage + ".log", args.timeout)
        if code or (stage == "simulate" and not passed(code, (directory / "simulate.log").read_text())):
            metadata.update(status="FAIL", stage=stage, returncode=code)
            break
    else:
        metadata.update(status="PASS", stage="simulate", returncode=0)
    (directory / "result.json").write_text(json.dumps(metadata, indent=2) + "\n")
    if metadata["status"] == "FAIL":
        execute([*shlex.split(args.objdump), "-d", "fw.elf"], directory, "firmware.dis", args.timeout)
    elif not args.keep_pass:
        for name in ("prog.c", "fw.elf", "boot.elf"):
            (directory / name).unlink()
    print(f"seed {seed} : {metadata['status']} ({metadata['stage']}; {directory})", flush=True)
    return directory, metadata


def replay(args):
    directory = Path(args.replay).resolve()
    metadata = json.loads((directory / "result.json").read_text())
    command = metadata["commands"][2]
    if not (directory / "fw.elf").exists():
        raise ValueError("replay requires a retained firmware ELF (failed run or --keep-pass)")
    if args.trace:
        command = [arg for arg in command if arg != "+notrace"] + ["+trace=replay.fst", "+trace_insns"]
    code = execute(command, directory, "replay.log", args.timeout)
    print((directory / "replay.log").read_text())
    return 0 if passed(code, (directory / "replay.log").read_text()) else 1


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--seeds", type=int, default=16)
    parser.add_argument("--start-seed", type=int, default=0)
    parser.add_argument("--gen-seed", type=int, default=0)
    parser.add_argument("--print-memory-seed", action="store_true")
    parser.add_argument("--length", type=int, default=2000)
    parser.add_argument("--classes", default="arith,mem,branch,ctrl,float")
    parser.add_argument("--jobs", type=int, default=4)
    parser.add_argument("--timeout", type=float, default=120)
    parser.add_argument("--cc", default="riscv32-unknown-elf-g++")
    parser.add_argument("--objdump", default="riscv32-unknown-elf-objdump")
    parser.add_argument("--cflags", default="-march=rv32imc_zfinx_zba_zbs_zicsr -mabi=ilp32 -mno-fdiv")
    parser.add_argument("--gen-flags", default="--ext rv32imc_zfinx_zba_zbs_zicsr")
    parser.add_argument("--sim-flags", default="+mem_random=1")
    parser.add_argument("--simulator", default=str(ROOT / "obj_dir/Vcosim_top"))
    parser.add_argument("--spike", help="optionally validate termination on standalone Spike before the DUT")
    parser.add_argument("--boot", type=Path, default=ROOT / "out/boot.elf")
    parser.add_argument("--out", type=Path, default=ROOT / "out/rtt")
    parser.add_argument("--coverage-dir", type=Path, default=ROOT / "out/cov")
    parser.add_argument("--keep-pass", action="store_true")
    parser.add_argument("--replay", help="directory of a retained seed")
    parser.add_argument("--trace", action="store_true", help="enable waveform and instruction trace on replay")
    args = parser.parse_args(argv)
    if args.print_memory_seed:
        print(memory_seed(args.start_seed, args.gen_seed))
        return 0
    if min(args.seeds, args.jobs, args.length, args.timeout) <= 0:
        parser.error("seeds, jobs, length and timeout must be positive")
    if args.replay:
        return replay(args)
    if args.spike:
        args.spike = shutil.which(args.spike) or str(Path(args.spike).resolve())
    args.out.mkdir(parents=True, exist_ok=True)
    args.coverage_dir.mkdir(parents=True, exist_ok=True)
    stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%SZ")
    campaign = Path(tempfile.mkdtemp(prefix=f"campaign-{args.gen_seed}-{stamp}-", dir=args.out)).resolve()
    print(f"Campaign: {campaign}", flush=True)
    (campaign / "config.json").write_text(json.dumps(vars(args), indent=2, default=str) + "\n")
    # The exact simulator is part of a replay, even after another configuration is built.
    simulator_copy = campaign / "Vcosim_top"
    shutil.copy2(args.simulator, simulator_copy)
    args.simulator = str(simulator_copy)
    totals = {name: Counter() for name in ("regress", "failed", "generated", "scenarios")}
    results = []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = [pool.submit(run_seed, args, campaign, seed)
                   for seed in range(args.start_seed, args.start_seed + args.seeds)]
        for future in as_completed(futures):
            directory, metadata = future.result()
            results.append(metadata)
            success = metadata["status"] == "PASS"
            totals["regress" if success else "failed"].update(read_counts(directory / "executed.cov"))
            if success:
                totals["generated"].update(read_counts(directory / "generated.cov"))
                totals["scenarios"].update(json.loads((directory / "generator.json").read_text())["scenarios"])
    for name, counts in totals.items():
        write_counts(campaign / f"{name}.cov", counts)
        write_counts(args.coverage_dir / f"{name}.cov", counts)
    summary = {"passed": sum(result["status"] == "PASS" for result in results),
               "failed": sum(result["status"] == "FAIL" for result in results),
               "results": sorted(results, key=lambda result: result["seed"])}
    (campaign / "summary.json").write_text(json.dumps(summary, indent=2) + "\n")
    print(f"{summary['passed']} PASS, {summary['failed']} FAIL; artifacts: {campaign}")
    return int(summary["failed"] != 0)


if __name__ == "__main__":
    sys.exit(main())
