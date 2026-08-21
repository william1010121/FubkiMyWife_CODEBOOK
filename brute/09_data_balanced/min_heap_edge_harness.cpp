#include <bits/stdc++.h>
using namespace std;

#define X first
#define Y second
#define SZ(x) ((int)(x).size())
#include "../../codebook/3_Data_Structure/min_heap.cpp"

using Item = pair<long long, int>;

[[noreturn]] static void fail(const string &msg) {
  cerr << "min_heap_edge_harness: " << msg << '\n';
  exit(1);
}

#define CHECK(cond) do { if (!(cond)) fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #cond); } while (0)

static void add_reference(multiset<Item> &ref, long long delta) {
  multiset<Item> next;
  for (auto [key, id] : ref) next.insert({key + delta, id});
  ref.swap(next);
}

static void check_heap(min_heap<long long, int> &heap,
                       const multiset<Item> &ref) {
  CHECK((int)heap.pq.size() == (int)ref.size());
  auto copy = heap.pq;
  multiset<Item> materialized;
  while (!copy.empty()) {
    Item x = copy.top();
    copy.pop();
    materialized.insert({x.first + heap.lazy, x.second});
  }
  CHECK(materialized == ref);
  CHECK(heap.empty() == ref.empty());
  if (!ref.empty()) CHECK(heap.top() == *ref.begin());
}

static void apply_action(array<min_heap<long long, int>, 2> &heap,
                         array<multiset<Item>, 2> &ref, int op) {
  switch (op) {
    case 0:
      heap[0].push({-1, 0}), ref[0].insert({-1, 0});
      break;
    case 1:
      heap[0].push({0, 0}), ref[0].insert({0, 0});
      break;
    case 2:
      heap[0].push({1, 1}), ref[0].insert({1, 1});
      break;
    case 3:
      heap[1].push({-1, 0}), ref[1].insert({-1, 0});
      break;
    case 4:
      heap[1].push({0, 0}), ref[1].insert({0, 0});
      break;
    case 5:
      heap[1].push({1, 1}), ref[1].insert({1, 1});
      break;
    case 6:
      heap[0].add_lazy(-2), add_reference(ref[0], -2);
      break;
    case 7:
      heap[1].add_lazy(3), add_reference(ref[1], 3);
      break;
    case 8:
      heap[0].join(heap[1]);
      ref[0].insert(ref[1].begin(), ref[1].end());
      ref[1].clear();
      break;
    case 9:
      heap[1].join(heap[0]);
      ref[1].insert(ref[0].begin(), ref[0].end());
      ref[0].clear();
      break;
    case 10:
      if (!ref[0].empty()) heap[0].pop(), ref[0].erase(ref[0].begin());
      break;
    case 11:
      if (!ref[1].empty()) heap[1].pop(), ref[1].erase(ref[1].begin());
      break;
  }
  check_heap(heap[0], ref[0]);
  check_heap(heap[1], ref[1]);
}

static void enumerate_sequences(array<int, 5> &ops, int depth) {
  if (depth == (int)ops.size()) {
    array<min_heap<long long, int>, 2> heap;
    array<multiset<Item>, 2> ref;
    for (int op : ops) apply_action(heap, ref, op);
    return;
  }
  // Pushes, lazy shifts, both join directions, and safe empty pops.
  for (int op = 0; op < 12; ++op) {
    ops[depth] = op;
    enumerate_sequences(ops, depth + 1);
  }
}

static void deterministic_edges() {
  array<min_heap<long long, int>, 2> heap;
  array<multiset<Item>, 2> ref;
  check_heap(heap[0], ref[0]), check_heap(heap[1], ref[1]);

  // Lazy offsets on empty heaps must not affect later absolute keys.
  heap[0].add_lazy(100), add_reference(ref[0], 100);
  heap[1].add_lazy(-100), add_reference(ref[1], -100);
  heap[0].push({7, 4}), ref[0].insert({7, 4});
  heap[1].push({7, 4}), ref[1].insert({7, 4});
  check_heap(heap[0], ref[0]), check_heap(heap[1], ref[1]);

  heap[0].join(heap[1]);
  ref[0].insert(ref[1].begin(), ref[1].end()), ref[1].clear();
  check_heap(heap[0], ref[0]), check_heap(heap[1], ref[1]);
  heap[0].join(heap[1]); // repeated merge with an empty source
  check_heap(heap[0], ref[0]), check_heap(heap[1], ref[1]);
  heap[1].join(heap[0]); // empty destination <- non-empty source
  ref[1].insert(ref[0].begin(), ref[0].end()), ref[0].clear();
  check_heap(heap[0], ref[0]), check_heap(heap[1], ref[1]);
  while (!ref[1].empty()) {
    CHECK(heap[1].top() == *ref[1].begin());
    heap[1].pop(), ref[1].erase(ref[1].begin());
  }
  check_heap(heap[1], ref[1]);
}

static void randomized_stress() {
  mt19937_64 rng(0x09EAULL);
  const int heap_count = 5;
  for (int tc = 0; tc < 650; ++tc) {
    vector<min_heap<long long, int>> heap(heap_count);
    vector<multiset<Item>> ref(heap_count);
    for (int step = 0; step < 340; ++step) {
      int a = rng() % heap_count;
      int action = rng() % 100;
      if (action < 35) {
        long long key = (long long)(rng() % 101) - 50;
        if (rng() % 20 == 0) key = (rng() & 1) ? 1000000000000LL : -1000000000000LL;
        int id = (int)(rng() % 9); // deliberately repeated Info values
        heap[a].push({key, id});
        ref[a].insert({key, id});
      } else if (action < 53) {
        long long delta = (long long)(rng() % 41) - 20;
        heap[a].add_lazy(delta);
        add_reference(ref[a], delta);
      } else if (action < 72) {
        int b = rng() % heap_count;
        if (a == b) b = (b + 1) % heap_count;
        heap[a].join(heap[b]);
        ref[a].insert(ref[b].begin(), ref[b].end());
        ref[b].clear();
      } else if (action < 90) {
        if (!ref[a].empty()) heap[a].pop(), ref[a].erase(ref[a].begin());
      } else {
        // Empty joins are a normal operation in meld-based algorithms.
        int b = rng() % heap_count;
        if (a == b) b = (b + 1) % heap_count;
        heap[a].join(heap[b]);
        ref[a].insert(ref[b].begin(), ref[b].end());
        ref[b].clear();
      }
      for (int i = 0; i < heap_count; ++i) check_heap(heap[i], ref[i]);
    }
    for (int i = 0; i < heap_count; ++i) {
      while (!ref[i].empty()) {
        CHECK(heap[i].top() == *ref[i].begin());
        heap[i].pop();
        ref[i].erase(ref[i].begin());
      }
      check_heap(heap[i], ref[i]);
    }
  }
}

int main() {
  deterministic_edges();
  array<int, 5> ops{};
  enumerate_sequences(ops, 0); // 12^5 deterministic short sequences.
  randomized_stress();
  return 0;
}
