#include <bits/stdc++.h>
using namespace std;
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"

int main() {
  const int n = 200000;
  Seg2D s(n, n);
  // Sparse diagonal points: rectangle answers are counts of multiples
  // of ten in the intersection of its x and y intervals.
  for (int i = 0; i < n; i += 10) s.update(i, i + 1, i, i + 1, 1);
  assert(s.query(0, n, 0, n) == n / 10);
  mt19937 rng(603);
  for (int q = 0; q < n; ++q) {
    int xl = rng() % (n + 1), xr = rng() % (n + 1);
    int yl = rng() % (n + 1), yr = rng() % (n + 1);
    if (xl > xr) swap(xl, xr);
    if (yl > yr) swap(yl, yr);
    int l = max(xl, yl), r = min(xr, yr);
    int want = l < r ? (r + 9) / 10 - (l + 9) / 10 : 0;
    assert(s.query(xl, xr, yl, yr) == want);
  }
  cout << "dynamic 2d scale: PASS\n";
}
