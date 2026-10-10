#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = LLONG_MAX;

#include "../../codebook/3_Data_Structure/LiChaoST.cpp"

static ll scan_max(const vector<L> &lines, ll x) {
  ll ans = -INF;
  for (L line : lines) {
    __int128 product = (__int128)line.m * x;
    __int128 value = product + line.k;
    assert(LLONG_MIN <= product && product <= LLONG_MAX);
    assert(-(__int128)INF <= value && value <= INF);
    ans = max(ans, (ll)value);
  }
  return ans;
}

static void check_all(
    LiChao &tree, const vector<L> &lines, int n,
    const string &name) {
  for (int x = 0; x < n; ++x) {
    ll got = tree.query(x);
    ll want = scan_max(lines, x);
    if (got != want) {
      cerr << "LiChao mismatch case=" << name
           << " x=" << x << " got=" << got
           << " want=" << want << " lines=" << lines.size() << '\n';
      exit(1);
    }
  }
}

static void run_case(int n, const vector<L> &to_insert,
                     const string &name) {
  LiChao tree(n);
  vector<L> lines;
  check_all(tree, lines, n, name + "-empty");
  for (size_t i = 0; i < to_insert.size(); ++i) {
    tree.insert(to_insert[i]);
    lines.push_back(to_insert[i]);
    check_all(tree, lines, n, name + "-after-" + to_string(i));
  }
}

int main() {
  run_case(0, {L(7, 11, 0), L(-3, 4, 1)}, "zero-x");
  for (int n : {INT_MIN, -7, 0}) {
    LiChao empty(n);
    empty.insert(L(LLONG_MAX, LLONG_MAX, 0));
    for (ll x : {LLONG_MIN, -1LL, 0LL, LLONG_MAX})
      assert(empty.query(x) == -INF);
  }
  run_case(1, {L(LLONG_MIN, -INF, -2), L(LLONG_MAX, INF, -2),
               L(0, 0, LLONG_MIN)}, "singleton-extreme-slopes-ids");
  run_case(2, {L(LLONG_MAX, -LLONG_MAX, 0),
               L(LLONG_MIN, LLONG_MAX, LLONG_MAX),
               L(0, -INF, 2), L(0, INF, 3)}, "ll-boundaries");
  run_case(3, {L(LLONG_MAX / 2, -LLONG_MAX / 2, 0),
               L(-LLONG_MAX / 2, LLONG_MAX / 2, 1)},
           "large-midpoint-tie");

  // Equal slopes need only the best intercept, even on a wide domain.
  {
    const int n = 1 << 20;
    LiChao tree(n), empty(n);
    for (int q = 0; q < 200000; ++q) {
      int x = (q * 7919LL) % n;
      assert(empty.query(x) == -INF);
      tree.insert(L(37, q, q));
      tree.insert(L(37, -q, q + 200000));
      assert(tree.query(x) == 37LL * x + q);
    }
    for (int x : {0, n / 2, n - 1})
      assert(tree.query(x) == 37LL * x + 199999);
  }

  run_case(1, {
      L(0, 0, 0), L(0, -7, 1), L(0, 7, 2),
      L(-1000000, 1000000, 3), L(1000000, -1000000, 4)},
      "one-x");

  run_case(2, {
      L(0, 0, 0), L(0, 5, 1), L(0, -5, 2),
      L(1, 0, 3), L(-1, 1, 4), L(1, 0, 5)},
      "two-x-equal-slopes");

  vector<L> exhaustive;
  ll id = 0;
  for (ll m = -8; m <= 8; ++m)
    for (ll k = -8; k <= 8; ++k)
      exhaustive.emplace_back(m, k, id++);
  // Equal slopes and descending intercepts are intentionally not shuffled.
  run_case(17, exhaustive, "small-coefficient-grid");

  mt19937_64 rng(0x1c4a0f13ULL);
  // Every small domain, with varied insertion order and crossing ties.
  mt19937_64 order_rng(0x51A0);
  vector<L> crossings;
  for (ll m = -3; m <= 3; ++m)
    for (ll k = -3; k <= 3; ++k) crossings.emplace_back(m, k, 0);
  for (int n = 1; n <= 19; ++n)
    for (int q = 0; q < 5; ++q) {
      shuffle(crossings.begin(), crossings.end(), order_rng);
      run_case(n, crossings, "permuted-crossings");
    }
  for (int tc = 0; tc < 120; ++tc) {
    int n = 1 + (int)(rng() % 127);
    vector<L> lines;
    int count = 1 + (int)(rng() % 180);
    for (int i = 0; i < count; ++i) {
      ll m;
      if (i % 5 == 0) m = -37;
      else if (i % 5 == 1) m = 37;
      else m = (ll)(rng() % 2000001) - 1000000;
      ll k = (ll)(rng() % 2000001) - 1000000;
      lines.emplace_back(m, k, i);
    }
    LiChao tree(n);
    vector<L> inserted;
    check_all(tree, inserted, n, "random-" + to_string(tc) + "-empty");
    for (int i = 0; i < count; ++i) {
      tree.insert(lines[i]);
      inserted.push_back(lines[i]);
      if (i % 3 == 0 || i + 1 == count)
        check_all(tree, inserted, n,
                  "random-" + to_string(tc) + "-" + to_string(i));
    }
  }
  cout << "lichao_edges: PASS\n";
}
