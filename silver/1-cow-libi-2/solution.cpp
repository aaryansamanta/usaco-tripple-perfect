// USACO 2026 Second Contest, Silver 1 -- Cow-libi 2
// https://usaco.org/index.php?page=viewproblem2&cpid=1566
//
// Key observation: a cow's statement about a neighbour only tells us whether that
// neighbour belongs to the SAME farmer as the cow.
//   * a truthful John cow says "J" exactly when the neighbour is John's  (same farmer)
//   * a lying Nhoj cow says "J" exactly when the neighbour is Nhoj's    (same farmer)
// So write L=1 if the left claim is 'J' and R=1 if the right claim is 'J'; then
//   L = [left neighbour has the same owner],  R = [right neighbour has the same owner].
//
// Let d[i] = 1 if seats i and i+1 (cyclically) share an owner, else 0. A cow sitting in
// seat i has (L, R) = (d[i-1], d[i]). So we need a cyclic 0/1 sequence d whose
// consecutive pairs (d[i-1], d[i]) match the multiset of cow pairs, and whose number of
// zeros is even (owners must flip an even number of times around a circle).
//
// Let a = #(1,1)  b = #(1,0)  b' = #(0,1)  c = #(0,0).  In a cyclic sequence the
// number of 1->0 and 0->1 transitions are equal, so we need b == b'. Then
//   * b == 0: d is constant, so a == 0 or c == 0
//   * b  > 0: d = 1^(a+1) 0^(c+1) (1 0)^(b-1) always works
//   * in both cases the number of zeros, b + c, must be even.
//
// Time: O(N).
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, wantConstruction;
    cin >> T >> wantConstruction;
    while (T--) {
        int n;
        string left, right;
        cin >> n >> left >> right;

        // bucket[L][R] = cows whose claims are (L, R)
        vector<int> bucket[2][2];
        for (int i = 0; i < n; i++)
            bucket[left[i] == 'J'][right[i] == 'J'].push_back(i + 1);

        int a = bucket[1][1].size(), b = bucket[1][0].size();
        int b2 = bucket[0][1].size(), c = bucket[0][0].size();

        bool ok = (b == b2) && ((b + c) % 2 == 0) && (b > 0 || a == 0 || c == 0);
        if (!ok) {
            cout << "NO\n";
            continue;
        }
        cout << "YES\n";
        if (!wantConstruction) continue;

        // 1. build the cyclic sequence d
        vector<int> d;
        if (b == 0) {
            d.assign(n, a > 0 ? 1 : 0);
        } else {
            d.assign(a + 1, 1);
            d.insert(d.end(), c + 1, 0);
            for (int i = 1; i < b; i++) d.push_back(1), d.push_back(0);
        }

        // 2. seat one cow per position i, chosen by the pair (d[i-1], d[i])
        vector<int> order(n);
        for (int i = 0; i < n; i++) {
            auto &pool = bucket[d[(i + n - 1) % n]][d[i]];
            order[i] = pool.back();
            pool.pop_back();
        }

        // 3. owners: walk around the circle, flipping owner whenever d[i] == 0
        string owner(n, 'J');
        for (int i = 0; i + 1 < n; i++)
            owner[i + 1] = (d[i] ? owner[i] : (owner[i] == 'J' ? 'N' : 'J'));

        for (int i = 0; i < n; i++) cout << order[i] << " \n"[i + 1 == n];
        cout << owner << "\n";
    }
    return 0;
}
