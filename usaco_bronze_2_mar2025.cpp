//USACO Bronze Problem 1 | Mar 2025 | More Cow Photos
#include <iostream>
#include <vector>
 
int main() {
  int t;
  std::cin >> t;
  for(int cn = 1; cn <= t; cn++) {
    int n;
    std::cin >> n;
    std::vector<int> f(n+1);
    for(int i = 0; i < n; i++) {
      int x;
      std::cin >> x;
      f[x]++;
    }
    int ret = 0;
    bool seen = false;
    for(int i = n; i > 0; i--) {
      if(f[i] == 0) continue;
      if(!seen) ret++;
      else if(f[i] >= 2) ret += 2;
      seen = true;
    }
    std::cout << ret << "\n";
  }
}
