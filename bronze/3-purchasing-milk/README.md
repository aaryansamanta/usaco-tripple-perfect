# Bronze 3 · Purchasing Milk

| | |
|---|---|
| **Official problem** | [Purchasing Milk](https://usaco.org/index.php?page=viewproblem2&cpid=1565) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob3_bronze_season26contest2.html](https://usaco.org/current/data/sol_prob3_bronze_season26contest2.html) |
| **Official test data** | [prob3_bronze_season26contest2.zip](https://usaco.org/current/data/prob3_bronze_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** greedy on powers of two, price normalisation · **Time:** O(N + Q·31) · **Difficulty:** ⭐⭐

## The problem in one paragraph
Deal `i` sells `2^(i-1)` buckets of milk for `a[i]` moonies, with `a` strictly increasing. For each query `x` (up to 10⁹) find the cheapest way to buy **at least** `x` buckets.

## Key insights
1. **Only 31 deals matter.** Deal 31 already gives 2³⁰ > 10⁹ buckets, and every later deal costs more.
2. **Normalise prices.** Let `c[i] = min(a[i], 2·c[i-1])`, because two copies of the previous deal give the same buckets as one copy of deal `i`. Afterwards `c` never decreases and `c[i] ≤ 2·c[i-1]`.
3. Because of (2), using a smaller deal twice is never better than using the next-bigger deal once. So an optimal purchase uses **each deal below the largest at most once**, which is exactly a binary representation.

## Algorithm
Go from the biggest deal to the smallest with `left = x` and `spent = 0`. At each level:
- candidate answer: `spent + (left / size + 1) · c[i]`, i.e. *round up here and stop*;
- then commit the exact part: `spent += (left / size) · c[i]`, `left %= size`.

If `left` ends at 0, `spent` is a candidate too. The answer is the smallest candidate.

## Pitfalls
- "At least" means overshooting can be cheaper (in the second sample, 3 buckets cost 30, the same as 4).
- With `N < 31` the top deal is used many times: values reach ~10¹⁸, so use `long long`.

## Takeaway
Fix the cost structure first (normalise), *then* the greedy becomes obviously correct.
