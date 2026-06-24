#!/usr/bin/env python3

import argparse
import random
import sys

# Parse extensions from the ISA string
def parse_ext(isa: str) -> set:
    isa = isa.lower()
    exts = set()

    # Base single-letter extensions after rv32
    body = isa
    if body.startswith("rv32"):
        body = body[4:]

    # Single-letter extensions up to the first '_'
    head = body.split("_")[0]
    for ch in head:
        exts.add(ch)           # i, m, c, a, f, ...

    # Multi-letter extensions
    # (zfinx, zba, zbs, zbb, zicsr, ...)
    for tok in body.split("_")[1:]:
        if tok:
            exts.add(tok)

    return exts


# Usable GPRs
SAFE_REGS = [f"x{i}" for i in range(5, 32)]  # x5..x31

def rreg(rng):
    return rng.choice(SAFE_REGS)


# Instruction families.
def g_arith_i(rng):
    op = rng.choice([
        "add", "sub", "and", "or", "xor",
        "sll", "srl", "sra", "slt", "sltu"
    ])
    return f"{op} {rreg(rng)}, {rreg(rng)}, {rreg(rng)}"


def g_arith_imm(rng):
    op = rng.choice([
        "addi", "andi", "ori", "xori",
        "slti", "sltiu"
    ])
    imm = rng.randint(-2048, 2047)
    return f"{op} {rreg(rng)}, {rreg(rng)}, {imm}"


def g_shift_imm(rng):
    op = rng.choice(["slli", "srli", "srai"])
    return f"{op} {rreg(rng)}, {rreg(rng)}, {rng.randint(0,31)}"


def g_lui(rng):
    return f"lui {rreg(rng)}, {rng.randint(0, 0xFFFFF)}"


def g_mul(rng):
    op = rng.choice([
        "mul", "mulh", "mulhu", "mulhsu",
        "div", "divu", "rem", "remu"
    ])
    return f"{op} {rreg(rng)}, {rreg(rng)}, {rreg(rng)}"


def g_zba(rng):
    op = rng.choice(["sh1add", "sh2add", "sh3add"])
    return f"{op} {rreg(rng)}, {rreg(rng)}, {rreg(rng)}"


def g_zbs(rng):
    op = rng.choice(["bset", "bclr", "binv", "bext"])
    return f"{op} {rreg(rng)}, {rreg(rng)}, {rreg(rng)}"


def g_zbb(rng):
    op = rng.choice([
        "min", "max", "minu", "maxu",
        "andn", "orn", "xnor",
        "rol", "ror",
        "sext.b", "sext.h",
        "zext.h",
        "clz", "ctz", "cpop",
        "orc.b", "rev8"
    ])

    if op in [
        "sext.b", "sext.h", "zext.h",
        "clz", "ctz", "cpop",
        "orc.b", "rev8"
    ]:
        return f"{op} {rreg(rng)}, {rreg(rng)}"

    return f"{op} {rreg(rng)}, {rreg(rng)}, {rreg(rng)}"


# Memory:
# use a fixed base register (x31) pointing to the reserved data area

MEM_BASE_REG = "x31"

def g_mem(rng):

    # Aligned offset within the data area
    # (0 .. DATA_BYTES-8)

    width = rng.choice(["w", "h", "hu", "b", "bu"])

    is_store = rng.random() < 0.5

    # Alignment consistent with access width
    align = {"w":4, "h":2, "hu":2, "b":1, "bu":1}[width]

    off = rng.randrange(0, 1024, align)

    if is_store:
        sw = {
            "w":"sw",
            "h":"sh",
            "hu":"sh",
            "b":"sb",
            "bu":"sb"
        }[width]

        return f"{sw} {rreg(rng)}, {off}({MEM_BASE_REG})"

    else:
        lw = {
            "w":"lw",
            "h":"lh",
            "hu":"lhu",
            "b":"lb",
            "bu":"lbu"
        }[width]

        return f"{lw} {rreg(rng)}, {off}({MEM_BASE_REG})"


