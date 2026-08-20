#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/4_Flow_Matching/SW-mincut.cpp"

static int brute(const vector<vector<int>> &w) {
  int n = (int)w.size(), ans = INT_MAX;
  for (int mask = 1; mask + 1 < (1 << n); ++mask) {
    int cut = 0;
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (((mask >> u) & 1) != ((mask >> v) & 1)) cut += w[u][v];
    ans = min(ans, cut);
  }
  return ans;
}

static void check(const vector<vector<int>> &w, int tc) {
  int n = (int)w.size();
  sw.init(n);
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    for (int z = 0; z < w[u][v]; ++z) sw.addEdge(u, v, 1);
  int got = sw.solve(n), want = brute(w);
  if (got != want) {
    cerr << "Stoer-Wagner mismatch on case " << tc << ": got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;
  for (int n = 2; n <= 4; ++n) {
    int edges = n * (n - 1) / 2;
    for (int mask = 0; mask < (1 << edges); ++mask) {
      vector<vector<int>> w(n, vector<int>(n));
      int bit = 0;
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v, ++bit)
        w[u][v] = w[v][u] = mask >> bit & 1;
      check(w, tc++);
    }
  }
  mt19937 rng(0x57A6);
  for (int z = 0; z < 3000; ++z) {
    int n = 2 + rng() % 8;
    vector<vector<int>> w(n, vector<int>(n));
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      w[u][v] = w[v][u] = rng() % 9;
    check(w, tc++);
  }
  cout << "PASS " << tc << " exhaustive/random weighted undirected graphs\n";
}
