#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
    ans = max(ans, (ll)w[u][v] +
        oracle(w, mask | (1 << u) | (1 << v), memo));
  return ans;
}

static void validate(WeightGraph &graph, const vector<vector<int>> &w,
                     int tc, const char *label) {
  int n = (int)w.size();
  vector<ll> memo(1 << n, LLONG_MIN);
  ll want = oracle(w, 0, memo);
  // Rebuild from scratch even if the previous search left active blossoms.
  for (int repeat = 0; repeat < 3; ++repeat) {
    auto got = graph.solve();

    ll internal = 0;
    int pairs = 0;
    for (int u = 1; u <= n; ++u) {
      int v = graph.match[u];
      if (!v) continue;
      if (v < 1 || v > n || v == u || graph.match[v] != u ||
          w[u - 1][v - 1] <= 0) {
        cerr << "WeightGraph invalid matching case " << tc << " (" << label
             << ") at " << u << " -> " << v << '\n';
        exit(1);
      }
      if (u < v) internal += w[u - 1][v - 1], ++pairs;
    }
    if (got.first != want || got.first != internal ||
        got.second != pairs) {
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
}

static void check(const vector<vector<int>> &w, int tc,
                  const char *label) {
  int n = (int)w.size();
  WeightGraph graph(n);
  for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
    // Duplicate calls overwrite weights rather than keeping the maximum.
    if ((i + j + tc) % 2 == 0) graph.add_edge(i + 1, j + 1, -7);
    graph.add_edge(i + 1, j + 1, w[i][j]);
  }
  validate(graph, w, tc, label);
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

  // All three-level weights on five vertices: tied greedy seeds, odd
  // cycles and alternative augmentations under every vertex ordering.
  for (int code = 0; code < 59049; ++code) {
    vector<vector<int>> w(5, vector<int>(5));
    int x = code;
    for (int u = 0; u < 5; ++u) for (int v = u + 1; v < 5; ++v) {
      w[u][v] = w[v][u] = x % 3;
      x /= 3;
    }
    check(w, tc++, "five-vertex-ternary-exhaustive");
  }

  mt19937 rng(0x14E17A);
  for (int z = 0; z < 3200; ++z) {
    int n = 1 + rng() % 10;
    vector<vector<int>> w(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      w[i][j] = w[j][i] = (int)(rng() % 13) - 4;
    check(w, tc++, "seeded-random-signed");
  }
  // Extreme weights without contractions, and a total exceeding int.
  for (int value : {INT_MIN, -1, 0, 1, INT_MAX / 2}) {
    check({{0, value}, {value, 0}}, tc++, "extreme-single-edge");
  }
  vector<vector<int>> large(8, vector<int>(8));
  for (int u = 0; u < 8; ++u) for (int v = u + 1; v < 8; ++v)
    large[u][v] = large[v][u] = INT_MAX / 2;
  check(large, tc++, "large-labels-and-ll-total");

  // Larger odd cycles with chords and heavily tied/scaled weights.
  // Random relabeling changes seed selection and blossom traversal order.
  for (int n : {7, 9, 11, 13, 15})
    for (int trial = 0; trial < 40; ++trial) {
    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    shuffle(order.begin(), order.end(), rng);
    vector<vector<int>> w(n, vector<int>(n));
    for (int u = 0; u < n; ++u) {
      int v = (u + 1) % n;
      int a = order[u], b = order[v];
      w[a][b] = w[b][a] = 17000000;
      v = (u + 3) % n; b = order[v];
      w[a][b] = w[b][a] = (int)(rng() % 25) * 1000000 - 8000000;
    }
    check(w, tc++, "relabeled-odd-cycle-chords");
  }

  // Scaling preserves optima but exercises larger dual updates/blossoms.
  // Mutations include deletion, restoration, ties, and negative weights.
  int mutations = 0;
  // Two odd components force seed rerouting through a bridge. Every
  // ordering moves the free vertices and contraction representatives.
  for (int bridge : {1, 99999999, 100000000}) {
    vector<int> order{0, 1, 2, 3, 4, 5};
    do {
      vector<vector<int>> w(6, vector<int>(6, -100000000));
      for (int u = 0; u < 6; ++u) w[u][u] = 0;
      for (int base : {0, 3})
        for (int u = base; u < base + 3; ++u)
          for (int v = u + 1; v < base + 3; ++v)
            w[order[u]][order[v]] = w[order[v]][order[u]] = 100000000;
      w[order[0]][order[3]] = w[order[3]][order[0]] = bridge;
      check(w, tc++, "permuted-odd-components-large-weights");
    } while (next_permutation(order.begin(), order.end()));
  }
  for (int trial = 0; trial < 500; ++trial) {
    int n = 2 + rng() % 9;
    vector<vector<int>> w(n, vector<int>(n));
    WeightGraph graph(n);
    for (int stage = 0; stage < 20; ++stage) {
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
        int value = ((int)(rng() % 25) - 8) * 1000000;
        if (stage % 5 == 0) value = 0;
        w[u][v] = w[v][u] = value;
        graph.add_edge(u + 1, v + 1, value);
      }
      validate(graph, w, mutations++, "mutated-scaled-weights");
    }
  }
  cout << "PASS " << tc << " maximum-weight matching oracle cases, "
       << mutations << " mutation stages\n";
}
