#include <bits/stdc++.h>
using namespace std;
#define MAXN 128
#include "../../../codebook/3_Data_Structure/Binary_Index_Tree.cpp"

int main() {
  mt19937 rng(15);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1 + rng() % 80;
    int init[MAXN + 1] = {}, raw[MAXN + 1] = {};
    for (int i = 1; i <= n; ++i) init[i] = raw[i] = (int)(rng() % 21) - 10;
    Binary_Index_Tree bit;
    bit.init(n, init);
    for (int q = 0; q < 300; ++q) {
      int x = 1 + rng() % n, v = (int)(rng() % 21) - 10;
      bool point = rng() & 1;
      if (point) {
        bit.modify(x, v), raw[x] += v;
      } else {
        bit.suf_modify(x, v);
        for (int i = x; i <= n; ++i) raw[i] += v;
      }
      for (int r = 1; r <= n; ++r) {
        int want = accumulate(raw + 1, raw + r + 1, 0);
        if (bit.query(r) != want) {
          cerr << "BIT mismatch tc=" << tc << " n=" << n << " q=" << q
               << " r=" << r << " x=" << x << " v=" << v
               << " op=" << (point ? "point" : "suffix")
               << " got=" << bit.query(r) << " want=" << want << '\n';
          return 1;
        }
      }
    }
  }
}
