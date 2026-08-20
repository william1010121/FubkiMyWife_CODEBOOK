#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL << 60);
#include "../../../codebook/3_Data_Structure/LiChaoST.cpp"

int main() {
  mt19937 rng(7);
  for (int tc = 0; tc < 1000; ++tc) {
    const int n = 64;
    LiChao tree(n);
    vector<L> lines;
    for (int q = 0; q < 100; ++q) {
      if (rng() & 1) {
        L z((int)(rng() % 31) - 15, (int)(rng() % 101) - 50, q);
        tree.insert(z), lines.push_back(z);
      } else if (!lines.empty()) {
        int x = rng() % n;
        ll want = -INF;
        for (auto z : lines) want = max(want, z.at(x));
        assert(tree.query(x) == want);
      }
    }
    for (int x = 0; x < n; ++x) {
      ll want = -INF;
      for (auto z : lines) want = max(want, z.at(x));
      assert(tree.query(x) == want);
    }
  }
}
