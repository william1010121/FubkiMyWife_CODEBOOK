#include <bits/stdc++.h>
using namespace std;

int main() {
  for (int n = 1; n <= 10000; ++n) {
    vector<tuple<int,int,int>> got;
    auto enumerate = [&](int n) {
      for (int l = 1, r; l <= n; l = r + 1) {
        int x = n / l;
        r = n / x;
        got.emplace_back(l, r, x);
      }
    };
    enumerate(n);
    vector<tuple<int,int,int>> want;
    for (int l = 1; l <= n; ) {
      int x = n / l, r = l;
      while (r + 1 <= n && n / (r + 1) == x) ++r;
      want.emplace_back(l, r, x);
      l = r + 1;
    }
    if (got != want) {
      cerr << "floor enumeration mismatch n=" << n << '\n';
      return 1;
    }
  }
  cout << "PASS floor_enumeration n=1..10000\n";
}
