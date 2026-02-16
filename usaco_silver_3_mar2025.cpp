//USACO 2025 Mar Bronze 3 | Ski Slope
#include <algorithm>
#include <iostream>
#include <vector>
 
const int MAXC = 11;
 
using namespace std;
 
int main() {
    int N; 
    cin >> N;
    int p[N], d[N], e[N];
 
    for (int i = 1; i < N; i++) {
        cin >> p[i] >> d[i] >> e[i];
        p[i]--;
    }
 
    long long E[N];
    E[0] = 0;
    for (int i = 1; i < N; i++) {
        E[i] = E[p[i]] + e[i];
    }
 
    vector<int> topC[N];
    topC[0] = vector<int>(MAXC, -1);
    for (int i = 1; i < N; i++) {
        topC[i] = topC[p[i]];
        topC[i].push_back(d[i]);
        sort(topC[i].begin(), topC[i].end(), greater<int>());
        topC[i].resize(MAXC);
    }
    
    pair<int, long long> DE[MAXC][N];
    for (int c = 0; c < MAXC; c++) {
        for (int i = 0; i < N; i++) {
            DE[c][i] = {topC[i][c], E[i]};
        }
        sort(DE[c], DE[c] + N);
        for (int i = 1; i < N; i++) {
            DE[c][i].second = max(DE[c][i].second, DE[c][i-1].second);
        }
    }
 
    int M;
    cin >> M;
 
    for (int i = 0; i < M; i++) {
        int S, C;
        cin >> S >> C;
        int idx = lower_bound(DE[C], DE[C] + N, make_pair(S+1, -1LL)) - DE[C];
        cout << DE[C][idx-1].second << endl;
    }
}