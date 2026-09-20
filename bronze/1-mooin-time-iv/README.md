# Bronze 1 · It's Mooin' Time IV

| | |
|---|---|
| **Official problem** | [It's Mooin' Time IV](https://usaco.org/index.php?page=viewproblem2&cpid=1563) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob1_bronze_season26contest2.html](https://usaco.org/current/data/sol_prob1_bronze_season26contest2.html) |
| **Official test data** | [prob1_bronze_season26contest2.zip](https://usaco.org/current/data/prob1_bronze_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** XOR / parity, difference array, constructive · **Time:** O(N) · **Difficulty:** ⭐

## The problem in one paragraph
Bessie's keyboard has only `M` and `O`. Whenever she types an `O`, every letter she has already typed flips (`M`↔`O`) *before* the new `O` is appended. Given a target string `S`, can she type it? If the flag `k = 1`, also print one key sequence that works.

## Key insight
Let `f[i]` be the final letter at position `i` (`M = 0`, `O = 1`) and `k[i]` the key pressed at step `i`. Position `i` is flipped once for every `O` pressed **after** it, and the key itself is never flipped when it lands, so

```
f[i] = k[i] xor k[i+1] xor ... xor k[N-1]        (suffix XOR)
```

A suffix XOR is invertible: subtract neighbours and you get the keys back.

```
k[i] = f[i] xor f[i+1]        with f[N] = 0
```

So the answer is **always `YES`**, and the key at position `i` is `O` exactly when `S[i] != S[i+1]` (treat the letter after the end as `M`).

## Worked example: `MOO`
`f = 0 1 1` → `k = (0⊕1, 1⊕1, 1⊕0) = 1 0 1` → keys `OMO`. Typing them: `O` → `O`; `M` → `OM`; `O` flips to `MO`, appends `O` → `MOO` ✓.

## Pitfalls
- The second line of output exists **only when `k = 1`**.
- Don't simulate the flips: that is O(N²). The formula needs one pass.

## Takeaway
When the process is "flip everything before me", think in terms of **suffix parity**. If a transformation is invertible, the answer to "is it possible?" is trivially yes.
