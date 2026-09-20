#!/usr/bin/env python3
"""
Randomised stress tests: every solution is compared against an obviously-correct (but slow)
brute force on thousands of tiny random inputs.

    python3 tools/stress.py bronze2            # one problem, 300 random tests
    python3 tools/stress.py gold3 1000         # one problem, 1000 random tests
    python3 tools/stress.py all                # everything

Each section below is  generator -> brute force -> comparison, so it doubles as a reference
for how to test your own solutions during a contest.
"""
import itertools
import random
import subprocess
import sys

from check_official import SOLUTIONS, compile_solution, validate_bronze1, validate_silver1


def run(exe, text):
    try:
        return subprocess.run([exe], input=text, capture_output=True, text=True, timeout=10).stdout
    except subprocess.TimeoutExpired:
        return "<timed out>"


def ints(text):
    """Parse whitespace-separated integers; None if the output is garbage (crash / timeout)."""
    try:
        return list(map(int, text.split()))
    except ValueError:
        return None


# ============================================================ bronze 1
def stress_bronze1(exe, rnd):
    # brute force: try all 2^n key sequences -> every string is reachable (so YES always)
    n = rnd.randint(1, 8)
    s = "".join(rnd.choice("MO") for _ in range(n))
    reachable = set()
    for keys in itertools.product("MO", repeat=n):
        typed = []
        for ch in keys:
            if ch == "O":
                typed = ["O" if c == "M" else "M" for c in typed]
            typed.append(ch)
        reachable.add("".join(typed))
    assert s in reachable
    inp = f"1 1\n{n}\n{s}\n"
    out = run(exe, inp)
    return validate_bronze1(inp, out, "YES\nx\n")


# ============================================================ bronze 2
def stress_bronze2(exe, rnd):
    n, k = rnd.randint(3, 8), rnd.randint(1, 12)
    moves = [rnd.sample(range(1, n + 1), 3) for _ in range(k)]
    best, ways = -1, 0
    for board in itertools.product("MO", repeat=n):
        score = sum(board[x - 1] == "M" and board[y - 1] == "O" and board[z - 1] == "O"
                    for x, y, z in moves)
        if score > best:
            best, ways = score, 1
        elif score == best:
            ways += 1
    inp = f"{n} {k}\n" + "".join(f"{x} {y} {z}\n" for x, y, z in moves)
    return None if run(exe, inp).split() == [str(best), str(ways)] else f"want {best} {ways}\n{inp}"


# ============================================================ bronze 3
def stress_bronze3(exe, rnd):
    n = rnd.randint(1, 6)
    a = sorted(rnd.sample(range(1, 40), n))
    xs = [rnd.randint(1, 70) for _ in range(6)]
    want = []
    for x in xs:
        cap = x + 2 ** (n - 1)
        INF = float("inf")
        dp = [INF] * (cap + 1)  # dp[j] = cheapest way to buy EXACTLY j buckets
        dp[0] = 0
        for j in range(1, cap + 1):
            for i in range(n):
                if 2 ** i <= j:
                    dp[j] = min(dp[j], dp[j - 2 ** i] + a[i])
        want.append(min(dp[x:]))
    inp = f"{n} {len(xs)}\n{' '.join(map(str, a))}\n" + "".join(f"{x}\n" for x in xs)
    return None if ints(run(exe, inp)) == want else f"want {want}\n{inp}"


# ============================================================ silver 1
def stress_silver1(exe, rnd):
    n = rnd.randint(2, 6)
    left = "".join(rnd.choice("JN") for _ in range(n))
    right = "".join(rnd.choice("JN") for _ in range(n))
    # brute force: every circular order and every owner assignment
    feasible = False
    for order in itertools.permutations(range(n)):
        for owners in itertools.product("JN", repeat=n):
            own = dict(zip(order, owners))
            good = True
            for i, cow in enumerate(order):
                for claim, nb in ((left[cow], order[i - 1]), (right[cow], order[(i + 1) % n])):
                    truth = own[nb] == "J"
                    said = claim == "J"
                    if said != (truth if own[cow] == "J" else not truth):
                        good = False
                        break
                if not good:
                    break
            if good:
                feasible = True
                break
        if feasible:
            break
    inp = f"1 1\n{n}\n{left}\n{right}\n"
    return validate_silver1(inp, run(exe, inp), "YES\nx\nx\n" if feasible else "NO\n")


