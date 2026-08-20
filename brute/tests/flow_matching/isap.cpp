#include <bits/stdc++.h>
using namespace std;
#define SZ(x) ((int)(x).size())

#include "codebook/4_Flow_Matching/isap.cpp"

static int mincut(const vector<vector<int>> &cap, int s, int t) {
  int n = cap.size(), ans = INT_MAX;
  for (int mask = 0; mask < (1 << n); ++mask) if ((mask >> s & 1) && !(mask >> t & 1)) {
    int cut = 0;
    for (int u = 0; u < n; ++u) if (mask >> u & 1)
      for (int v = 0; v < n; ++v) if (!(mask >> v & 1)) cut += cap[u][v];
    ans = min(ans, cut);
  }
  return ans;
}

int main() {
  mt19937 rng(0x15a9);
  for (int tc = 0; tc < 3000; ++tc) {
    int n = 2 + rng() % 7, s = n + 1, t = n + 2;
    vector<vector<int>> cap(n + 3, vector<int>(n + 3));
    flow.init(n);
    for (int u = 0; u < n + 3; ++u) for (int v = 0; v < n + 3; ++v)
      if (u != v && u != n && v != n && u != s && v != t) {
      cap[u][v] = rng() % 5;
      if (cap[u][v]) flow.addEdge(u, v, cap[u][v]);
    }
    for (int v = 0; v < n; ++v) {
      cap[s][v] = rng() % 5;
      cap[v][t] = rng() % 5;
      if (cap[s][v]) flow.addEdge(s, v, cap[s][v]);
      if (cap[v][t]) flow.addEdge(v, t, cap[v][t]);
    }
    int got = flow.solve(), want = mincut(cap, s, t);
    if (got != want) {
      cerr << "ISAP mismatch tc=" << tc << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "isap: PASS (3000 exhaustive-cut random cases)\n";
}
