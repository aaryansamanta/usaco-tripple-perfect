//USACO 2025 Mar Bronze 2 | Compatible Pairs
#include <bits/stdc++.h>
using namespace std;

template <class T> using V = vector<T>;
#define all(x) begin(x), end(x)

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int N, A, B;
    cin >> N >> A >> B;

    map<int, int> cnt;
    while (N--) {
        int n, id;
        cin >> n >> id;
        cnt[id] = n;
    }
    map<int, set<int>> adj;
    vector<int> cands;
    for (auto [id, _] : cnt) {
        cands.push_back(id);
        adj[id] = {};
        for (int s : {A, B})
            if (cnt.count(s - id)) adj[id].insert(s - id);
    }
    int64_t ans = 0;
    while (!cands.empty()) {
        int x = cands.back();
        cands.pop_back();
        if (adj.at(x).size() != 1) continue;
        int y = *begin(adj.at(x));
        if (x == y) {
            ans += cnt.at(x) / 2;
        } else {
            int pairs = min(cnt.at(x), cnt.at(y));
            ans += pairs;
            cnt.at(y) -= pairs;
        }
        // note: cnt[x] is no longer used in any future steps,
        // so its value doesn't matter
        adj.at(x).erase(y);
        adj.at(y).erase(x);
        cands.push_back(y);
    }
    // sanity check: no edges remaining
    for (const auto &[id, s] : adj) assert(s.empty());
    cout << ans << "\n";
}
