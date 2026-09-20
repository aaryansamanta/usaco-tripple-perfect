# Silver 3 · Farmer John Loves Rotations

| | |
|---|---|
| **Official problem** | [Farmer John Loves Rotations](https://usaco.org/index.php?page=viewproblem2&cpid=1568) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob3_silver_season26contest2.html](https://usaco.org/current/data/sol_prob3_silver_season26contest2.html) |
| **Official test data** | [prob3_silver_season26contest2.zip](https://usaco.org/current/data/prob3_silver_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** circular arrays, two pointers, sliding-window minimum · **Time:** O(N) · **Difficulty:** ⭐⭐⭐

## The problem in one paragraph
For each starting index `j` of a circular array, Farmer John repeatedly rotates left or right by one and writes down the value now at index `j`. What is the fewest rotations after which every distinct value has been written?

## Key insight: it is a walk on a circle
Rotating is the same as moving a pointer to a neighbouring cell and reading it. The visited cells always form an **arc** `[j-l, j+r]`, and covering an arc costs

```
l + r + min(l, r)      (go to the nearer end, come back, go to the far end)
```

We want the cheapest arc through `j` containing every distinct value.

## Algorithm
1. Triple the array. For each left end `s`, two pointers give `e(s)`, the smallest right end so that `[s, e(s)]` contains all `K` distinct values (`e` is non-decreasing).
2. For start `j` (in the middle copy), let `s*` be the largest `s ≤ j` with `e(s) ≤ j`. That arc needs no right moves: cost `j - s*`.
3. Every `s` in `(s*, j]` has `e(s) > j`, so `l = j - s`, `r = e(s) - j`, and the cost is `min( e(s) - 2s + j ,  2e(s) - s - j )`.
4. Minimise `e(s) - 2s` and `2e(s) - s` over that window with two monotone deques. Both ends of the window only move right.

## Pitfalls
- Three copies are needed so that arcs can wrap in either direction.
- `min(X, Y)` over `s` equals `min(min X, min Y)`, which is why two independent window minima suffice.

## Takeaway
"Left cost + right cost + the smaller of the two" is the standard shape for a there-and-back walk on a line or circle. Split the `min` into two linear cases and use sliding-window minima.
