#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

const int N = 12;
const ll INF = (1LL << 60);

#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"

static pll oracle(const vector<vector<ll>> &w) {
  int n = static_cast<int>(w.size());
  bool found = false;
  ll best_num = 0, best_den = 1;
  vector<char> used(n);

  // A minimum-mean closed walk contains a minimum-mean simple cycle, so
  // enumerate every simple directed cycle, including one-vertex loops.
  function<void(int, int, int, ll)> dfs =
      [&](int start, int u, int len, ll sum) {
    for (int v = 0; v < n; ++v) {
      if (w[u][v] == INF) continue;
      if (v == start) {
        ll num = sum + w[u][v], den = len + 1;
        if (!found || (__int128)num * best_den <
                         (__int128)best_num * den)
          found = true, best_num = num, best_den = den;
      } else if (!used[v]) {
        used[v] = 1;
        dfs(start, v, len + 1, sum + w[u][v]);
        used[v] = 0;
      }
    }
  };
  for (int start = 0; start < n; ++start) {
    fill(used.begin(), used.end(), 0);
    used[start] = 1;
    dfs(start, start, 0, 0);
  }
  if (!found) return {-1, -1};
  ll g = gcd(llabs(best_num), best_den);
  return {best_num / g, best_den / g};
}

static void check(int tc, const vector<vector<ll>> &w) {
  int n = static_cast<int>(w.size());
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      road[i][j] = w[i][j];
  MinimumMeanCycle solver;
  solver.init(n);
  pll got = solver.solve(), want = oracle(w);
  if (got != want) {
    cerr << "minimum_mean_cycle mismatch tc=" << tc << " n=" << n
         << " got=" << got.first << '/' << got.second
         << " want=" << want.first << '/' << want.second << '\n';
    for (const auto &row : w) {
      for (ll x : row) cerr << (x == INF ? string("INF") : to_string(x)) << ' ';
      cerr << '\n';
    }
    exit(1);
  }
}

int main() {
  int tc = 0;
  // Exhaust all directed topologies through three vertices, with loops.
  for (int n = 2; n <= 3; ++n) {
    int possible = n * n;
    for (int mask = 0; mask < (1 << possible); ++mask) {
      vector<vector<ll>> w(n, vector<ll>(n, INF));
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        int bit = i * n + j;
        if (mask >> bit & 1)
          w[i][j] = (ll)((bit * 19 + mask * 7) % 31) - 15;
      }
      check(++tc, w);
    }
  }

  vector<vector<vector<ll>>> special = {
    {{0, 4}, {-9, INF}},
    {{INF, 1, INF}, {INF, INF, 2}, {-4, INF, INF}},
    {{0, INF, 6}, {INF, 0, INF}, {INF, INF, 0}},
    {{INF, -8, INF, INF}, {INF, INF, 3, INF},
     {INF, INF, INF, 1}, {2, INF, INF, INF}},
  };
  for (auto &w : special) check(++tc, w);

  mt19937 rng(0x4D4D4358);
  for (int round = 0; round < 4500; ++round) {
    int n = 2 + rng() % 6;
    vector<vector<ll>> w(n, vector<ll>(n, INF));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      if (rng() % 100 < 48)
        w[i][j] = (ll)(rng() % 101) - 50;
    check(++tc, w);
  }
  cout << "minimum_mean_cycle PASS cases=" << tc
       << " oracle=enumerated-simple-directed-cycles\n";
}
