#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

#define X first
#define Y second
#define SZ(v) static_cast<int>((v).size())

#include "../../codebook/2_Graph/Vizing.cpp"

static bool colorable(int n, const vector<pii> &edges, int colors) {
  if (edges.empty()) return true;
  vector<unsigned long long> used(n + 1);
  vector<int> degree(n + 1);
  for (auto [u, v] : edges) ++degree[u], ++degree[v];
  vector<int> color(edges.size(), 0);

  function<bool(int)> dfs = [&](int done) {
    if (done == SZ(edges)) return true;
    int pick = -1, best_score = -1;
    for (int i = 0; i < SZ(edges); ++i) if (!color[i]) {
      auto [u, v] = edges[i];
      int score = degree[u] + degree[v] +
                  __builtin_popcountll(used[u] | used[v]);
      if (score > best_score) best_score = score, pick = i;
    }
    auto [u, v] = edges[pick];
    unsigned long long forbidden = used[u] | used[v];
    for (int c = 1; c <= colors; ++c) if (!(forbidden >> c & 1ULL)) {
      color[pick] = c;
      used[u] |= 1ULL << c;
      used[v] |= 1ULL << c;
      if (dfs(done + 1)) return true;
      used[u] ^= 1ULL << c;
      used[v] ^= 1ULL << c;
      color[pick] = 0;
    }
    return false;
  };
  return dfs(0);
}

static int exact_chromatic_index(int n, const vector<pii> &edges) {
  if (edges.empty()) return 0;
  vector<int> degree(n + 1);
  for (auto [u, v] : edges) ++degree[u], ++degree[v];
  int delta = *max_element(degree.begin() + 1, degree.end());
  for (int k = 1; k <= delta + 1; ++k)
    if (colorable(n, edges, k)) return k;
  return delta + 2;
}

static void check(int tc, int n, vector<pii> edges, int expected_chi) {
  vector<vector<char>> present(n + 1, vector<char>(n + 1));
  vector<int> degree(n + 1);
  for (auto [u, v] : edges) {
    if (u == v || present[u][v]) {
      cerr << "internal invalid simple graph at tc=" << tc << '\n';
      exit(1);
    }
    present[u][v] = present[v][u] = 1;
    ++degree[u], ++degree[v];
  }
  int delta = *max_element(degree.begin() + 1, degree.end());
  vizing::init(n);
  vizing::solve(edges);
  vector<vector<char>> used(n + 1, vector<char>(delta + 2));
  int max_color = 0;
  for (auto [u, v] : edges) {
    int c = vizing::G[u][v];
    if (c < 1 || c > delta + 1 || used[u][c] || used[v][c]) {
      cerr << "vizing invalid coloring tc=" << tc << " n=" << n
           << " edge=" << u << ',' << v << " color=" << c
           << " delta=" << delta << '\n';
      exit(1);
    }
    used[u][c] = used[v][c] = 1;
    max_color = max(max_color, c);
  }
  for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
    if (!present[u][v] && vizing::G[u][v] != 0) {
      cerr << "vizing colored a nonexistent edge tc=" << tc << '\n';
      exit(1);
    }
  if (edges.empty()) {
    if (max_color != 0) {
      cerr << "vizing empty graph got a color tc=" << tc << '\n';
      exit(1);
    }
  } else if (max_color > delta + 1 || expected_chi > delta + 1 ||
             !colorable(n, edges, delta + 1)) {
    cerr << "vizing chromatic-index mismatch tc=" << tc << " n=" << n
         << " chi=" << expected_chi << " delta=" << delta
         << " max_color=" << max_color << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;
  // Every simple graph through five vertices, plus three edge orders. This
  // exhausts all small adjacency patterns and exercises stale-color resets.
  for (int n = 1; n <= 5; ++n) {
    vector<pii> possible;
    for (int u = 1; u <= n; ++u)
      for (int v = u + 1; v <= n; ++v) possible.push_back({u, v});
    const int total = 1 << SZ(possible);
    for (int mask = 0; mask < total; ++mask) {
      vector<pii> base;
      for (int i = 0; i < SZ(possible); ++i)
        if (mask >> i & 1) base.push_back(possible[i]);
      int chi = exact_chromatic_index(n, base);
      for (int order = 0; order < 3; ++order) {
        vector<pii> edges = base;
        if (order == 1) reverse(edges.begin(), edges.end());
        if (order == 2) rotate(edges.begin(), edges.begin() +
                                  (edges.empty() ? 0 : edges.size() / 2),
                                edges.end());
        check(++tc, n, edges, chi);
      }
    }
  }

  mt19937 rng(0x56495A49);
  for (int round = 0; round < 2500; ++round) {
    int n = 6 + rng() % 3;
    vector<pii> edges;
    for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
      if (rng() % 100 < 34) edges.push_back({u, v});
    check(++tc, n, edges, 0);
  }
  cout << "vizing PASS cases=" << tc
       << " oracle=exact-small-edge-colorability-plus-validity\n";
}
