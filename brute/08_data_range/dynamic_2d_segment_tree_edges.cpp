#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"

static void fail(const string &name, int n, int m, int xl, int xr, int yl,
                 int yr, int got, int want) {
  cerr << "Dynamic 2D segment tree mismatch case=" << name
       << " shape=" << n << 'x' << m << " x=[" << xl << ',' << xr
       << ") y=[" << yl << ',' << yr << ") got=" << got
       << " want=" << want << '\n';
  exit(1);
}

static int scan(const vector<vector<int>> &a, int xl, int xr, int yl, int yr) {
  int ans = 0;
  for (int x = xl; x < xr; ++x)
    for (int y = yl; y < yr; ++y) ans += a[x][y];
  return ans;
}

static void check_all(Seg2D &tree, const vector<vector<int>> &a, int n, int m,
                      const string &name) {
  for (int xl = 0; xl <= n; ++xl)
    for (int xr = xl; xr <= n; ++xr)
      for (int yl = 0; yl <= m; ++yl)
        for (int yr = yl; yr <= m; ++yr) {
          int want = scan(a, xl, xr, yl, yr);
          int got = tree.query(xl, xr, yl, yr);
          if (got != want) fail(name, n, m, xl, xr, yl, yr, got, want);
        }
}

static void run_case(int n, int m, const string &name, mt19937 &rng) {
  Seg2D tree(n, m);
  vector<vector<int>> a(n, vector<int>(m));
  check_all(tree, a, n, m, name + "-empty");

  // Every update is valid half-open input, including empty rectangles.
  for (int step = 0; step < 240; ++step) {
    int xl = rng() % (n + 1), xr = rng() % (n + 1);
    int yl = rng() % (m + 1), yr = rng() % (m + 1);
    if (xl > xr) swap(xl, xr);
    if (yl > yr) swap(yl, yr);
    int v = static_cast<int>(rng() % 9) - 4;
    tree.update(xl, xr, yl, yr, v);
    for (int x = xl; x < xr; ++x)
      for (int y = yl; y < yr; ++y) a[x][y] += v;

    // Full verification catches lazy propagation and all partial-overlap
    // combinations, while keeping the oracle deliberately elementary.
    check_all(tree, a, n, m, name + "-step-" + to_string(step));
  }
}

int main() {
  mt19937 rng(0x2D5E2D);
  // The smallest domains expose midpoint and leaf handling.  2x3 and 3x2
  // also exercise asymmetric recursion in both dimensions.
  run_case(0, 0, "zero-by-zero", rng);
  run_case(0, 3, "zero-by-three", rng);
  run_case(3, 0, "three-by-zero", rng);
  run_case(1, 1, "singleton", rng);
  run_case(1, 2, "one-by-two", rng);
  run_case(2, 1, "two-by-one", rng);
  run_case(2, 3, "small-asymmetric", rng);
  run_case(3, 2, "small-transpose", rng);
  for (int tc = 0; tc < 90; ++tc) {
    int n = 1 + rng() % 9;
    int m = 1 + rng() % 9;
    run_case(n, m, "random-" + to_string(tc), rng);
  }
  cout << "dynamic_2d_segment_tree_edges: PASS\n";
}
