#include <bits/stdc++.h>
using namespace std;

#define ALL(x) (x).begin(), (x).end()
#include "../../codebook/3_Data_Structure/DSU.cpp"

[[noreturn]] static void fail(const string &msg) {
  cerr << "dsu_edge_harness: " << msg << '\n';
  exit(1);
}

#define CHECK(cond) do { if (!(cond)) fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #cond); } while (0)

struct Reference {
  vector<int> label;
  explicit Reference(int n = 0) : label(n) { iota(label.begin(), label.end(), 0); }

  int find(int x) {
    return label[x] == x ? x : label[x] = find(label[x]);
  }

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    for (int &z : label) if (z == y) z = x;
    return true;
  }
};

static void check_dsu(DSU &got, Reference &ref) {
  CHECK(got.arr.size() == ref.label.size());
  for (int i = 0; i < (int)ref.label.size(); ++i) {
    int boss = got.boss(i);
    CHECK(0 <= boss && boss < (int)ref.label.size());
    CHECK((boss == got.boss(boss)));
  }
  for (int i = 0; i < (int)ref.label.size(); ++i)
    for (int j = 0; j < (int)ref.label.size(); ++j)
      CHECK((got.boss(i) == got.boss(j)) ==
            (ref.find(i) == ref.find(j)));
}

static void run_sequence(const array<pair<int, int>, 5> &ops) {
  DSU got(4);
  Reference ref(4);
  for (auto [x, y] : ops) {
    bool a = got.Union(x, y);
    bool b = ref.unite(x, y);
    CHECK(a == b);
    check_dsu(got, ref);
  }
}

static vector<pair<int, int>> all_pairs(int n) {
  vector<pair<int, int>> result;
  for (int i = 0; i < n; ++i)
    for (int j = i; j < n; ++j)
      result.push_back({i, j});
  return result;
}

static void enumerate_sequences(array<pair<int, int>, 5> &ops,
                                const vector<pair<int, int>> &choices,
                                int depth) {
  if (depth == (int)ops.size()) {
    run_sequence(ops);
    return;
  }
  for (auto edge : choices) {
    ops[depth] = edge;
    enumerate_sequences(ops, choices, depth + 1);
  }
}

static void deterministic_edges() {
  DSU empty;
  CHECK(empty.arr.empty());

  DSU one(1);
  CHECK(!one.Union(0, 0));
  CHECK(one.boss(0) == 0);
  CHECK(!one.Union(0, 0));

  // The implementation intentionally has no union-by-size; this produces a
  // long parent chain and tests recursive path compression directly.
  const int n = 4096;
  DSU chain(n);
  for (int i = 0; i + 1 < n; ++i) CHECK(chain.Union(i + 1, i));
  for (int i = 0; i < n; ++i) CHECK(chain.boss(i) == n - 1);
  for (int i = 0; i < n; ++i) CHECK(!chain.Union(i, (i * 37) % n));
  for (int i = 0; i < n; ++i) CHECK(chain.boss(i) == n - 1);
}

static void randomized_stress() {
  mt19937 rng(0x09d5u);
  for (int tc = 0; tc < 850; ++tc) {
    int n = rng() % 90;
    DSU got(n);
    Reference ref(n);
    check_dsu(got, ref);
    for (int q = 0; q < 650; ++q) {
      if (n == 0) break;
      int x = rng() % n, y = rng() % n;
      CHECK(got.Union(x, y) == ref.unite(x, y));
      if (q % 7 == 0) check_dsu(got, ref);
    }
    check_dsu(got, ref);
  }
}

int main() {
  deterministic_edges();
  array<pair<int, int>, 5> ops{};
  enumerate_sequences(ops, all_pairs(4), 0); // 10^5 ordered sequences.
  randomized_stress();
  return 0;
}
