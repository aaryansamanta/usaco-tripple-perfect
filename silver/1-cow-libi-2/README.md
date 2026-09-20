# Silver 1 · Cow-libi 2

| | |
|---|---|
| **Official problem** | [Cow-libi 2](https://usaco.org/index.php?page=viewproblem2&cpid=1566) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob1_silver_season26contest2.html](https://usaco.org/current/data/sol_prob1_silver_season26contest2.html) |
| **Official test data** | [prob1_silver_season26contest2.zip](https://usaco.org/current/data/prob1_silver_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** logic reduction, cyclic sequences, construction · **Time:** O(N) · **Difficulty:** ⭐⭐⭐

## The problem in one paragraph
`N` cows sit in a circle; each belongs to Farmer John (truthful) or Farmer Nhoj (always lies). Each cow claims which farmer owns her left and right neighbour. Decide whether some seating and ownership is consistent with all claims and, if `C = 1`, print one.

## Key insight: a claim only says "same" or "different"
- A truthful John cow says `J` exactly when the neighbour is John's: **same owner as her**.
- A lying Nhoj cow says `J` exactly when the neighbour is Nhoj's: again **same owner as her**.

So `L = 1` (claim `J`) ⇔ "my left neighbour has my owner", and likewise `R`. Owners no longer matter for the check.

## Turning it into a cyclic bit string
Let `d[i] = 1` if seats `i` and `i+1` share an owner. The cow in seat `i` has claims `(L, R) = (d[i-1], d[i])`. Count the cows by pair type: `a = #(1,1)`, `b = #(1,0)`, `b' = #(0,1)`, `c = #(0,0)`.

A cyclic 0/1 sequence exists with these pair counts iff
- `b = b'` (1→0 and 0→1 transitions balance around a cycle);
- `b = 0` ⇒ the sequence is constant, so `a = 0` or `c = 0`;
- the number of zeros, `b + c`, is **even** (owners must switch an even number of times to return to the start).

## Construction
- `b = 0`: all ones (or all zeros).
- `b > 0`: `1^(a+1)  0^(c+1)  (1 0)^(b-1)`.

Seat, at position `i`, any unused cow with claims `(d[i-1], d[i])`; then walk around flipping the owner whenever `d[i] = 0`.

## Pitfalls
- The `N = 2` circle needs no special case (the parity rule already rejects `b = 1`).
- Print the permutation and owner string only when `C = 1` **and** the answer is `YES`.

## Takeaway
When liars and truth-tellers are mixed, look for a quantity that both types report the same way. Here it is the *relation* "same / different".