# ============================================================ silver 2
def stress_silver2(exe, rnd):
    n, c = rnd.randint(1, 7), rnd.randint(1, 5)
    f = [rnd.randint(1, n) for _ in range(c)]
    p = rnd.sample(range(1, n + 1), n)
    crit = [rnd.sample(range(1, c + 1), rnd.randint(1, c)) for _ in range(n)]
    want = []
    for i in range(n):
        declined = set(p[:i])
        invited = set()
        for j in range(1, c + 1):
            pool = [r for r in range(1, n + 1)
                    if r not in declined and r not in invited and j in crit[r - 1]]
            invited |= set(pool[:f[j - 1]])
        want.append(sum(invited))
    inp = f"{n} {c}\n{' '.join(map(str, f))}\n{' '.join(map(str, p))}\n"
    inp += "".join(f"{len(cs)} {' '.join(map(str, cs))}\n" for cs in crit)
    return None if ints(run(exe, inp)) == want else f"want {want}\n{inp}"


# ============================================================ silver 3
def stress_silver3(exe, rnd):
    n = rnd.randint(1, 8)
    a = [rnd.randint(1, rnd.randint(1, n)) for _ in range(n)]
    full = len(set(a))
    want = []
    for j in range(n):
        # BFS over (pointer position, set of values written down)
        start = (j, 1 << a[j])
        dist = {start: 0}
        frontier = [start]
        answer = None
        while answer is None:
            nxt = []
            for pos, seen in frontier:
                if bin(seen).count("1") == full:
                    answer = dist[(pos, seen)]
                    break
                for d in (-1, 1):
                    q = (pos + d) % n
                    state = (q, seen | (1 << a[q]))
                    if state not in dist:
                        dist[state] = dist[(pos, seen)] + 1
                        nxt.append(state)
            frontier = nxt
        want.append(answer)
    inp = f"{n}\n{' '.join(map(str, a))}\n"
    return None if ints(run(exe, inp)) == want else f"want {want}\n{inp}"


# ============================================================ gold 1
def stress_gold1(exe, rnd):
    n, k = rnd.randint(1, 3), rnd.randint(1, 8)
    a = [rnd.randint(0, 9) for _ in range(n)]
    b = [rnd.randint(0, 9) for _ in range(n)]
    best = None
    for split in itertools.product(range(k + 1), repeat=n):
        if sum(split) != k:
            continue
        val = max(x - s for x, s in zip(a, split)) - min(y + s for y, s in zip(b, split))
        best = val if best is None else min(best, val)
    inp = f"1\n{n} {k}\n{' '.join(map(str, a))}\n{' '.join(map(str, b))}\n"
    return None if run(exe, inp).split() == [str(best)] else f"want {best}\n{inp}"


# ============================================================ gold 2
def stress_gold2(exe, rnd):
    n = rnd.randint(1, 7)
    edges = [(i, rnd.randint(0, i - 1), rnd.choice("abc")) for i in range(1, n)]  # spanning tree
    for _ in range(rnd.randint(0, 5)):
        edges.append((rnd.randint(0, n - 1), rnd.randint(0, n - 1), rnd.choice("abc")))
    adj = [[] for _ in range(n)]
    for u, v, c in edges:
        adj[u].append((v, c))
        adj[v].append((u, c))

    # brute force 1: follow the greedy word with explicit vertex sets until a set repeats
    first = [-1] * n
    X, t, seen = frozenset([0]), 0, set()
    while X not in seen:
        seen.add(X)
        for v in X:
            if first[v] < 0:
                first[v] = t
        letters = [c for v in X for _, c in adj[v]]
        if not letters:
            break
        m = min(letters)
        X = frozenset(w for v in X for w, c in adj[v] if c == m)
        t += 1

    # brute force 2 (independent): smallest string over ALL walks of length <= 9
    LIM = 9
    best = [None] * n
    layer = {0: ""}
    for _ in range(LIM + 1):
        for v, s in layer.items():
            if best[v] is None or s < best[v]:
                best[v] = s
        nxt = {}
        for u, s in layer.items():
            for w, c in adj[u]:
                if w not in nxt or s + c < nxt[w]:
                    nxt[w] = s + c
        layer = nxt
    for v in range(n):
        if first[v] != -1 and first[v] <= LIM:
            assert len(best[v]) == first[v], "greedy-word theory disagrees with walk enumeration"

    inp = f"1\n{n} {len(edges)}\n" + "".join(f"{u+1} {v+1} {c}\n" for u, v, c in edges)
    return None if ints(run(exe, inp)) == first else f"want {first}\n{inp}"


