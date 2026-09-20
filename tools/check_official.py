#!/usr/bin/env python3
"""
Run a solution against USACO's official test data.

USACO publishes the test data after each contest (links are in every problem's README).
Unzip a problem's data into a folder so that it contains 1.in, 1.out, 2.in, 2.out, ...

    python3 tools/check_official.py bronze1 path/to/bronze1_data
    python3 tools/check_official.py --all path/to/parent_folder      # parent has bronze1/, ..., gold3/

Problem keys: bronze1 bronze2 bronze3 silver1 silver2 silver3 gold1 gold2 gold3

Two problems accept many different outputs (bronze1 and silver1 ask for a
construction), so those are checked by a validator that simulates the answer
instead of diffing it against the official .out file.
"""
import os
import subprocess
import sys
import time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BUILD = os.path.join(ROOT, ".build")

SOLUTIONS = {
    "bronze1": "bronze/1-mooin-time-iv/solution.cpp",
    "bronze2": "bronze/2-moo-hunt/solution.cpp",
    "bronze3": "bronze/3-purchasing-milk/solution.cpp",
    "silver1": "silver/1-cow-libi-2/solution.cpp",
    "silver2": "silver/2-declining-invitations/solution.cpp",
    "silver3": "silver/3-fj-loves-rotations/solution.cpp",
    "gold1": "gold/1-balancing-the-barns/solution.cpp",
    "gold2": "gold/2-lex-smallest-path/solution.cpp",
    "gold3": "gold/3-the-chase/solution.cpp",
}


def compile_solution(key):
    os.makedirs(BUILD, exist_ok=True)
    exe = os.path.join(BUILD, key)
    src = os.path.join(ROOT, SOLUTIONS[key])
    subprocess.check_call(["g++", "-O2", "-std=c++17", "-o", exe, src])
    return exe


# ---------------------------------------------------------------- validators
def validate_bronze1(inp, got, want):
    """k = 1 answers are any key sequence that types S; simulate it."""
    it = iter(inp.split())
    t, k = int(next(it)), int(next(it))
    g, w = got.split("\n"), want.split("\n")
    gi = wi = 0
    for _ in range(t):
        n, s = int(next(it)), next(it)
        if g[gi].strip() != w[wi].strip():
            return "verdict mismatch"
        gi += 1
        wi += 1
        if k == 1:
            keys = g[gi].strip()
            gi += 1
            wi += 1
            if len(keys) != n:
                return "wrong length"
            # simulate in O(N): keep a lazy global flip instead of flipping every letter
            flip, raw = 0, []
            for ch in keys:
                if ch == "O":
                    flip ^= 1                # everything typed so far flips ...
                raw.append((1 if ch == "O" else 0) ^ flip)  # ... then the key is appended
            typed = ["O" if r ^ flip else "M" for r in raw]
            if "".join(typed) != s:
                return "keystrokes do not type S"
    return None


def validate_silver1(inp, got, want):
    """C = 1 answers are any valid seating; check every cow's statements."""
    it = iter(inp.split())
    t, c = int(next(it)), int(next(it))
    g, w = got.split("\n"), want.split("\n")
    gi = wi = 0
    for _ in range(t):
        n = int(next(it))
        left, right = next(it), next(it)
        verdict = g[gi].strip()
        gi += 1
        if verdict != w[wi].strip():
            return "verdict mismatch"
        wi += 1
        if verdict == "YES" and c == 1:
            wi += 2
            order = list(map(int, g[gi].split()))
            kind = g[gi + 1].strip()
            gi += 2
            if sorted(order) != list(range(1, n + 1)) or len(kind) != n:
                return "not a permutation / wrong length"
            owner = {}
            for cow, ch in zip(order, kind):
                owner[cow] = ch
            for i, cow in enumerate(order):
                l_cow, r_cow = order[i - 1], order[(i + 1) % n]
                truthful = owner[cow] == "J"
                for claim, nb in ((left[cow - 1], l_cow), (right[cow - 1], r_cow)):
                    is_j = owner[nb] == "J"
                    if (claim == "J") != (is_j if truthful else not is_j):
                        return "a cow's statement is violated"
    return None


VALIDATORS = {"bronze1": validate_bronze1, "silver1": validate_silver1}


# ---------------------------------------------------------------- runner
def run_problem(key, data_dir):
    exe = compile_solution(key)
    cases = sorted(
        int(f[:-3]) for f in os.listdir(data_dir) if f.endswith(".in") and f[:-3].isdigit()
    )
    if not cases:
        print(f"{key}: no N.in files found in {data_dir}")
        return False
    ok = True
    worst = 0.0
    for c in cases:
        with open(os.path.join(data_dir, f"{c}.in")) as f:
            inp = f.read()
        with open(os.path.join(data_dir, f"{c}.out")) as f:
            want = f.read()
        start = time.time()
        proc = subprocess.run([exe], input=inp, capture_output=True, text=True, timeout=60)
        took = time.time() - start
        worst = max(worst, took)
        got = proc.stdout
        if proc.returncode != 0:
            err = f"runtime error (exit {proc.returncode})"
        elif key in VALIDATORS:
            err = VALIDATORS[key](inp, got, want)
        else:
            err = None if got.split() == want.split() else "wrong answer"
        status = "ok " if err is None else "FAIL"
        print(f"  {key} case {c:>2}: {status} {took:5.2f}s" + (f"  <- {err}" if err else ""))
        ok &= err is None
    print(f"{key}: {'ALL PASSED' if ok else 'FAILED'}  (slowest case {worst:.2f}s)\n")
    return ok


def main():
    args = sys.argv[1:]
    if len(args) == 2 and args[0] == "--all":
        results = [run_problem(k, os.path.join(args[1], k)) for k in SOLUTIONS]
        sys.exit(0 if all(results) else 1)
    if len(args) == 2 and args[0] in SOLUTIONS:
        sys.exit(0 if run_problem(args[0], args[1]) else 1)
    print(__doc__)
    sys.exit(2)


if __name__ == "__main__":
    main()
