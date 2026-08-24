#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL << 60);

#include "../../codebook/3_Data_Structure/LiChaoST.cpp"

static ll scan_max(const vector<L> &lines, ll x) {
  ll ans = -INF;
  for (L line : lines) ans = max(ans, line.at(x));
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
