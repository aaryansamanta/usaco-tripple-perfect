# 🥇 Gold

Larger ideas: convex optimisation under huge numbers, an automaton hidden inside a graph, and a rotating frame on a functional graph.

| # | Problem | Core idea | Complexity | Notes |
|---|---------|-----------|-----------|-------|
| 1 | [Balancing the Barns](1-balancing-the-barns/) | Binary search the answer; inner convex search over the max; `__int128` | O(N·log²) | [solution](1-balancing-the-barns/solution.cpp) |
| 2 | [Lexicographically Smallest Path](2-lex-smallest-path/) | Greedy word over vertex sets, ≤ 26 phases, parity BFS | O(26·(N+M)) | [solution](2-lex-smallest-path/solution.cpp) |
| 3 | [The Chase](3-the-chase/) | All rests first; farmers become fixed "slots" in a rotating frame | O(N) | [solution](3-the-chase/solution.cpp) |

**Suggested order:** 1 → 3 → 2.
