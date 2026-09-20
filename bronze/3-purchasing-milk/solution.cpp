// USACO 2026 Second Contest, Bronze 3 -- Purchasing Milk
// https://usaco.org/index.php?page=viewproblem2&cpid=1565
//
// Deal i sells 2^(i-1) buckets for a[i]. Two facts make this easy:
//
//  1. Only the first 31 deals matter: 2^30 > 10^9 >= x, so deal 31 alone already
//     covers any query, and later deals cost even more.
//  2. Normalise the prices: c[i] = min(a[i], 2 * c[i-1]) (two copies of the previous
//     deal give the same buckets). Now c never decreases and c[i] <= 2 * c[i-1], so
//     using a deal twice is never better than using the next-bigger deal once.
//
// Hence an optimal purchase uses at most one of each smaller deal, and the "binary
// representation" of x can be processed greedily from the biggest deal down. At every
// level we also consider "round up here and stop" as a candidate answer.
//
// Time: O((N + Q * 31)).
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &v : a) cin >> v;

    int m = min(n, 31);  // deals that can matter
    vector<ll> c(m);
    for (int i = 0; i < m; i++) c[i] = (i == 0) ? a[0] : min(a[i], 2 * c[i - 1]);

    while (q--) {
        ll x;
        cin >> x;

        ll best = LLONG_MAX;  // cheapest candidate found
        ll spent = 0;         // cost of the exact part chosen so far
        ll left = x;          // buckets still to cover exactly
        for (int i = m - 1; i >= 0; i--) {
            ll size = 1LL << i;
            ll copies = left / size;
            best = min(best, spent + (copies + 1) * c[i]);  // round up with this deal
            spent += copies * c[i];
            left %= size;
        }
        if (left == 0) best = min(best, spent);  // x was hit exactly
        cout << best << "\n";
    }
    return 0;
}
