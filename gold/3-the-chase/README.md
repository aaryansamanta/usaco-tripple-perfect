# Gold 3 · The Chase

| | |
|---|---|
| **Official problem** | [The Chase](https://usaco.org/index.php?page=viewproblem2&cpid=1571) |
| **Official analysis** | [https://usaco.org/current/data/sol_prob3_gold_season26contest2.html](https://usaco.org/current/data/sol_prob3_gold_season26contest2.html) |
| **Official test data** | [prob3_gold_season26contest2.zip](https://usaco.org/current/data/prob3_gold_season26contest2.zip) |
| **My solution** | [`solution.cpp`](solution.cpp) |

**Topics:** functional graphs, multi-source BFS, cycle "frames", modular arithmetic · **Time:** O(N) · **Difficulty:** ⭐⭐⭐⭐

## The problem in one paragraph
Every farm has exactly one outgoing road. Farmers start on some farms and **all move along their road every step**. Bessie starts on farm `b`; each step she may rest or follow her road, and she is caught if she ever shares a farm with a farmer. For each `b`, print the maximum number of rests she can take without ever being caught: `-1` if she cannot avoid capture, `-2` if she can rest forever.

## Structure
The roads form cycles with in-trees attached. Bessie's path is `b → next(b) → …` and ends up circling one cycle.

## Key insights
1. **Take all rests at the start.** Delaying a rest can only hurt (a farmer behind her closes in), so for a start `b` it is enough to choose `k` = rests in place at `b`, then walk forever. This is the observation highlighted in USACO's official analysis, and the stress test confirms it against a full state-space search.
2. **Resting at `b` is safe for `k < t[b]`**, where `t[b]` is the earliest time any farmer reaches `b` (multi-source BFS along the roads). If `t[b] = ∞`, nobody ever comes: answer `-2`. If `t[b] = 0`, a farmer stands on `b`: answer `-1`.
3. **Use a rotating frame on the cycle.** A farmer on cycle position `p` at time `t` has *slot* `(p − t) mod len`, which never changes. Bessie, once she walks continuously, also has a fixed slot. She is caught iff her slot equals a farmer's slot. A farmer starting in a tree at depth `d` under root position `q` has slot `(q − d) mod len`. Mark those slots **bad**.
4. Bessie reaches her root at time `k + depth[b]`, so her slot is `(q_b − depth[b] − k) mod len`. If she meets a farmer on the way up the tree they stay together up to the root, so checking the root slot is enough.

## Answer for one start
Try `k = t[b] − 1` (the maximum). Each unit fewer moves her slot one step forward, so we need the distance `y` from her slot forward to the nearest **good** slot (precomputed cyclically). Answer `k − y`, or `-1` if there is no good slot or `y > k`.

## Pitfalls
- Do everything iteratively: trees can be 5·10⁵ deep, so recursion would overflow the stack.
- Use positive modulo for slots (`((x % L) + L) % L`).
- Cycles are numbered along the road direction so that "slot + 1" really means "one rest earlier".

## Takeaway
On a functional graph, switch to the frame that moves with everything at speed 1. Moving obstacles become fixed points, and "resting" becomes a simple shift.
