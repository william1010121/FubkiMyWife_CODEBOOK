#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/KDTree_useful.cpp"

int main() {
  using Tree = kd_tree<int, 2>;
  using P = Tree::point;
  mt19937 rng(12);
  Tree t(1'000'000);
  vector<P> live;
  auto makep = [&](int x, int y) { P p{{x, y}}; return p; };
  for (int q = 0; q < 300; ++q) {
    P p = makep((int)(rng() % 31) - 15, (int)(rng() % 31) - 15);
    bool exists = find(live.begin(), live.end(), p) != live.end();
    if (!exists && (rng() & 1)) t.insert(p), live.push_back(p);
    else if (!live.empty() && (rng() & 1)) {
      int id = rng() % live.size();
      assert(t.erase(live[id]));
      live.erase(live.begin() + id);
    }
    assert(t.size() == (int)live.size());
    if (!live.empty()) {
      P z = makep((int)(rng() % 31) - 15, (int)(rng() % 31) - 15);
      int want = 1'000'000;
      for (auto x : live) want = min(want, x.dist(z));
      assert(t.nearest(z, 1) == want);
    }
  }
}
