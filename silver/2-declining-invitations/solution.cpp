// USACO 2026 Second Contest, Silver 2 -- Declining Invitations
// https://usaco.org/index.php?page=viewproblem2&cpid=1567
//
// Run the invitation process once with nobody declining. Then process the declines
// one at a time (forward), repairing the process after each one.
//
// Let inv[y] be the criterion under which contestant y is invited (INF if never).
// When contestant x declines and x was invited under criterion j, that criterion has
// one free slot. The best-ranked contestant y who satisfies j, is still present and is
// not invited before j (i.e. inv[y] > j) takes it. That leaves y's old slot inv[y]
// free (if y had one), and we repeat with that criterion. If y was not invited at all,
// the chain stops.
//
// Every repair moves some contestant to a STRICTLY EARLIER criterion from his own list,
// so the total number of moves is at most sum(n_i). For each criterion the candidates
// are scanned in rank order with a pointer that never moves back, because "present and
// inv[y] > j" can only turn from true to false.
//
// Time: O(N + C + sum n_i).
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;
    vector<int> f(c + 1), p(n);
    for (int j = 1; j <= c; j++) cin >> f[j];
    for (auto &v : p) cin >> v;

    // read criteria of every contestant, then bucket contestants per criterion
    // (iterating contestants by rank keeps every bucket sorted by rank)
    vector<int> start(n + 2), crit;
    for (int i = 1; i <= n; i++) {
        int k;
        cin >> k;
        start[i] = crit.size();
        while (k--) {
            int j;
            cin >> j;
            crit.push_back(j);
        }
    }
    start[n + 1] = crit.size();

    vector<int> off(c + 2, 0);
    for (int j : crit) off[j + 1]++;
    for (int j = 1; j <= c + 1; j++) off[j] += off[j - 1];
    vector<int> who(crit.size()), fill_pos(off.begin(), off.end());
    for (int i = 1; i <= n; i++)
        for (int e = start[i]; e < start[i + 1]; e++) who[fill_pos[crit[e]]++] = i;
    // contestants satisfying criterion j: who[off[j] .. off[j+1])

    const int INF = INT_MAX;
    vector<int> inv(n + 1, INF);
    vector<char> present(n + 1, 1);
    ll sum = 0;

    for (int j = 1; j <= c; j++) {  // the process with no declines
        int taken = 0;
        for (int e = off[j]; e < off[j + 1] && taken < f[j]; e++) {
            int y = who[e];
            if (inv[y] == INF) inv[y] = j, sum += y, taken++;
        }
    }

    vector<int> ptr(off.begin(), off.end() - 1);  // ptr[j] scans who[off[j] .. off[j+1])

    for (int i = 0; i < n; i++) {
        cout << sum << "\n";  // answer with p[0..i-1] declined
        int x = p[i];
        present[x] = 0;
        if (inv[x] == INF) continue;

        int j = inv[x];
        sum -= x;
        while (true) {  // repair chain
            int &e = ptr[j];
            while (e < off[j + 1] && !(present[who[e]] && inv[who[e]] > j)) e++;
            if (e == off[j + 1]) break;  // nobody can fill the slot
            int y = who[e];
            int old = inv[y];
            inv[y] = j;
            if (old == INF) {
                sum += y;
                break;
            }
            j = old;  // y left a slot behind; fill that one next
        }
    }
    return 0;
}
