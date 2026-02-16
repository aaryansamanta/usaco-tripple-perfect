//USACO Bronze Problem 1 | Mar 2025 | Hoof Paper Scissors Minus One
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    vector<string> data(N);
    for (int i = 0; i < N; i++) {
        cin >> data[i];
    }
    vector<vector<int>> beat(N, vector<int>(N, 0));
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < i; j++) {
            if (data[i][j] != 'D') {
                if (data[i][j] == 'W')
                    beat[i][j] = 1;
                else
                    beat[j][i] = 1;
            }
        }
    }
    
    while (M--) {
        int x, y;
        cin >> x >> y;
        x--; 
        y--;
        
        // Number of symbols b that can beat both x and y
        int winning = 0;
        for (int b = 0; b < N; b++) {
            if (beat[b][x] && beat[b][y])
                winning++;
        }
        
        int total_playable_pairs = N * N;
        int losing_pairs = (N - winning) * (N - winning);
        
        cout << total_playable_pairs - losing_pairs << "\n";
    }
    return 0;
}
