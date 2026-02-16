//USACO Bronze Problem 3 | Mar 2025 | It's Mooin' Time III
#include <bits/stdc++.h>
using namespace std; 
 
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    int n, q;
    cin >> n >> q;
 
    string S;
    cin >> S;
 
    vector<vector<int>> left(n, vector<int>(26, -1));
    vector<vector<int>> right(n, vector<int>(26, n));
    vector<vector<int>> rightExclude(n, vector<int>(26, n));
    
    for (int i = 0; i < n; i++){
        if (i)
            left[i] = left[i - 1];
 
        left[i][S[i] - 'a'] = i;
    }
    for (int i = n - 1; i >= 0; i--){
        if (i + 1 < n)
            right[i] = right[i + 1];
 
        right[i][S[i] - 'a'] = i;
 
        // Calculate minimum right if we exclude each character
        int best = n, sbest = n;
 
        for (int c = 0; c < 26; c++){ 
            if (right[i][c] < best){
                sbest = best;
                best = right[i][c];
            }
            else if (right[i][c] < sbest)
                sbest = right[i][c];
        }
        for (int c = 0; c < 26; c++)
            rightExclude[i][c] = (right[i][c] == best ? sbest : best);
    }
    while (q--){
        int ql, qr;
        cin >> ql >> qr;
        ql--; qr--;
        
        long long best = -1;
 
        for (int rc = 0; rc < 26; rc++){
            int k = left[qr][rc];
            int i = rightExclude[ql][rc];
 
            if (i >= k)
                continue;

            for (int j : {left[(i + k) / 2][rc], right[(i + k) / 2][rc]})
                if (j > i and j < k)
                    best = max(best, 1LL * (k - j) * (j - i));
        }
        cout << best << "\n";
    }
}
