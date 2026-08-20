#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
#define X first
#define Y second
#define SZ(v) ((int)(v).size())
#include "../../../codebook/2_Graph/Vizing.cpp"

static int chromatic_index(int n, const vector<pii> &es, int k) {
  vector<vector<char>> used(n + 1, vector<char>(k + 1));
  vector<int> order(es.size());
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) {
    return (es[a].X == es[a].Y ? 0 : 1) + 0 > (es[b].X == es[b].Y ? 0 : 1);
  });
  function<bool(int)> dfs = [&](int at) {
    if (at == (int)order.size()) return true;
    auto [u, v] = es[order[at]];
    for (int c = 1; c <= k; ++c) if (!used[u][c] && !used[v][c]) {
      used[u][c] = used[v][c] = 1;
      if (dfs(at + 1)) return true;
      used[u][c] = used[v][c] = 0;
    }
    return false;
  };
  return dfs(0);
}

static int exact_chi(int n, const vector<pii> &es, int delta) {
  if (es.empty()) return 0;
  for (int k = 1; k <= delta + 1; ++k) if (chromatic_index(n, es, k)) return k;
  return delta + 2;
}

int main() {
  mt19937 rng(0x56495A49);
  int cases = 0;
  for (int n = 1; n <= 8; ++n) for (int tc = 0; tc < 450; ++tc) {
    vector<pii> es;
    vector<vector<char>> exists(n, vector<char>(n));
    for (int u = 1; u <= n; ++u) for (int v = u + 1; v <= n; ++v)
      if ((int)(rng() % 100) < 38) es.push_back({u, v}), exists[u - 1][v - 1] = 1;
    vector<int> deg(n);
    for (auto [u, v] : es) ++deg[u - 1], ++deg[v - 1];
    int delta = *max_element(deg.begin(), deg.end());
    int chi = exact_chi(n, es, delta);
    vizing::init(n);
    vizing::solve(es);
    vector<vector<char>> used(n + 1, vector<char>(delta + 2));
    int used_max = 0;
    bool valid = true;
    for (auto [u, v] : es) {
      int c = vizing::G[u][v];
      if (c < 1 || c > delta + 1 || used[u - 1][c] || used[v - 1][c]) valid = false;
      used[u - 1][c] = used[v - 1][c] = 1;
      used_max = max(used_max, c);
    }
    ++cases;
    if (!valid || (es.empty() ? used_max != 0 : used_max < chi) ||
        (es.empty() ? false : !chromatic_index(n, es, delta + 1))) {
      cerr << "vizing mismatch case=" << cases << " n=" << n
           << " edges=" << es.size() << " valid=" << valid
           << " used_max=" << used_max << " chi=" << chi << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=exact-edge-coloring-plus-validity\n";
}
