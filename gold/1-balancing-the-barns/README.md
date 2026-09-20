# Gold 1 · Balancing the Barns

| | |
|---|---|
| **Official problem** | [Balancing the Barns](https://usaco.org/index.php?page=viewproblem2&cpid=1569) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob1_gold_season26contest2.html](https://usaco.org/current/data/sol_prob1_gold_season26contest2.html) |
| **Official test data** | [prob1_gold_season26contest2.zip](https://usaco.org/current/data/prob1_gold_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** binary search on the answer, convexity, nested search, 128-bit arithmetic · **Time:** O(N·log²) · **Difficulty:** ⭐⭐⭐

## The problem in one paragraph
Barn `i` holds `a[i]` haybales and `b[i]` bags of feed. One operation turns a haybale of barn `i` into a bag of feed there (`a[i]−1`, `b[i]+1`). After exactly `K` operations (K up to 10¹⁸), minimise `max(a) − min(b)`. Barns may go negative.

## Key insights
1. **More operations never hurt.** Each one lowers some `a[i]` and raises some `b[i]`, so `max(a)` cannot rise and `min(b)` cannot fall. "Exactly K" therefore equals "at most K": dump the spare operations anywhere.
2. **Binary search the answer `D`.** Feasibility is monotone: if imbalance ≤ D is reachable, so is any larger D.
3. **Test one `D`.** Suppose the final maximum hay is `M`. Barn `i` needs `k[i] ≥ max(0, a[i]−M, M−D−b[i])` operations (bring hay down to `M`, and feed up to `M−D`). So

```
cost(M) = Σ max(0, a[i] − M, M − D − b[i])        D feasible  ⇔  min over M of cost(M) ≤ K
```

4. **`cost` is convex in `M`** (a sum of maxima of linear functions), so the best `M` comes from a second binary search on the slope: the smallest `M` with `cost(M+1) − cost(M) ≥ 0`.

## Numbers
`D` can be as low as about −2·10¹⁸ and single terms reach ~10¹⁸ with `N` of them, so sum costs in `__int128`. The search range for `M` is `[−10¹⁸, max a]`: below that, one barn alone would already exceed `K`.

## Pitfalls
- Off-by-one in the slope search: search for the *first* `M` where the slope is non-negative.
- The answer can be negative; use a signed lower bound well below zero.

## Takeaway
Two nested binary searches are fine when each layer is monotone/convex. Always check what "exactly K" really forces (here: nothing).
