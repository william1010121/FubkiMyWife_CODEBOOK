#include <bits/stdc++.h>
using namespace std;
#define X first
#define Y second
#define SZ(x) ((int)(x).size())
#include "../../../codebook/3_Data_Structure/min_heap.cpp"

int main() {
  mt19937 rng(11);
  for (int tc = 0; tc < 1000; ++tc) {
    min_heap<int, int> a, b;
    vector<pair<int, int>> va, vb;
    for (int i = 0; i < 40; ++i) {
      pair<int, int> z{(int)(rng() % 101) - 50, i};
      if (i & 1) b.push(z), vb.push_back(z);
      else a.push(z), va.push_back(z);
    }
    int da = (int)(rng() % 21) - 10, db = (int)(rng() % 21) - 10;
    a.add_lazy(da), b.add_lazy(db);
    for (auto &z : va) z.first += da;
    for (auto &z : vb) z.first += db;
    a.join(b), va.insert(va.end(), vb.begin(), vb.end());
    while (!va.empty()) {
      auto it = min_element(va.begin(), va.end());
      assert(a.top() == *it);
      va.erase(it), a.pop();
    }
    assert(a.empty() && b.empty());
  }
}
