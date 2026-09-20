// USACO 2026 Second Contest, Bronze 2 -- Moo Hunt
// https://usaco.org/index.php?page=viewproblem2&cpid=1564
//
// N <= 20, so there are only 2^N boards. A board is described by the bitmask A of
// its M-cells (every other cell is an O).
//
//   score(A) = sum over x in A of  g_x(complement of A)
//   g_x(T)   = number of moooves (x, y, z) whose "OO part" {y, z} is a subset of T
//
// For each first cell x we drop every move (x, y, z) onto the mask {y, z}, then run a
// subset-sum ("SOS") DP so that g_x[T] counts all moves with {y, z} inside T.
// Accumulating over x gives score[A] for every board; then take the max and count it.
//
// Time: O(N * N * 2^N) ~ 4e8 very simple operations for N = 20.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> pairs(n);  // pairs[x] = masks {y, z} of moves starting at x
    for (int i = 0; i < k; i++) {
        int x, y, z;
        cin >> x >> y >> z;
        pairs[x - 1].push_back((1 << (y - 1)) | (1 << (z - 1)));
    }

    const int full = (1 << n) - 1;
    vector<int> score(1 << n, 0);
    vector<int> g(1 << n);

    for (int x = 0; x < n; x++) {
        fill(g.begin(), g.end(), 0);
        for (int m : pairs[x]) g[m]++;

        // subset-sum over every bit except x (no pair of x's moves contains bit x)
        for (int bit = 0; bit < n; bit++) {
            if (bit == x) continue;
            for (int mask = 0; mask <= full; mask++)
                if (mask >> bit & 1) g[mask] += g[mask ^ (1 << bit)];
        }

        for (int mask = 0; mask <= full; mask++)
            if (mask >> x & 1)  // x is an M-cell, so the O-cells are the complement
                score[mask] += g[full ^ mask];
    }

    int best = -1;
    long long ways = 0;
    for (int mask = 0; mask <= full; mask++) {
        if (score[mask] > best) best = score[mask], ways = 1;
        else if (score[mask] == best) ways++;
    }
    cout << best << " " << ways << "\n";
    return 0;
}
