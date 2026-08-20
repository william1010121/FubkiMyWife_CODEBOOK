#include <bits/stdc++.h>
using namespace std;
#define RANGE_CHMIN_CHMAX_ADD_RANGE_SUM_NO_MAIN
#include "../../../codebook/3_Data_Structure/Range_Chmin_Chmax_Add_Range_Sum.cpp"

int main() {
  mt19937 rng(10);
  for (int tc = 0; tc < 300; ++tc) {
    int n = 1 + rng() % 30;
    vector<ll> ref(n + 1);
    for (int i = 1; i <= n; ++i) ref[i] = (int)(rng() % 41) - 20, a[i] = ref[i];
    build(1, n, 1);
    for (int q = 0; q < 200; ++q) {
      int l = 1 + rng() % n, r = 1 + rng() % n;
      if (l > r) swap(l, r);
      int type = rng() % 4;
      ll x = (int)(rng() % 31) - 15;
      if (type == 0) {
        modifymin(l, r, 1, n, 1, x);
        for (int i = l; i <= r; ++i) ref[i] = min(ref[i], x);
      } else if (type == 1) {
        modifymax(l, r, 1, n, 1, x);
        for (int i = l; i <= r; ++i) ref[i] = max(ref[i], x);
      } else if (type == 2) {
        modifyadd(l, r, 1, n, 1, x);
        for (int i = l; i <= r; ++i) ref[i] += x;
      } else {
        ll want = 0;
        for (int i = l; i <= r; ++i) want += ref[i];
        assert(query(l, r, 1, n, 1) == want);
      }
      for (int i = 1; i <= n; ++i) assert(query(i, i, 1, n, 1) == ref[i]);
    }
  }
}
