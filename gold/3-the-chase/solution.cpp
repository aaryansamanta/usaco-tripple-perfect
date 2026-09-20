// USACO 2026 Second Contest, Gold 3 -- The Chase
// https://usaco.org/index.php?page=viewproblem2&cpid=1571
//
// The road network is a functional graph: cycles with trees hanging into them.
//
// (1) All rests can be taken at the very start. Moving a rest earlier never lets a
//     farmer catch Bessie who was safe before, so for start b we only have to choose
//     k = number of rests at b, then move forever.
// (2) Resting at b is safe for k < t[b], where t[b] = first time any farmer reaches b
//     (multi-source BFS along the roads). t[b] = INF means Bessie can rest forever: -2.
// (3) After k rests Bessie reaches her tree's cycle root at time k + depth[b] and then
//     circles forever. Look at the cycle in a frame that rotates with the farmers: a
//     farmer standing on cycle position p at time t sits in "slot" (p - t) mod len, which
//     never changes, and so does Bessie's slot once she moves continuously. She is caught
//     iff her slot equals some farmer's slot. A farmer starting in the tree of root
//     position q at depth d has slot (q - d) mod len (cycle nodes: d = 0). Mark those slots
//     "bad". Any collision on the way up the tree would keep both together up to the root,
//     so checking the root slot is enough.
// (4) Bessie's slot is s(k) = (q_b - depth[b] - k) mod len. We want the largest k in
//     [0, t[b]-1] with s(k) not bad. Start from k = t[b]-1 and slide k down; each step
//     increases the slot by one, so precompute for every slot the distance to the next
//     good slot (cyclically). If none is good, or the distance overshoots k = 0, answer -1.
//
// Time: O(N).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, f;
    cin >> n >> f;
    vector<int> nxt(n);
    for (int i = 0; i < n; i++) cin >> nxt[i], nxt[i]--;
    vector<int> farmers(f);
    for (auto &s : farmers) cin >> s, s--;

    const int INF = INT_MAX;

    // t[v]: earliest time a farmer stands on v (multi-source BFS, out-degree 1)
    vector<int> t(n, INF);
    {
        vector<int> q;
        for (int s : farmers) t[s] = 0, q.push_back(s);
        for (size_t h = 0; h < q.size(); h++) {
            int u = q[h], v = nxt[u];
            if (t[v] == INF) t[v] = t[u] + 1, q.push_back(v);
        }
    }

    // peel leaves (Kahn) to find the cycle nodes; the peel order is leaves -> root
    vector<int> indeg(n, 0), order;
    for (int v = 0; v < n; v++) indeg[nxt[v]]++;
    for (int v = 0; v < n; v++) if (indeg[v] == 0) order.push_back(v);
    for (size_t h = 0; h < order.size(); h++) {
        int v = order[h];
        if (--indeg[nxt[v]] == 0) order.push_back(nxt[v]);
    }
    vector<char> onCycle(n);
    for (int v = 0; v < n; v++) onCycle[v] = indeg[v] > 0;

    // number the cycles: cyc[v], idx[v] (position, following the roads), len[cycle]
    vector<int> cyc(n, -1), idx(n, 0), base, len;  // base[c] = offset of c's slots
    int totalSlots = 0;
    for (int v = 0; v < n; v++) {
        if (!onCycle[v] || cyc[v] != -1) continue;
        int id = base.size(), L = 0;
        for (int u = v; cyc[u] == -1; u = nxt[u]) cyc[u] = id, idx[u] = L++;
        base.push_back(totalSlots);
        len.push_back(L);
        totalSlots += L;
    }

    // tree nodes: root (cycle node they drain into) and depth, parents before children
    vector<int> root(n), depth(n, 0);
    for (int v = 0; v < n; v++) if (onCycle[v]) root[v] = v;
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int v = order[i], u = nxt[v];
        root[v] = root[u];
        depth[v] = depth[u] + 1;
    }

    // bad slots = slots of all farmers
    vector<char> bad(totalSlots, 0);
    for (int s : farmers) {
        int r = root[s], id = cyc[r], L = len[id];
        int slot = ((idx[r] - depth[s]) % L + L) % L;
        bad[base[id] + slot] = 1;
    }

    // toGood[slot] = steps forward (cyclically) to the nearest good slot, INF if none
    vector<int> toGood(totalSlots, INF);
    for (int id = 0; id < (int)len.size(); id++) {
        int L = len[id], b0 = base[id];
        int run = INF;
        for (int i = 2 * L - 1; i >= 0; i--) {
            int s = i % L;
            run = bad[b0 + s] ? (run == INF ? INF : run + 1) : 0;
            if (i < L) toGood[b0 + s] = run;
        }
    }

    for (int b = 0; b < n; b++) {
        if (t[b] == INF) { cout << -2 << "\n"; continue; }
        long long k0 = (long long)t[b] - 1;  // most rests that resting in place allows
        if (k0 < 0) { cout << -1 << "\n"; continue; }
        int r = root[b], id = cyc[r], L = len[id];
        long long s0 = (((long long)idx[r] - depth[b] - k0) % L + L) % L;
        int y = toGood[base[id] + (int)s0];
        if (y == INF || y > k0) cout << -1 << "\n";
        else cout << k0 - y << "\n";
    }
    return 0;
}
