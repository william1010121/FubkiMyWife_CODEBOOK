#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define INF 0x3f3f3f3f
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "../../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"

static ll oracle(const vector<vector<int>> &w, int mask,
                 vector<ll> &memo) {
  int n = (int)w.size();
  if (mask == (1 << n) - 1) return 0;
  ll &ans = memo[mask];
  if (ans != LLONG_MIN) return ans;
  int u = 0;
  while (mask >> u & 1) ++u;
  ans = oracle(w, mask | (1 << u), memo);  // leave u unmatched
  for (int v = u + 1; v < n; ++v) if (!(mask >> v & 1) && w[u][v] > 0)
    ans = max(ans, (ll)w[u][v] + oracle(w, mask | (1 << u) | (1 << v), memo));
  return ans;
}

static void check(const vector<vector<int>> &w, int tc, const char *label) {
  int n = (int)w.size();
  WeightGraph graph(n);
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
    // Exercise overwrite semantics on half of the edges without changing
    // their final value.
    if ((i + j + tc) % 2 == 0) graph.add_edge(i + 1, j + 1, -7);
    graph.add_edge(i + 1, j + 1, w[i][j]);
  }
  vector<ll> memo(1 << n, LLONG_MIN);
  ll want = oracle(w, 0, memo);
  auto got = graph.solve();

  ll internal = 0;
  int pairs = 0;
  for (int u = 1; u <= n; ++u) {
    int v = graph.match[u];
    if (!v) continue;
    if (v < 1 || v > n || v == u || graph.match[v] != u || w[u - 1][v - 1] <= 0) {
      cerr << "WeightGraph invalid matching case " << tc << " (" << label
           << ") at " << u << " -> " << v << '\n';
      exit(1);
    }
    if (u < v) internal += w[u - 1][v - 1], ++pairs;
  }
  if (got.first != want || got.first != internal || got.second != pairs) {
    cerr << "WeightGraph mismatch case " << tc << " (" << label << ") n="
         << n << " got=(" << got.first << ',' << got.second << ") want="
         << want << " actual=(" << internal << ',' << pairs << ")\n";
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j)
        cerr << w[i][j] << (j + 1 == n ? '\n' : ' ');
    }
    exit(1);
  }
}

int main() {
  int tc = 0;
  const int values[] = {-2, -1, 0, 1, 2};

  // Exhaustive signed matrices through four vertices.  Nonpositive edges
  // must be left unmatched because this implementation is a max-weight
  // matching routine, not a forced perfect matcher.
  for (int n = 0; n <= 4; ++n) {
    vector<pair<int, int>> edges;
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      edges.push_back({i, j});
    long long total = 1;
    for (int i = 0; i < (int)edges.size(); ++i) total *= 5;
    for (long long code = 0; code < total; ++code) {
      long long x = code;
      vector<vector<int>> w(n, vector<int>(n));
      for (auto [u, v] : edges) {
        w[u][v] = w[v][u] = values[x % 5];
        x /= 5;
      }
      check(w, tc++, "signed-exhaustive");
    }
  }

  vector<vector<vector<int>>> fixed;
  fixed.push_back({});
  fixed.push_back(vector<vector<int>>(1, vector<int>(1)));
  for (int n : {5, 6, 7, 8, 9, 10}) {
    vector<vector<int>> w(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      w[i][j] = w[j][i] = (i + j) % 3 == 0 ? 2 : (i + j) % 3 == 1 ? 1 : -3;
    fixed.push_back(w);
  }
  for (auto &w : fixed) check(w, tc++, "fixed-ties-and-unmatched");

  mt19937 rng(0x14E17A);
  for (int z = 0; z < 3200; ++z) {
    int n = 1 + rng() % 10;
    vector<vector<int>> w(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      w[i][j] = w[j][i] = (int)(rng() % 13) - 4;
    check(w, tc++, "seeded-random-signed");
  }
  cout << "PASS " << tc << " maximum-weight matching oracle cases\n";
}
