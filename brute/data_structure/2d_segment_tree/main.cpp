#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/2D_Segment_Tree.cpp"

int main() {
  mt19937 rng(17);
  for (int tc = 0; tc < 300; ++tc) {
    N = 1 + rng() % 12, M = 1 + rng() % 12;
    for (int i = 1; i <= N; ++i) for (int j = 1; j <= M; ++j)
      num[i][j] = 1 + rng() % 100;
    seg_1D st;
    st.init();
    for (int q = 0; q < 200; ++q) {
      int xl = 1 + rng() % N, xr = 1 + rng() % N;
      int yl = 1 + rng() % M, yr = 1 + rng() % M;
      if (xl > xr) swap(xl, xr);
      if (yl > yr) swap(yl, yr);
      int want = 0;
      for (int i = xl; i <= xr; ++i) for (int j = yl; j <= yr; ++j)
        want = max(want, num[i][j]);
      assert(st.query(xl, xr, yl, yr) == want);
    }
  }
}