# ============================================================ gold 3
def brute_chase(n, nxt, farmers):
    def step(x): return nxt[x]
    # farmer configuration is eventually periodic in time
    configs, cur, seen = [], tuple(farmers), {}
    while cur not in seen:
        seen[cur] = len(configs)
        configs.append(set(cur))
        cur = tuple(step(x) for x in cur)
    t0, total = seen[cur], len(configs)          # config(total) == config(t0)
    nt = lambda t: t + 1 if t + 1 < total else t0

    states = [(v, t) for t in range(total) for v in range(n) if v not in configs[t]]
    alive = set(states)
    edges = {s: [] for s in states}
    for v, t in states:
        for w, weight in ((v, 1), (step(v), 0)):
            if (w, nt(t)) in alive:
                edges[(v, t)].append(((w, nt(t)), weight))
    changed = True
    while changed:                                # keep only states that can survive forever
        changed = False
        for s in list(alive):
            if not any(d in alive for d, _ in edges[s]):
                alive.discard(s)
                changed = True
    edges = {s: [(d, w) for d, w in edges[s] if d in alive] for s in alive}

    reach = {s: {s} for s in alive}               # transitive closure (tiny graphs)
    changed = True
    while changed:
        changed = False
        for s in alive:
            for d, _ in edges[s]:
                if not reach[d] <= reach[s]:
                    reach[s] |= reach[d]
                    changed = True

    res = []
    for b in range(n):
        s0 = (b, 0)
        if s0 not in alive:
            res.append(-1)
            continue
        if any(w == 1 and u in reach[d] for u in reach[s0] for d, w in edges[u]):
            res.append(-2)                        # a rest edge lies on a reachable cycle
            continue
        dist = {s0: 0}
        for _ in range(len(alive) + 1):           # longest path (all cycles have weight 0)
            for u in list(dist):
                for d, w in edges[u]:
                    if dist.get(d, -1) < dist[u] + w:
                        dist[d] = dist[u] + w
        res.append(max(dist.values()))
    return res


def stress_gold3(exe, rnd):
    n = rnd.randint(2, 7)
    nxt = [rnd.choice([x for x in range(n) if x != i]) for i in range(n)]
    farmers = rnd.sample(range(n), rnd.randint(1, n))
    want = brute_chase(n, nxt, farmers)
    inp = f"{n} {len(farmers)}\n{' '.join(str(x + 1) for x in nxt)}\n"
    inp += f"{' '.join(str(x + 1) for x in farmers)}\n"
    return None if ints(run(exe, inp)) == want else f"want {want}\n{inp}"


TESTS = {
    "bronze1": stress_bronze1, "bronze2": stress_bronze2, "bronze3": stress_bronze3,
    "silver1": stress_silver1, "silver2": stress_silver2, "silver3": stress_silver3,
    "gold1": stress_gold1, "gold2": stress_gold2, "gold3": stress_gold3,
}


def main():
    if len(sys.argv) < 2 or (sys.argv[1] != "all" and sys.argv[1] not in TESTS):
        print(__doc__)
        sys.exit(2)
    names = list(TESTS) if sys.argv[1] == "all" else [sys.argv[1]]
    iterations = int(sys.argv[2]) if len(sys.argv) > 2 else 300
    failed = False
    for name in names:
        exe = compile_solution(name)
        rnd = random.Random(12345)
        for i in range(iterations):
            problem = TESTS[name](exe, rnd)
            if problem:
                print(f"{name}: MISMATCH on random test {i}\n{problem}")
                failed = True
                break
        else:
            print(f"{name}: {iterations} random tests passed")
    sys.exit(1 if failed else 0)


if __name__ == "__main__":
    main()
