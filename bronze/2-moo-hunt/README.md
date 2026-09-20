# Bronze 2 · Moo Hunt

| | |
|---|---|
| **Official problem** | [Moo Hunt](https://usaco.org/index.php?page=viewproblem2&cpid=1564) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob2_bronze_season26contest2.html](https://usaco.org/current/data/sol_prob2_bronze_season26contest2.html) |
| **Official test data** | [prob2_bronze_season26contest2.zip](https://usaco.org/current/data/prob2_bronze_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** bitmask enumeration, subset-sum (SOS) DP · **Time:** O(N²·2ᴺ) · **Difficulty:** ⭐⭐

## The problem in one paragraph
A board has `N ≤ 20` cells, each `M` or `O`. There are `K` "mooves" `(x, y, z)`; a moove scores a point when cell `x` is `M` and cells `y` and `z` are both `O`. Find the best possible score over all `2ᴺ` boards and how many boards achieve it.

## Why brute force is too slow
Scoring one board costs O(K), and there are ~10⁶ boards with K up to 2·10⁵.

## Key insight
Write a board as the bitmask `A` of its `M` cells. Group the moves by their first cell `x`. For a fixed `x`, a move contributes to boards where `x ∈ A` and `{y, z} ⊆ complement(A)`:

```
score(A) = Σ_{x ∈ A}  g_x( ~A )
g_x(T)   = number of moves starting at x whose {y, z} is a subset of T
```

`g_x` is a classic **subset-sum over masks** (SOS DP): drop each move on the mask `{y, z}`, then for every bit add `g[mask without bit]` into `g[mask]`. One SOS per `x`, then read `g_x[~A]` for every board that has `x` as an `M`.

## Complexity
20 values of `x` × 19 bits × 2²⁰ masks ≈ 4·10⁸ trivial operations: about 0.3 s in practice. (Skipping the bit `x` itself is safe because no pair belonging to `x` contains it.)

## Pitfalls
- Moves are per *first* cell; `y` and `z` are unordered for the subset test.
- Keep the score array as `int` (≤ K) and count ways in `long long`.

## Takeaway
"For every subset, count the items that fit inside it" is exactly SOS DP. Recognise it whenever you see `N ≤ 20` and pair/triple conditions.
