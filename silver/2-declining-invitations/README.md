# Silver 2 · Declining Invitations

| | |
|---|---|
| **Official problem** | [Declining Invitations](https://usaco.org/index.php?page=viewproblem2&cpid=1567) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob2_silver_season26contest2.html](https://usaco.org/current/data/sol_prob2_silver_season26contest2.html) |
| **Official test data** | [prob2_silver_season26contest2.zip](https://usaco.org/current/data/prob2_silver_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** offline processing, amortised chains, monotone pointers · **Time:** O(N + C + Σnᵢ) · **Difficulty:** ⭐⭐⭐

## The problem in one paragraph
`N` ranked contestants, `C` criteria in order. Under criterion `j` the best `fⱼ` **not-yet-invited** contestants who satisfy it are invited. Contestants then decline one by one in a given order. Print the sum of invited ranks before each decline.

## Key insight: repair, don't recompute
Run the process once with nobody declining and remember `inv[y]`, the criterion under which `y` was invited (∞ if never).

When `x` declines and `x` was invited under criterion `j`, criterion `j` has **one free slot**. Who takes it? The best-ranked contestant `y` who satisfies `j`, is still present, and is not invited *before* `j` (`inv[y] > j`).

- If `y` was never invited, he simply takes the slot and we are done.
- Otherwise `y` leaves a slot behind at his old criterion `inv[y] > j`, and we repeat there.

## Why it is fast
Each repair step moves a contestant to a **strictly earlier** criterion from his own list, so overall there are at most Σnᵢ steps. To find the best candidate for criterion `j` we scan its rank-sorted list with a pointer that never moves back: "present and `inv[y] > j`" can only change from true to false.

## Pitfalls
- Buckets must be sorted by rank: build them by iterating contestants in rank order.
- Use `long long` for the sum (up to ~5·10⁹).
- Output the sum *before* each decline (N lines, the last state is not printed).

## Takeaway
If a process is easy to run forward, ask what *one change* does to it. Bound the total number of changes with a potential ("only ever moves earlier").
