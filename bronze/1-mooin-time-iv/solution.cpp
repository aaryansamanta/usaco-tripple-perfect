// USACO 2026 Second Contest, Bronze 1 -- It's Mooin' Time IV
// https://usaco.org/index.php?page=viewproblem2&cpid=1563
//
// Idea: if f[i] is the FINAL letter at position i (M=0, O=1) and k[i] is the key
// pressed at step i, then f[i] = k[i] xor k[i+1] xor ... xor k[N-1]
// (every later 'O' flips position i once). Taking differences gives
// k[i] = f[i] xor f[i+1], with f[N] = 0. So the answer is always YES.
//
// Time: O(N) per test.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, k;
    cin >> T >> k;
    while (T--) {
        int n;
        string s;
        cin >> n >> s;

        cout << "YES\n";
        if (k == 1) {
            string keys(n, 'M');
            for (int i = 0; i < n; i++) {
                char after = (i + 1 < n) ? s[i + 1] : 'M';  // f[N] = 0 (an 'M')
                if (s[i] != after) keys[i] = 'O';           // press O iff f[i] != f[i+1]
            }
            cout << keys << "\n";
        }
    }
    return 0;
}
