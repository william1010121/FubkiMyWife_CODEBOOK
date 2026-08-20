#include <bits/stdc++.h>
using namespace std;
#define ALL(x) (x).begin(), (x).end()
#define pb push_back
#include "../../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"

static int brute(const vector<vector<char>> &adj, int used) {
  int n = (int)adj.size(), u = 0;
  while (u < n && (used >> u & 1)) ++u;
  if (u == n) return 0;
  int ans = brute(adj, used | (1 << u)); // leave u unmatched
  for (int v = u + 1; v < n; ++v)
    if (adj[u][v] && !(used >> v & 1))
      ans = max(ans, 1 + brute(adj, used | (1 << u) | (1 << v)));
  return ans;
}

static void check(const vector<vector<char>> &adj, int tc) {
  int n = (int)adj.size();
  Matching matching(n);
  for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
    if (adj[u][v]) matching.add_edge(u, v);
  int got = matching.solve(), want = brute(adj, 0);
  if (got != want) {
    cerr << "Blossom mismatch on case " << tc << ": got=" << got
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  int tc = 0;
  // Exhaust every simple graph through six vertices; this includes all odd cycles.
  for (int n = 1; n <= 6; ++n) {
    int edges = n * (n - 1) / 2;
    for (int mask = 0; mask < (1 << edges); ++mask) {
      vector<vector<char>> adj(n, vector<char>(n));
      int bit = 0;
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v, ++bit)
        adj[u][v] = adj[v][u] = mask >> bit & 1;
      check(adj, tc++);
    }
  }
  mt19937 rng(0xB10550);
  for (int z = 0; z < 2500; ++z) {
    int n = 7 + rng() % 7;
    vector<vector<char>> adj(n, vector<char>(n));
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      adj[u][v] = adj[v][u] = rng() % 2;
    check(adj, tc++);
  }
  cout << "PASS " << tc << " exhaustive/random simple graphs\n";
}
