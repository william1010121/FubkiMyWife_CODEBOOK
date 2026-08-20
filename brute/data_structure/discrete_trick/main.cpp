#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/discrete_trick.cpp"

int main() {
  mt19937 rng(1);
  for (int tc = 0; tc < 2000; ++tc) {
    vector<int> a;
    for (int i = 0; i < 30; ++i) a.push_back((int)rng() % 11 - 5);
    sort(a.begin(), a.end());
    for (int x = -8; x <= 8; ++x) {
      int lo = 0, le = 0;
      while (lo < (int)a.size() && a[lo] < x) ++lo;
      while (le < (int)a.size() && a[le] <= x) ++le;
      assert(index_of(a, x) == lo);
      assert(count_lt(a, x) == lo);
      assert(count_le(a, x) == le);
    }
  }
  return 0;
}
