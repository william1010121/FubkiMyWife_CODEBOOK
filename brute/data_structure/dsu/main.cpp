#include <bits/stdc++.h>
using namespace std;
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/3_Data_Structure/DSU.cpp"

int main() {
  mt19937 rng(13);
  for (int tc = 0; tc < 1000; ++tc) {
    int n = 1 + rng() % 60;
    DSU d(n);
    vector<int> c(n);
    iota(c.begin(), c.end(), 0);
    for (int q = 0; q < 500; ++q) {
      int a = rng() % n, b = rng() % n;
      bool want = c[a] != c[b];
      bool got = d.Union(a, b);
      assert(got == want);
      if (want) {
        int x = c[b], y = c[a];
        for (int &z : c) if (z == x) z = y;
      }
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
        assert((d.boss(i) == d.boss(j)) == (c[i] == c[j]));
    }
  }
}
