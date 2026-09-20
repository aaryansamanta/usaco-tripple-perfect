# 🥈 Silver

Reduction and amortisation: turn a story into a clean combinatorial statement, then bound the work.

| # | Problem | Core idea | Complexity | Notes |
|---|---------|-----------|-----------|-------|
| 1 | [Cow-libi 2](1-cow-libi-2/) | Claims only say "same owner or not"; count pair types on a cyclic bit string | O(N) | [solution](1-cow-libi-2/solution.cpp) |
| 2 | [Declining Invitations](2-declining-invitations/) | Repair chains that only move to earlier criteria | O(N + C + Σnᵢ) | [solution](2-declining-invitations/solution.cpp) |
| 3 | [Farmer John Loves Rotations](3-fj-loves-rotations/) | Walk on a circle, cost `l + r + min(l, r)`, two pointers + deques | O(N) | [solution](3-fj-loves-rotations/solution.cpp) |

**Suggested order:** 3 → 1 → 2.
