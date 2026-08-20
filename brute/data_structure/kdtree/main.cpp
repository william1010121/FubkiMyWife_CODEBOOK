#include <bits/stdc++.h>
using namespace std;
const int maxn = 256;
struct point { int x, y; };
#include "../../../codebook/3_Data_Structure/KDTree.cpp"

int main() {
  mt19937 rng(8);
  for (int tc = 0; tc < 500; ++tc) {
    vector<point> p(1 + rng() % 80);
    for (auto &z : p) z = {(int)(rng() % 41) - 20, (int)(rng() % 41) - 20};
    kdt::init(p);
    for (int q = 0; q < 100; ++q) {
      point x{(int)(rng() % 41) - 20, (int)(rng() % 41) - 20};
      long long want = (1LL << 60);
      for (auto z : p) {
        long long d = 1LL * (z.x - x.x) * (z.x - x.x) +
                      1LL * (z.y - x.y) * (z.y - x.y);
        if (d) want = min(want, d);
      }
      assert(kdt::nearest(x) == want);
    }
  }
}
