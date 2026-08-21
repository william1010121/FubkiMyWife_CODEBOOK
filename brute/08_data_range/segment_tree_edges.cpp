#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/Segment_Tree.cpp"

[[noreturn]] static void fail(int tc, int l, int r, int got, int want) {
  cerr << "Segment_Tree mismatch: tc=" << tc << " range=[" << l << ',' << r
       << "] got=" << got << " want=" << want << '\n';
  exit(1);
}

static void check_all(Segment_Tree &tree, const vector<int> &a, int tc) {
  const int n = (int)a.size() - 1;
  for (int l = 1; l <= n; ++l) {
    int want = numeric_limits<int>::min();
    for (int r = l; r <= n; ++r) {
      want = max(want, a[r]);
      int got = tree.query(l, r);
      if (got != want) fail(tc, l, r, got, want);
    }
  }
}

static void run_case(vector<int> a, int tc, mt19937 &rng, bool exhaustive) {
  const int n = (int)a.size() - 1;
  Segment_Tree tree;
  tree.init(1, n, a.data());
  check_all(tree, a, tc);

  if (exhaustive) {
    for (int l = 1; l <= n; ++l) {
      for (int r = l; r <= n; ++r) {
        for (int x : {-6, 0, 5}) {
          tree.modify(l, r, x);
          for (int i = l; i <= r; ++i) a[i] += x;
          check_all(tree, a, tc);
        }
      }
    }
    return;
  }

  for (int q = 0; q < 260; ++q) {
    int l = 1 + (int)(rng() % n), r = 1 + (int)(rng() % n);
    if (l > r) swap(l, r);
    if (rng() & 1) {
      int x = (int)(rng() % 101) - 50;
      tree.modify(l, r, x);
      for (int i = l; i <= r; ++i) a[i] += x;
    } else {
      int want = *max_element(a.begin() + l, a.begin() + r + 1);
      int got = tree.query(l, r);
      if (got != want) fail(tc, l, r, got, want);
    }
    // Every point and the full range are checked after every operation.
    for (int i = 1; i <= n; ++i) {
      if (tree.query(i, i) != a[i]) fail(tc, i, i, tree.query(i, i), a[i]);
    }
    int full = *max_element(a.begin() + 1, a.end());
    if (tree.query(1, n) != full) fail(tc, 1, n, tree.query(1, n), full);
  }
}

static void enumerate(vector<int> &a, int at, int &tc, mt19937 &rng) {
  if (at == (int)a.size()) {
    run_case(a, tc++, rng, true);
    return;
  }
  for (int x : {-2, 0, 3}) {
    a[at] = x;
    enumerate(a, at + 1, tc, rng);
  }
}

int main() {
  mt19937 rng(0x5E6);  // fixed seed
  int tc = 0;
  for (int n = 1; n <= 5; ++n) {
    vector<int> a(n + 1);
    enumerate(a, 1, tc, rng);
  }
  for (int test = 0; test < 450; ++test) {
    int n = 1 + (int)(rng() % 40);
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) a[i] = (int)(rng() % 2001) - 1000;
    run_case(a, tc++, rng, false);
  }
  return 0;
}
