#include <bits/stdc++.h>
using namespace std;
#include "../../codebook/3_Data_Structure/Treap.cpp"

int check(node *o, vector<int> &keys) {
  if (!o) return 0;
  assert(!o->l || o->priority <= o->l->priority);
  assert(!o->r || o->priority <= o->r->priority);
  int left = check(o->l, keys);
  keys.push_back(o->data);
  int size = left + 1 + check(o->r, keys);
  assert(o->sz == size);
  return size;
}

int main() {
  const int n = 200000;
  srand(606);
  mt19937 rng(606);
  for (int mode = 0; mode < 3; ++mode) {
    vector<int> values(n);
    iota(values.begin(), values.end(), 0);
    if (mode == 0) shuffle(values.begin(), values.end(), rng);
    if (mode == 2) fill(values.begin(), values.end(), 7);
    node *root = nullptr;
    for (int x : values) insert(root, x);
    vector<int> keys;
    assert(check(root, keys) == n);
    assert(is_sorted(keys.begin(), keys.end()));
    for (int q = 0; q < n; ++q) {
      int k = 1 + rng() % n, x = rng() % n;
      assert(kth(root, k)->data == keys[k - 1]);
      int want = lower_bound(keys.begin(), keys.end(), x) - keys.begin();
      assert(Rank(root, x) == want);
    }
    for (int k : {0, 1, n / 2, n - 1, n}) {
      node *a, *b;
      split2(root, a, b, k);
      assert(sz(a) == k && sz(b) == n - k);
      root = merge(a, b);
    }
    for (int x : values) assert(erase(root, x));
    assert(!root);
  }
  // Equal priorities are legal: ties may choose either merge root.
  node *root = nullptr;
  for (int x = 0; x < 64; ++x) {
    auto o = new node(x); o->priority = 7;
    root = merge(root, o);
  }
  node *a, *b;
  split(root, a, b, 31), root = merge(a, b);
  vector<int> keys;
  assert(check(root, keys) == 64);
  for (int x = 0; x < 64; ++x) assert(erase(root, x));
  assert(!root);
  cout << "treap scale: PASS\n";
}
