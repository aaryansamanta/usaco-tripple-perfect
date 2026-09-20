# Gold 2 · Lexicographically Smallest Path

| | |
|---|---|
| **Official problem** | [Lexicographically Smallest Path](https://usaco.org/index.php?page=viewproblem2&cpid=1570) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob2_gold_season26contest2.html](https://usaco.org/current/data/sol_prob2_gold_season26contest2.html) |
| **Official test data** | [prob2_gold_season26contest2.zip](https://usaco.org/current/data/prob2_gold_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** automata / subset simulation, parity BFS, phase decomposition · **Time:** O(26·(N+M)) · **Difficulty:** ⭐⭐⭐⭐

## The problem in one paragraph
An undirected connected graph has lowercase letters on its edges (multi-edges and self-loops allowed). Walks may reuse edges. For every vertex `v`, find the **length** of the lexicographically smallest walk-string from vertex 1 to `v`, or `-1` if none exists (a shorter string is smaller than any extension of it).

## Key insight: follow the greedy word
Keep the set `X` of vertices you could be at after reading the word so far. At each step read the **smallest letter available from any vertex in `X`**, and move `X` to everything reachable by that letter.

- Every vertex can still finish a walk (the graph is connected), so the greedy choice never gets stuck.
- The smallest walk to `v` exists iff `v` **ever appears in `X`**, and its length is the **first time** it does. If `v` is already in `X`, stopping there beats every extension. If `v` never appears, longer and longer strings keep getting smaller, so no minimum exists.

## Making it fast
Simulating `X` step by step can take very long, so use the structure:

1. **Letters never increase.** A vertex we just entered has an edge with the letter we entered by, so the next minimum is ≤ the last. Hence at most **26 phases**, each with a fixed letter `c`.
2. **Inside a phase**, `X` after `s` more steps is "vertices reachable from the start set by a walk of exactly `s` edges of letter `c`". A BFS over `(vertex, parity)` gives `D[v][p]`, the shortest such walk with parity `p`; `v ∈ X` at step `s` iff `s ≥ D[v][s mod 2]`.
3. **A phase ends** the first time `X` contains a vertex that has an edge with a smaller letter. Only those vertices matter for the next phase.
4. If no such vertex is ever reached, the phase lasts forever and we just record `t0 + min(D[v][0], D[v][1])` for the remaining vertices.

Each phase is one BFS, so the whole thing is at most 26 BFS passes.

## Pitfalls
- Self-loops flip parity and are handled naturally by the `(vertex, parity)` BFS.
- A vertex is recorded the *first* time it appears; never overwrite it.
- Vertices in the start set that have no edge of letter `c` stay put for that phase only.

## Takeaway
When the question is "does the smallest string exist?", think of the **infimum** as an infinite greedy word and ask when your target lies on it. Then compress the simulation by exploiting monotonicity (here, non-increasing letters).
