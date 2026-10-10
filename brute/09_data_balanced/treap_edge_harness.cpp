#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/Treap.cpp"

[[noreturn]] static void fail(const string &msg) {
  cerr << "treap_edge_harness: " << msg << '\n';
  exit(1);
}

#define CHECK(cond) do { if (!(cond)) fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #cond); } while (0)

static void collect(node *o, vector<int> &out) {
  if (!o) return;
  CHECK(o->sz == 1 + sz(o->l) + sz(o->r));
  CHECK(!o->l || o->priority <= o->l->priority);
  CHECK(!o->r || o->priority <= o->r->priority);
  collect(o->l, out);
  out.push_back(o->data);
  collect(o->r, out);
}

static void destroy(node *o) {
  if (!o) return;
  destroy(o->l);
  destroy(o->r);
  delete o;
}

static vector<int> as_vector(const multiset<int> &ref) {
  return vector<int>(ref.begin(), ref.end());
}

static void check_tree(node *root, const multiset<int> &ref) {
  vector<int> got;
  collect(root, got);
  vector<int> want = as_vector(ref);
  CHECK(got == want);
  CHECK(sz(root) == (int)want.size());
  vector<int> probes = {INT_MIN, INT_MAX};
  for (int key = -20; key <= 20; ++key) probes.push_back(key);
  for (int key : probes)
    CHECK(Rank(root, key) == (int)distance(ref.begin(), ref.lower_bound(key)));
  if (!want.empty()) {
    for (int i = 1; i <= (int)want.size(); ++i)
      CHECK(kth(root, i)->data == want[i - 1]);
  }
}

static node *make_tree(const vector<int> &values) {
  node *root = nullptr;
  for (int x : values) insert(root, x);
  return root;
}

static void check_split_operations(const vector<int> &values) {
  multiset<int> ref(values.begin(), values.end());
  node *root = make_tree(values);
  check_tree(root, ref);

  vector<int> keys = {INT_MIN, INT_MAX};
  for (int key = -20; key <= 20; ++key) keys.push_back(key);
  for (int key : keys) {
    node *left, *right;
    split(root, left, right, key);
    vector<int> a, b;
    collect(left, a), collect(right, b);
    vector<int> want_left, want_right;
    for (int x : values) (x <= key ? want_left : want_right).push_back(x);
    sort(want_left.begin(), want_left.end());
    sort(want_right.begin(), want_right.end());
    CHECK(a == want_left && b == want_right);
    root = merge(left, right);
    check_tree(root, ref);

    // Either split output may alias the input root variable.
    split(root, root, right, key);
    a.clear(), b.clear();
    collect(root, a), collect(right, b);
    CHECK(a == want_left && b == want_right);
    root = merge(root, right);
    split(root, left, root, key);
    a.clear(), b.clear();
    collect(left, a), collect(root, b);
    CHECK(a == want_left && b == want_right);
    root = merge(left, root);
    check_tree(root, ref);
  }

  for (int k = 0; k <= (int)values.size(); ++k) {
    node *left, *right;
    split2(root, left, right, k);
    vector<int> a, b;
    collect(left, a), collect(right, b);
    vector<int> want = as_vector(ref);
    CHECK(a == vector<int>(want.begin(), want.begin() + k));
    CHECK(b == vector<int>(want.begin() + k, want.end()));
    root = merge(left, right);
    check_tree(root, ref);
    split2(root, root, right, k);
    a.clear(), b.clear();
    collect(root, a), collect(right, b);
    CHECK(a == vector<int>(want.begin(), want.begin() + k));
    CHECK(b == vector<int>(want.begin() + k, want.end()));
    root = merge(root, right);
    split2(root, left, root, k);
    a.clear(), b.clear();
    collect(left, a), collect(root, b);
    CHECK(a == vector<int>(want.begin(), want.begin() + k));
    CHECK(b == vector<int>(want.begin() + k, want.end()));
    root = merge(left, root);
    check_tree(root, ref);
  }

  // interval(root, l, len) is a split-and-merge helper; exercise every
  // valid one-based interval, including an empty prefix/suffix.
  for (int l = 1; l <= (int)values.size() + 1; ++l) {
    int max_len = (int)values.size() - l + 1;
    if (max_len < 0) max_len = 0;
    for (int len = 0; len <= max_len; ++len) {
      interval(root, l, len);
      check_tree(root, ref);
    }
  }
  destroy(root);
}

static const vector<pair<bool, int>> exhaustive_ops = {
    {true, -1}, {true, 0}, {true, 1},
    {false, -1}, {false, 0}, {false, 1},
};

static void enumerate_sequences(node *&root, multiset<int> &ref, int depth) {
  check_tree(root, ref);
  if (depth == 7) return;
  for (auto [is_insert, x] : exhaustive_ops) {
    if (is_insert) {
      insert(root, x);
      ref.insert(x);
      enumerate_sequences(root, ref, depth + 1);
      CHECK(erase(root, x));
      auto it = ref.find(x);
      CHECK(it != ref.end());
      ref.erase(it);
    } else {
      bool got = erase(root, x);
      auto it = ref.find(x);
      bool want = it != ref.end();
      if (want) ref.erase(it);
      CHECK(got == want);
      enumerate_sequences(root, ref, depth + 1);
      if (want) insert(root, x), ref.insert(x);
    }
    check_tree(root, ref);
  }
}

static void randomized_stress() {
  mt19937 rng(0x09b1a1);
  const int values[] = {INT_MIN, -1000000, -7, -1, 0, 1, 7, 1000000, INT_MAX};
  for (int tc = 0; tc < 450; ++tc) {
    node *root = nullptr;
    multiset<int> ref;
    int steps = 120 + (int)(rng() % 160);
    for (int q = 0; q < steps; ++q) {
      int x;
      if (rng() % 4) x = values[rng() % size(values)];
      else x = (int)(rng() % 41) - 20;
      if (rng() % 100 < 56) {
        insert(root, x), ref.insert(x);
      } else {
        bool got = erase(root, x);
        auto it = ref.find(x);
        bool want = it != ref.end();
        if (want) ref.erase(it);
        CHECK(got == want);
      }
      check_tree(root, ref);
    }
    check_split_operations(as_vector(ref));
    destroy(root);
  }
}

int main() {
  srand(0x09b1a1);
  check_split_operations({});
  check_split_operations({0});
  check_split_operations({-3, -3, -3, 0, 0, 4, 4, 9});
  check_split_operations({INT_MIN, -1, 0, INT_MAX, 0, INT_MIN});

  node *root = nullptr;
  multiset<int> ref;
  enumerate_sequences(root, ref, 0);
  CHECK(root == nullptr && ref.empty());
  randomized_stress();
  // Larger duplicate/sorted inputs; verify every kth and node size.
  for (int mode = 0; mode < 3; ++mode) {
    node *tree = nullptr;
    multiset<int> want;
    for (int i = 0; i < 4096; ++i) {
      int key = mode == 0 ? 0 : mode == 1 ? i : -i;
      insert(tree, key), want.insert(key);
    }
    check_tree(tree, want);
    for (int k : {0, 1, 2048, 4095, 4096}) {
      node *a, *b;
      split2(tree, a, b, k);
      CHECK(sz(a) == k && sz(b) == 4096 - k);
      tree = merge(a, b);
      check_tree(tree, want);
    }
    while (!want.empty()) {
      int key = *want.begin();
      CHECK(erase(tree, key));
      want.erase(want.begin());
      CHECK(sz(tree) == (int)want.size());
    }
    CHECK(tree == nullptr && !erase(tree, 0));
  }
  return 0;
}
