// USACO 2026 Second Contest, Gold 1 -- Balancing the Barns
// https://usaco.org/index.php?page=viewproblem2&cpid=1569
//
// Let k[i] >= 0 be the number of transfers at barn i (sum k[i] = K). Then
//     imbalance = max_i (a[i] - k[i]) - min_i (b[i] + k[i]).
// Every transfer lowers a[i] and raises b[i], so it can never hurt: if we can reach
// imbalance <= D with AT MOST K transfers, the leftover transfers can be dumped anywhere.
//
// Binary search on D (feasibility is monotone). To test D: if the final maximum hay is M
// then barn i needs   k[i] >= max(0, a[i] - M, M - D - b[i]),
// so   cost(M) = sum_i max(0, a[i] - M, M - D - b[i])   and D is feasible iff
// min_M cost(M) <= K. cost is a sum of convex functions, hence convex in M, so the best
// M is found by a second binary search on the slope cost(M+1) - cost(M) >= 0.
//
// Numbers get large (K up to 1e18, answers as low as about -2e18), so costs use
// __int128. Time: O(N * log^2(range)) ~ 62 * 62 * N.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

int n;
ll K;
vector<ll> a, b;

lll cost(ll M, ll D) {
    lll total = 0;
    for (int i = 0; i < n; i++) {
        ll need = max<ll>(0, max(a[i] - M, M - D - b[i]));
        total += need;
    }
    return total;
}

bool feasible(ll D) {
    // smallest M in [lo, hi] with cost(M+1) >= cost(M); cost is convex so that M minimises it
    ll lo = -(ll)1e18 - 5, hi = *max_element(a.begin(), a.end());
    while (lo < hi) {
        ll mid = lo + (hi - lo) / 2;
        if (cost(mid + 1, D) >= cost(mid, D)) hi = mid;
        else lo = mid + 1;
    }
    return cost(lo, D) <= (lll)K;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        cin >> n >> K;
        a.assign(n, 0);
        b.assign(n, 0);
        for (auto &v : a) cin >> v;
        for (auto &v : b) cin >> v;

        // D = max(a) - min(b) needs no transfers, so it is always feasible
        ll hi = *max_element(a.begin(), a.end()) - *min_element(b.begin(), b.end());
        ll lo = -(ll)2e18 - (ll)2e9;  // below any achievable imbalance
        while (lo < hi) {
            ll mid = lo + (hi - lo) / 2;
            if (feasible(mid)) hi = mid;
            else lo = mid + 1;
        }
        cout << lo << "\n";
    }
    return 0;
}
