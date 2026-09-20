// USACO 2026 Second Contest, Gold 2 -- Lexicographically Smallest Path
// https://usaco.org/index.php?page=viewproblem2&cpid=1570
//
// Think of the graph as an automaton and follow the *greedy word*: at every step take
// the smallest letter available from ANY vertex we could currently be at.
//   X_0 = {1};  c_t = smallest letter on an edge leaving X_{t-1};
//   X_t = every vertex reached from X_{t-1} by an edge labelled c_t.
// Every vertex can finish the walk (the graph is connected), and a word that stops early
// is smaller than any extension of it, so the smallest walk to v exists iff v ever shows
// up in some X_t, and its length is the FIRST such t. Otherwise the answer is -1.
//
// Simulating step by step could take forever, so notice two things:
//   1. The letters c_t never increase (a vertex we just reached has an edge with the
//      letter we used to get there), so there are at most 26 "phases" of a single letter.
//   2. Inside a phase with letter c, X_t is just "which vertices can be reached from the
//      phase's start set by a walk of exactly t steps in the c-labelled subgraph G_c".
//      A BFS on (vertex, parity) gives D[v][p] = the shortest such walk with parity p,
//      and v is in X_{t0+s} iff s >= D[v][p] with s = p (mod 2).
// The phase ends the first time X contains a vertex having a smaller letter; only those
// vertices matter for the next phase. If that never happens, the phase lasts forever and
// we just record first-appearance times = t0 + min(D[v][0], D[v][1]).
//
// Time: O(26 * (N + M)).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> eu(m), ev(m), deg(n + 1, 0);
        vector<int> el(m);
        for (int i = 0; i < m; i++) {
            char ch;
            cin >> eu[i] >> ev[i] >> ch;
            eu[i]--, ev[i]--;
            el[i] = ch - 'a';
            deg[eu[i]]++, deg[ev[i]]++;  // a self-loop appears twice, which is harmless
        }
        // CSR adjacency: (neighbour, letter)
        vector<int> off(n + 1, 0);
        for (int v = 0; v < n; v++) off[v + 1] = off[v] + deg[v];
        vector<int> to(2 * m), let(2 * m), pos(off.begin(), off.end() - 1);
        vector<int> mn(n, 26);  // smallest letter on any edge at v
        for (int i = 0; i < m; i++) {
            to[pos[eu[i]]] = ev[i], let[pos[eu[i]]++] = el[i];
            to[pos[ev[i]]] = eu[i], let[pos[ev[i]]++] = el[i];
            mn[eu[i]] = min(mn[eu[i]], el[i]);
            mn[ev[i]] = min(mn[ev[i]], el[i]);
        }

        const int INF = INT_MAX;
        vector<long long> first(n, -1);
        first[0] = 0;
        vector<int> D[2] = {vector<int>(n, INF), vector<int>(n, INF)};
        vector<int> frontier = {0};  // vertices of X at time t0 that can still matter
        long long t0 = 0;

        while (true) {
            int c = 26;
            for (int v : frontier) c = min(c, mn[v]);
            if (c == 26) break;  // no edges at all (n == 1)

            // parity BFS from the frontier inside the subgraph of letter c
            vector<pair<int, int>> queue;  // (vertex, parity)
            for (int v : frontier) D[0][v] = 0, queue.push_back({v, 0});
            for (size_t h = 0; h < queue.size(); h++) {
                auto [v, p] = queue[h];
                for (int e = off[v]; e < off[v + 1]; e++) {
                    if (let[e] != c) continue;
                    int w = to[e];
                    if (D[p ^ 1][w] == INF) {
                        D[p ^ 1][w] = D[p][v] + 1;
                        queue.push_back({w, p ^ 1});
                    }
                }
            }

            // the phase lasts L steps: until some vertex with a smaller letter shows up
            int L = INF;
            for (auto [v, p] : queue)
                if (mn[v] < c) L = min(L, D[p][v]);

            for (auto [v, p] : queue) {
                int d = D[p][v];
                if (d <= L && first[v] < 0) first[v] = t0 + d;
                // (each vertex appears once or twice in the queue; the smaller d wins
                //  because BFS visits states in increasing order of d)
            }

            vector<int> next;
            if (L != INF)
                for (auto [v, p] : queue)
                    if (mn[v] < c && p == (L & 1) && D[p][v] == L) next.push_back(v);

            for (auto [v, p] : queue) D[p][v] = INF;  // reset for the next phase
            if (L == INF) break;
            t0 += L;
            frontier = next;
        }

        for (int v = 0; v < n; v++) cout << first[v] << " \n"[v + 1 == n];
    }
    return 0;
}
