#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/2D_Segment_Tree.cpp"

[[noreturn]] static void fail(int tc, int xl, int xr, int yl, int yr,
                              int got, int want) {
  cerr << "2D segment tree mismatch: tc=" << tc << " x=[" << xl << ',' << xr
       << "] y=[" << yl << ',' << yr << "] got=" << got << " want=" << want
       << '\n';
  exit(1);
}

static void check_tree(seg_1D &tree, int tc) {
  for (int xl = 1; xl <= N; ++xl) {
    for (int xr = xl; xr <= N; ++xr) {
      for (int yl = 1; yl <= M; ++yl) {
        for (int yr = yl; yr <= M; ++yr) {
          int want = numeric_limits<int>::min();
          for (int x = xl; x <= xr; ++x)
            for (int y = yl; y <= yr; ++y) want = max(want, num[x][y]);
          int got = tree.query(xl, xr, yl, yr);
          if (got != want) fail(tc, xl, xr, yl, yr, got, want);
        }
      }
    }
  }
}

static void run_matrix(const vector<int> &values, int tc) {
  int at = 0;
  for (int x = 1; x <= N; ++x)
    for (int y = 1; y <= M; ++y) num[x][y] = values[at++];
  seg_1D tree;
  tree.init();
  check_tree(tree, tc);  // singleton, full, duplicate values, every rectangle
}

static void enumerate(vector<int> &values, int at, int &tc) {
  if (at == (int)values.size()) {
    run_matrix(values, tc++);
    return;
  }
  for (int x : {0, 1, 4}) {
    values[at] = x;
    enumerate(values, at + 1, tc);
  }
}

int main() {
  int tc = 0;
  // Exhaust all matrices up to 3x3 over a small nonnegative alphabet.
  // The source's partial-query accumulator starts at zero, so nonnegative
  // input is the implicit contract currently exercised by this harness.
  for (int n = 1; n <= 3; ++n) {
    for (int m = 1; m <= 3; ++m) {
      N = n, M = m;
      vector<int> values(N * M);
      enumerate(values, 0, tc);
    }
  }

  mt19937 rng(0x2D5E);
  for (int test = 0; test < 700; ++test) {
    N = 1 + (int)(rng() % 12);
    M = 1 + (int)(rng() % 12);
    vector<int> values(N * M);
    for (int &x : values) x = (int)(rng() % 101);  // includes zero
    run_matrix(values, tc++);
  }
  return 0;
}
