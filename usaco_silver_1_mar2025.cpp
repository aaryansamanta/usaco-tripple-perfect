//USACO 2025 Mar Silver 1 | Sequence Construction
#include <iostream>
#include <vector>

using namespace std;

int main(){
	cin.tie(0) -> sync_with_stdio(0);
	int T; cin >> T;
	while(T--){
		int m, k; cin >> m >> k;
		vector<int> a;
		int x = 0;
		for(int i = 0; i<5; i++){
			if(k&(1 << i)){
				a.push_back((1 << (1 << i)) - 1);
				x += a.back();
			}
		}

		int d = m - x;
		vector<int> b;
		int flag = 0; 
		if(d < 0){
			flag = 1;
		}else if(d >= 2){
			if(d%2 == 0){
				b = {d/2, d/2};
			}else{
				b = {1, 2, (d-3)/2, (d-3)/2};
			}
		}else if(d == 0){
			b = {};
		}else{
			if(a[0] == 1){
				a[0] = 2;
			}else{
				flag = -1;
			}
		}

		if(flag){
			cout << "-1\n";
		}else{
			cout << a.size() + b.size() << "\n";
			for(int i : a) 
				cout << i << " ";
			for(int i : b)
				cout << i << " " ;
			cout << "\n";
		}
	}
}
