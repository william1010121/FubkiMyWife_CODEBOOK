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

// Verify retained-lazy invariants and push at every allocated level.
static void push_all(Seg &s, Seg::Node *o, int l, int r) {
  if (!o) return;
  long long children = (o->l ? o->l->sum : 0LL)
                     + (o->r ? o->r->sum : 0LL);
  assert(o->sum == children + 1LL * o->lazy * (r - l));
  if (r - l == 1) return;
  s.push(o, l, r);
  children = (o->l ? o->l->sum : 0LL) + (o->r ? o->r->sum : 0LL);
  assert(o->lazy == 0 && o->sum == children);
  int m = l + (r - l) / 2;
  push_all(s, o->l, l, m), push_all(s, o->r, m, r);
}

static void int_boundaries() {
  Seg empty;
  empty.update(0, 0, INT_MIN);
  assert(empty.query(0, 0) == 0 && !empty.root);
  Seg leaf(1);
  leaf.update(0, 1, INT_MIN);
  assert(leaf.query(0, 1) == INT_MIN);
  leaf.push(leaf.root, 0, 1);
  assert(!leaf.root->l && !leaf.root->r);
  leaf.update(0, 1, INT_MAX);
  assert(leaf.query(0, 1) == -1);
  leaf.update(0, 1, 1);
  assert(leaf.query(0, 1) == 0);

  Seg2D singleton(1, 1);
  singleton.update(0, 1, 0, 1, INT_MIN);
  assert(singleton.query(0, 1, 0, 1) == INT_MIN);
  singleton.update(0, 1, 0, 1, INT_MAX);
  assert(singleton.query(0, 1, 0, 1) == -1);
  singleton.update(0, 1, 0, 1, 1);
  assert(singleton.query(0, 1, 0, 1) == 0);

  // Near-limit parent lazy plus negative children, in both dimensions.
  const int v = INT_MAX / 4;
  Seg2D grid(2, 2);
  vector<vector<int>> a(2, vector<int>(2, v));
  grid.update(0, 2, 0, 2, v);
  check_all(grid, a, 2, 2, "int-boundary-full");
  for (int q = 0; q < 2; ++q) {
    grid.update(1, 2, 0, 1, -v);
    a[1][0] -= v;
    check_all(grid, a, 2, 2, "int-boundary-partial");
  }
  grid.update(0, 2, 0, 2, -v);
  for (auto &row : a) for (int &x : row) x -= v;
  check_all(grid, a, 2, 2, "int-boundary-cancel");
}

int main() {
  int_boundaries();
  mt19937 rng(0x2D5E2D);
  mt19937 check_rng(0x2D57E55);
  // Zero additions on wide domains must remain allocation-free.
  {
    mt19937 zero_rng(0x2D0);
    const int n = 1000000000;
    Seg line(n);
    Seg2D grid(n, n);
    for (int q = 0; q < 200000; ++q) {
      int xl = zero_rng() % n, xr = xl + 1 + zero_rng() % (n - xl);
      int yl = zero_rng() % n, yr = yl + 1 + zero_rng() % (n - yl);
      line.update(xl, xr, 0);
      grid.update(xl, xr, yl, yr, 0);
    }
    assert(!line.root && !grid.root);
    assert(line.query(0, n) == 0 && grid.query(0, n, 0, n) == 0);
    // A subsequent nonzero update must work on the untouched trees.
    line.update(n - 1, n, 7);
    grid.update(n - 1, n, n - 1, n, -3);
    assert(line.query(0, n) == 7 && grid.query(0, n, 0, n) == -3);
    // Zero updates must not descend below already populated roots either.
    auto line_left = line.root->l;
    auto grid_left = grid.root->l;
    line.update(0, 1, 0);
    grid.update(0, 1, 0, 1, 0);
    assert(line.root->l == line_left && grid.root->l == grid_left);
    assert(!line_left && !grid_left);
    assert(line.query(0, n) == 7 && grid.query(0, n, 0, n) == -3);
  }
  // Midpoints must not overflow, even if all stored sums are small.
  {
    Seg edge(INT_MAX);
    edge.update(INT_MAX - 1, INT_MAX, 7);
    edge.update(0, 1, -3);
    assert(edge.query(0, INT_MAX) == 4);
    assert(edge.query(INT_MAX - 2, INT_MAX) == 7);
    edge.push(edge.root, 0, INT_MAX);
    assert(edge.query(0, 1) == -3);
    Seg lazy(INT_MAX);
    lazy.update(0, INT_MAX, 1);
    lazy.update(INT_MAX - 1, INT_MAX, -1);
    lazy.push(lazy.root, 0, INT_MAX);
    lazy.push(lazy.root->r, INT_MAX / 2, INT_MAX);
    assert(lazy.query(0, INT_MAX) == INT_MAX - 1);
    assert(lazy.query(INT_MAX - 2, INT_MAX) == 1);

    Seg2D grid(INT_MAX, INT_MAX);
    grid.update(INT_MAX - 1, INT_MAX, INT_MAX - 1, INT_MAX, 9);
    grid.update(0, 1, 0, 1, -2);
    assert(grid.query(0, INT_MAX, 0, INT_MAX) == 7);
    assert(grid.query(INT_MAX - 2, INT_MAX,
                      INT_MAX - 2, INT_MAX) == 9);
    assert(grid.query(0, 1, 0, 1) == -2);
    assert(grid.query(0, INT_MAX - 1, 1, INT_MAX) == 0);
  }
  // Wide lazy range: point queries must not materialize children.
  Seg wide(1000000000);
  wide.update(0, wide.n, 1);
  for (int q = 0; q < 200000; ++q) {
    int x = check_rng() % wide.n;
    assert(wide.query(x, x + 1) == 1);
  }
  assert(!wide.root->l && !wide.root->r);

  // The public push helper must still work with retained parent lazy.
  Seg one(31);
  vector<int> ref(one.n);
  for (int q = 0; q < 1000; ++q) {
    int l = check_rng() % (one.n + 1), r = check_rng() % (one.n + 1);
    if (l > r) swap(l, r);
    int v = int(check_rng() % 11) - 5;
    one.update(l, r, v);
    for (int i = l; i < r; ++i) ref[i] += v;
    if (q % 3 == 0) one.push(one.root, 0, one.n);
    if (q % 17 == 0) push_all(one, one.root, 0, one.n);
    for (int a = 0; a <= one.n; ++a)
      for (int b = a; b <= one.n; ++b)
        assert(one.query(a, b) ==
               accumulate(ref.begin() + a, ref.begin() + b, 0));
  }
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
