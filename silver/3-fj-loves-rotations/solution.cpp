// USACO 2026 Second Contest, Silver 3 -- Farmer John Loves Rotations
// https://usaco.org/index.php?page=viewproblem2&cpid=1568
//
// Rotating the array is the same as walking a pointer around a circle: shifting left or
// right moves the pointer to a neighbour and "writes down" the value there. Starting at
// index j, the visited positions always form an arc [j-l, j+r], and covering it costs
//     l + r + min(l, r)      (go to the nearer end first, come back, go to the far end).
// We need the cheapest arc through j that contains every distinct value.
//
// Work on three copies of A. For each left end s let e(s) be the smallest right end such
// that [s, e(s)] holds all K distinct values (two pointers). Take j in the middle copy.
// For a left end s <= j the best right end is max(e(s), j), so with l = j-s, r = e-j:
//   * if e(s) <= j: cost = j - s, best for the largest such s (call it s*)
//   * else (s in (s*, j]):  cost = min( e(s) - 2s + j ,  2e(s) - s - j )
// The two expressions are minimised over s in the sliding window (s*, j] with monotone
// deques, and s* only moves right.
//
// Time: O(N).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &v : a) cin >> v;

    int distinct = 0;
    {
        vector<char> seen(n + 1, 0);
        for (int v : a) if (!seen[v]) seen[v] = 1, distinct++;
    }

    // e[s] for s in [0, 2n]: smallest end so that B[s..e[s]] has every value (B = A x3)
    auto B = [&](int i) { return a[i % n]; };
    vector<int> e(2 * n + 1);
    {
        vector<int> cnt(n + 1, 0);
        int have = 0, r = 0;  // window is B[s .. r-1]
        for (int s = 0; s <= 2 * n; s++) {
            while (have < distinct) {
                if (cnt[B(r)]++ == 0) have++;
                r++;
            }
            e[s] = r - 1;
            if (--cnt[B(s)] == 0) have--;
        }
    }

    auto P = [&](int s) { return e[s] - 2 * s; };
    auto Q = [&](int s) { return 2 * e[s] - s; };

    deque<int> dp, dq;  // sliding-window minima of P and Q
    int pushed = 0, star = 0;
    vector<int> ans(n);
    for (int j = n; j < 2 * n; j++) {
        while (star + 1 <= j && e[star + 1] <= j) star++;
        while (pushed <= j) {
            while (!dp.empty() && P(dp.back()) >= P(pushed)) dp.pop_back();
            while (!dq.empty() && Q(dq.back()) >= Q(pushed)) dq.pop_back();
            dp.push_back(pushed);
            dq.push_back(pushed);
            pushed++;
        }
        while (!dp.empty() && dp.front() <= star) dp.pop_front();
        while (!dq.empty() && dq.front() <= star) dq.pop_front();

        int best = j - star;
        if (!dp.empty()) best = min(best, j + P(dp.front()));
        if (!dq.empty()) best = min(best, Q(dq.front()) - j);
        ans[j - n] = best;
    }

    for (int i = 0; i < n; i++) cout << ans[i] << " \n"[i + 1 == n];
    return 0;
}