# Branch/jump:
def g_branch(rng, label_id):
    op = rng.choice(["beq","bne","blt","bge","bltu","bgeu"])
    lbl = f"L{label_id}"

    return (
        f"{op} {rreg(rng)}, {rreg(rng)}, {lbl}\n"
        f"    nop\n"
        f"{lbl}:"
    )


FAMILIES = [

    # (class, required extension or None, function, takes_label?)

    ("arith",  None,  g_arith_i,   False),
    ("arith",  None,  g_arith_imm, False),
    ("arith",  None,  g_shift_imm, False),
    ("arith",  None,  g_lui,       False),
    ("arith",  "m",   g_mul,       False),
    ("arith",  "zba", g_zba,       False),
    ("arith",  "zbs", g_zbs,       False),
    ("arith",  "zbb", g_zbb,       False),

    ("mem",    None,  g_mem,       False),

    ("branch", None,  g_branch,    True),
]

def build_pool(classes: set, exts: set):
    pool = []

    for cls, ext, fn, takes_label in FAMILIES:

        if cls not in classes:
            continue

        if ext is None or ext in exts:
            pool.append((fn, takes_label))

    return pool


def main():

    ap = argparse.ArgumentParser()

    ap.add_argument("--seed", type=int, default=0)
    ap.add_argument("--n", type=int, default=2000,
                    help="number of instructions")

    ap.add_argument("--class",
                    dest="classes",
                    default="arith,mem,branch",
                    help="classes: arith,mem,branch (csv)")

    ap.add_argument("--ext",
                    default="rv32im_zfinx_zba_zbs_zicsr",
                    help="ISA string (= $(ISA) from config.mk)")

    ap.add_argument("--no-fdiv",
                    action="store_true")

    ap.add_argument("--priv",
                    default="mu")

    ap.add_argument("--out",
                    default="cosim/tests/prog.c")

    args = ap.parse_args()

    rng = random.Random(args.seed)

    classes = set(
        c.strip()
        for c in args.classes.split(",")
        if c.strip()
    )

    exts = parse_ext(args.ext)

    pool = build_pool(classes, exts)

    if not pool:
        print(
            "rvgen: no active instruction family for the selected classes/extensions",
            file=sys.stderr
        )
        sys.exit(1)

    # Each "line" may contain multiple instructions
    # (e.g. branch + label).
    # Split them and emit them as separate C string fragments
    # containing "...\\n", so that inline assembly compiles correctly.

    def emit(raw_block):

        segs = []

        for ln in raw_block.split("\n"):
            ln = ln.strip()

            if ln:
                segs.append(f'        "{ln}\\n"')

        return segs

    body_segs = []
    label_id = 0

    for _ in range(args.n):

        fn, takes_label = rng.choice(pool)

        if takes_label:
            body_segs += emit(fn(rng, label_id))
            label_id += 1
        else:
            body_segs += emit(fn(rng))

    asm_body = "\n".join(body_segs)

    # Initialize source registers with known values

    init_segs = []

    for i in range(5, 31):
        init_segs.append(
            f'        "li x{i}, {rng.randint(0, 0x7fffffff)}\\n"'
        )

    init_body = "\n".join(init_segs)

    with open(args.out, "w") as f:

        f.write(f"""// AUTO-GENERATED by rvgen.py
// seed={args.seed} ext={args.ext} priv={args.priv}

// Data area reserved for load/store operations (aligned).
static volatile unsigned char data_area[1280]
    __attribute__((aligned(64)));

void main(void)
{{
    register unsigned long base asm("x31") =
        (unsigned long)data_area;

    asm volatile (

        // --- Initialize source registers
        //     (known, deterministic values) ---

{init_body}

        // --- Random instruction stream ---

{asm_body}

        :
        : "r"(base)
        : "x5","x6","x7","x8","x9","x10",
          "x11","x12","x13","x14","x15",
          "x16","x17","x18","x19","x20",
          "x21","x22","x23","x24","x25",
          "x26","x27","x28","x29","x30",
          "memory"
    );
}}
""")

    print(
        f"rvgen: wrote {args.out} "
        f"({args.n} instructions, "
        f"classes={sorted(classes)}, "
        f"active extensions={sorted(exts)})"
    )


if __name__ == "__main__":
    main()