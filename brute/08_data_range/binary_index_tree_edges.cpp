#include <bits/stdc++.h>
using namespace std;

#define MAXN 64
#include "../../codebook/3_Data_Structure/Binary_Index_Tree.cpp"

[[noreturn]] static void fail(int tc, const string &what, int got, int want) {
  cerr << "Binary_Index_Tree mismatch: tc=" << tc << " " << what
       << " got=" << got << " want=" << want << '\n';
  exit(1);
}

static void verify(Binary_Index_Tree &tree, const vector<int> &a, int tc) {
  if (tree.query(0) != 0) fail(tc, "empty prefix", tree.query(0), 0);
  int sum = 0;
  for (int r = 1; r < (int)a.size(); ++r) {
    sum += a[r];
    if (tree.query(r) != sum) fail(tc, "prefix " + to_string(r), tree.query(r), sum);
  }
}

static void run_case(const vector<int> &initial, int tc, mt19937 &rng) {
  const int n = (int)initial.size() - 1;
  int data[MAXN + 1] = {};
  for (int i = 1; i <= n; ++i) data[i] = initial[i];

  Binary_Index_Tree tree;
  tree.init(n, data);
  vector<int> a = initial;
  verify(tree, a, tc);

  auto range_add = [&](int l, int r, int v) {
    tree.range_add(l, r, v);
    for (int i = l; i <= r; ++i) a[i] += v;
    verify(tree, a, tc);
  };

  // Full and singleton ranges exercise both r + 1 == n + 1 and l == r.
  range_add(1, n, -7);
  range_add(n, n, 11);
  range_add(1, 1, -5);

  for (int q = 0; q < 180; ++q) {
    int l = 1 + (int)(rng() % n), r = 1 + (int)(rng() % n);
    if (l > r) swap(l, r);
    int v = (int)(rng() % 25) - 12;
    switch (q % 3) {
      case 0:
        tree.modify(l, v);
        a[l] += v;
        break;
      case 1:
        tree.suf_modify(l, v);
        for (int i = l; i <= n; ++i) a[i] += v;
        break;
      default:
        tree.range_add(l, r, v);
        for (int i = l; i <= r; ++i) a[i] += v;
        break;
    }
    verify(tree, a, tc);
  }
}

static void enumerate(vector<int> &a, int at, int &tc, mt19937 &rng) {
  if (at == (int)a.size()) {
    run_case(a, tc++, rng);
    return;
  }
  for (int x : {-3, 0, 4}) {
    a[at] = x;
    enumerate(a, at + 1, tc, rng);
  }
}

int main() {
  mt19937 rng(0xB17E);  // fixed seed, including repeated and negative updates
  int tc = 0;
  for (int n = 1; n <= 5; ++n) {
    vector<int> a(n + 1);
    enumerate(a, 1, tc, rng);
  }

  for (int test = 0; test < 700; ++test) {
    int n = 1 + (int)(rng() % MAXN);
    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) a[i] = (int)(rng() % 2001) - 1000;
    run_case(a, tc++, rng);
  }
  return 0;
}
