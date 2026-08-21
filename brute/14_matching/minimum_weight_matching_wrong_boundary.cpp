#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 10
#include "../../codebook/4_Flow_Matching/Minimum_Weight_Matching_wrong.cpp"

static ll oracle(const vector<vector<ll>> &w, int mask,
                 vector<ll> &memo) {
  int n = (int)w.size();
  if (mask == (1 << n) - 1) return 0;
  ll &ans = memo[mask];
  if (ans != LLONG_MAX) return ans;
  int u = 0;
  while (mask >> u & 1) ++u;
  ans = LLONG_MAX;
  for (int v = u + 1; v < n; ++v) if (!(mask >> v & 1))
    ans = min(ans, w[u][v] + oracle(w, mask | (1 << u) | (1 << v), memo));
  return ans;
}

static void check(const vector<vector<ll>> &w, int tc, const char *label) {
  int n = (int)w.size();
  Graph graph;
  graph.init(n);
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
    graph.add_edge(i, j, w[i][j]);
  vector<ll> memo(1 << n, LLONG_MAX);
  ll want = oracle(w, 0, memo);
  ll got = graph.solve();
  if (got != want) {
    cerr << "Minimum perfect matching mismatch case " << tc << " (" << label
         << ") n=" << n << " got=" << got << " want=" << want << '\n';
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j)
        cerr << w[i][j] << (j + 1 == n ? '\n' : ' ');
    }
    exit(1);
  }
}

int main() {
  int tc = 0;
  const ll values[] = {-3, -1, 0, 2, 5};

  check({}, tc++, "n-zero");
  for (int n = 2; n <= 4; n += 2) {
    vector<pair<int, int>> edges;
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      edges.push_back({i, j});
    long long total = 1;
    for (int i = 0; i < (int)edges.size(); ++i) total *= 5;
    for (long long code = 0; code < total; ++code) {
      long long x = code;
      vector<vector<ll>> w(n, vector<ll>(n));
      for (auto [u, v] : edges) {
        w[u][v] = w[v][u] = values[x % 5];
        x /= 5;
      }
      check(w, tc++, "signed-perfect-exhaustive");
    }
  }

  mt19937_64 rng(0x14A17C4);
  for (int z = 0; z < 500; ++z) {
    int n = 2 + 2 * (rng() % 4);
    vector<vector<ll>> w(n, vector<ll>(n));
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      w[i][j] = w[j][i] = (ll)(rng() % 101) - 50;
    check(w, tc++, "seeded-random-perfect");
  }
  cout << "PASS " << tc << " minimum-perfect-matching oracle cases\n";
}
