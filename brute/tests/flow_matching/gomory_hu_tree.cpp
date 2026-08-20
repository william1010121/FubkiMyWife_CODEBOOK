#include <bits/stdc++.h>
using namespace std;
#define MAXN 32
#define INF 0x3f3f3f3f

struct MaxFlow {
  struct Edge { int to, cap, flow, rev; };
  vector<Edge> G[MAXN];
  int dis[MAXN], cur[MAXN], n;
  void init(int x) { n = x; for (int i = 0; i < n; ++i) G[i].clear(); }
  void add_edge(int u, int v, int cap) {
    G[u].push_back({v, cap, 0, (int)G[v].size()});
    G[v].push_back({u, 0, 0, (int)G[u].size() - 1});
  }
  void reset() { for (int i = 0; i < n; ++i) for (auto &e : G[i]) e.flow = 0; }
  int dfs(int u, int t, int f) {
    if (u == t) return f;
    for (int &i = cur[u]; i < (int)G[u].size(); ++i) {
      Edge &e = G[u][i];
      if (dis[e.to] == dis[u] + 1 && e.cap != e.flow) {
        int d = dfs(e.to, t, min(f, e.cap - e.flow));
        if (d) { e.flow += d; G[e.to][e.rev].flow -= d; return d; }
      }
    }
    dis[u] = -1;
    return 0;
  }
  int maxflow(int s, int t) {
    int ans = 0;
    while (true) {
      fill(dis, dis + n, -1);
      queue<int> q; q.push(s); dis[s] = 0;
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &e : G[u]) if (e.cap != e.flow && dis[e.to] < 0)
          dis[e.to] = dis[u] + 1, q.push(e.to);
      }
      if (dis[t] < 0) return ans;
      fill(cur, cur + n, 0);
      while (int d = dfs(s, t, INF)) ans += d;
    }
  }
};

#include "codebook/4_Flow_Matching/Gomory_Hu_tree.cpp"

static int direct_cut(const vector<vector<int>> &cap, int s, int t) {
  int n = cap.size(), ans = INF;
  for (int mask = 0; mask < (1 << n); ++mask) if ((mask >> s & 1) && !(mask >> t & 1)) {
    int cut = 0;
    for (int u = 0; u < n; ++u) if (mask >> u & 1)
      for (int v = 0; v < n; ++v) if (!(mask >> v & 1)) cut += cap[u][v];
    ans = min(ans, cut);
  }
  return ans;
}

static int tree_cut(int n, int s, int t) {
  vector<vector<pair<int, int>>> tree(n);
  for (int i = 1; i < n; ++i)
    tree[i].push_back({g[i], gh_w[i]}), tree[g[i]].push_back({i, gh_w[i]});
  vector<int> seen(n);
  queue<pair<int, int>> q;
  q.push({s, INF}), seen[s] = 1;
  while (!q.empty()) {
    auto [u, value] = q.front(); q.pop();
    if (u == t) return value;
    for (auto [v, w] : tree[u]) if (!seen[v])
      seen[v] = 1, q.push({v, min(value, w)});
  }
  return -1;
}

int main() {
  mt19937 rng(0x6f6d6f72);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 2 + rng() % 7;
    vector<vector<int>> cap(n, vector<int>(n));
    Dinic.init(n);
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v) {
      int c = rng() % 6;
      cap[u][v] = cap[v][u] = c;
      if (c) Dinic.add_edge(u, v, c), Dinic.add_edge(v, u, c);
    }
    GomoryHu(n);
    for (int s = 0; s < n; ++s) for (int t = s + 1; t < n; ++t)
      if (tree_cut(n, s, t) != direct_cut(cap, s, t)) {
        cerr << "Gomory-Hu mismatch tc=" << tc << " pair=" << s << ',' << t << '\n';
        return 1;
      }
  }
  cout << "gomory_hu_tree: PASS (500 exhaustive-cut random cases)\n";
}
