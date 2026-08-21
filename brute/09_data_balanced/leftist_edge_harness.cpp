#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#include "../../codebook/3_Data_Structure/Leftist_Tree.cpp"

[[noreturn]] static void fail(const string &msg) {
  cerr << "leftist_edge_harness: " << msg << '\n';
  exit(1);
}

#define CHECK(cond) do { if (!(cond)) fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #cond); } while (0)

struct Audit {
  ll count = 0, total = 0;
};

static Audit audit(node *o) {
  if (!o) return {};
  Audit left = audit(o->l), right = audit(o->r);
  if (o->l) CHECK(o->data >= o->l->data);
  if (o->r) CHECK(o->data >= o->r->data);
  CHECK(V(o->r) <= V(o->l));
  CHECK(o->v == V(o->r) + 1);
  Audit all{left.count + right.count + 1,
            left.total + right.total + o->data};
  CHECK(o->sz == all.count);
  CHECK(o->sum == all.total);
  return all;
}

static void check_heap(node *root, const multiset<ll> &ref) {
  Audit a = audit(root);
  CHECK(a.count == (ll)ref.size());
  CHECK(a.total == accumulate(ref.begin(), ref.end(), 0LL));
  if (ref.empty()) {
    CHECK(root == nullptr);
  } else {
    CHECK(root != nullptr);
    CHECK(root->data == *ref.rbegin());
  }
}

static void drain(node *&root, multiset<ll> &ref) {
  while (!ref.empty()) {
    CHECK(root != nullptr);
    CHECK(root->data == *ref.rbegin());
    auto it = prev(ref.end());
    ref.erase(it);
    pop(root);
    check_heap(root, ref);
  }
  CHECK(root == nullptr);
}

static void execute_sequence(const array<int, 5> &ops) {
  node *h[2] = {nullptr, nullptr};
  multiset<ll> ref[2];
  for (int op : ops) {
    if (op < 6) {
      int which = op / 3;
      ll value = op % 3 - 1;
      h[which] = merge(h[which], new node(value));
      ref[which].insert(value);
    } else if (op == 6 || op == 7) {
      int which = op - 6;
      if (!ref[which].empty()) {
        ref[which].erase(prev(ref[which].end()));
        pop(h[which]);
      }
    } else {
      int dst = op == 8 ? 0 : 1;
      int src = 1 - dst;
      h[dst] = merge(h[dst], h[src]);
      ref[dst].insert(ref[src].begin(), ref[src].end());
      ref[src].clear();
      h[src] = nullptr;
    }
    check_heap(h[0], ref[0]);
    check_heap(h[1], ref[1]);
  }
  drain(h[0], ref[0]);
  drain(h[1], ref[1]);
}

static void enumerate_sequences(array<int, 5> &ops, int depth) {
  if (depth == (int)ops.size()) {
    execute_sequence(ops);
    return;
  }
  // Six inserts (three duplicate-friendly values in either heap), two pops,
  // and both merge directions: 10^5 short sequences are exhaustive here.
  for (int op = 0; op < 10; ++op) {
    ops[depth] = op;
    enumerate_sequences(ops, depth + 1);
  }
}

static void deterministic_edges() {
  node *a = nullptr, *b = nullptr;
  multiset<ll> ra, rb;
  check_heap(a, ra), check_heap(b, rb);
  a = merge(a, b); // empty <- empty
  check_heap(a, ra), check_heap(b, rb);
  b = merge(b, new node(0));
  rb.insert(0);
  b = merge(b, new node(0));
  rb.insert(0);
  a = merge(a, b); // empty <- duplicate singleton heap
  ra.insert(rb.begin(), rb.end()), rb.clear(), b = nullptr;
  check_heap(a, ra), check_heap(b, rb);
  a = merge(a, nullptr); // non-empty <- empty
  check_heap(a, ra);
  pop(a), ra.erase(prev(ra.end()));
  check_heap(a, ra);
  pop(a), ra.erase(prev(ra.end()));
  check_heap(a, ra);
  CHECK(a == nullptr);

  // Large magnitudes still fit in aggregate sums and exercise comparisons.
  node *large = merge(new node(-4000000000000LL), new node(9000000000000LL));
  multiset<ll> values{-4000000000000LL, 9000000000000LL};
  check_heap(large, values);
  drain(large, values);
}

static void randomized_stress() {
  mt19937_64 rng(0x09dada);
  const int heap_count = 6;
  for (int tc = 0; tc < 320; ++tc) {
    vector<node *> h(heap_count, nullptr);
    vector<multiset<ll>> ref(heap_count);
    for (int step = 0; step < 360; ++step) {
      int which = rng() % heap_count;
      int action = rng() % 100;
      if (action < 42) {
        ll x = (ll)(rng() % 31) - 15;
        if (rng() % 15 == 0) x = (rng() & 1) ? 1000000000000LL : -1000000000000LL;
        h[which] = merge(h[which], new node(x));
        ref[which].insert(x);
      } else if (action < 64) {
        int other = rng() % heap_count;
        if (other == which) other = (other + 1) % heap_count;
        h[which] = merge(h[which], h[other]);
        ref[which].insert(ref[other].begin(), ref[other].end());
        ref[other].clear();
        h[other] = nullptr;
      } else if (action < 82) {
        if (!ref[which].empty()) {
          ref[which].erase(prev(ref[which].end()));
          pop(h[which]);
        }
      } else {
        // Explicitly exercise no-op merges with an empty source/destination.
        int other = rng() % heap_count;
        if (other == which) other = (other + 1) % heap_count;
        if (rng() & 1) {
          h[which] = merge(h[which], nullptr);
        } else {
          h[which] = merge(nullptr, h[which]);
        }
        (void)other;
      }
      for (int i = 0; i < heap_count; ++i) check_heap(h[i], ref[i]);
    }
    for (int i = 0; i < heap_count; ++i) drain(h[i], ref[i]);
  }
}

int main() {
  deterministic_edges();
  array<int, 5> ops{};
  enumerate_sequences(ops, 0);
  randomized_stress();
  return 0;
}
