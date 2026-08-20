#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
#define X first
#define Y second
#include "../../../codebook/3_Data_Structure/IntervalContainer.cpp"

static set<int> expand(const set<pii> &s) {
  set<int> r;
  for (auto [l, q] : s) for (int x = l; x < q; ++x) r.insert(x);
  return r;
}
int main() {
  mt19937 rng(3);
  for (int tc = 0; tc < 5000; ++tc) {
    set<pii> got;
    set<int> want;
    for (int op = 0; op < 100; ++op) {
      int l = rng() % 21 - 10, r = rng() % 21 - 10;
      if (l > r) swap(l, r);
      if (rng() & 1) {
        addInterval(got, l, r);
        for (int x = l; x < r; ++x) want.insert(x);
      } else {
        removeInterval(got, l, r);
        for (int x = l; x < r; ++x) want.erase(x);
      }
      assert(expand(got) == want);
      for (auto it = got.begin(); it != got.end(); ++it) {
        assert(it->first < it->second);
        auto nx = next(it);
        if (nx != got.end()) assert(it->second < nx->first);
      }
    }
  }
}
