struct Maxflow {
  static const int MAXV = 20010;
  static const int INF = 1000000;
  struct Edge { int v, c, r; };
  int s, t;
  vector<Edge> G[MAXV * 2];
  int iter[MAXV * 2], d[MAXV * 2], gap[MAXV * 2], tot;
  void init(int x) {
    tot = x + 2;
    s = x + 1, t = x + 2;
    for (int i = 0; i <= tot; i++)
      G[i].clear(), iter[i] = d[i] = gap[i] = 0;
  }
  void addEdge(int u, int v, int c) {
    G[u].push_back({v, c, SZ(G[v])}
      ), G[v].push_back({u, 0, SZ(G[u]) - 1});
  }
  void bfs() {
    fill(d, d + tot + 1, tot);
    queue<int> q;
    d[t] = 0, q.push(t);
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (auto &e : G[u])
        if (G[e.v][e.r].c > 0 && d[e.v] == tot)
          d[e.v] = d[u] + 1, q.push(e.v);
    }
    fill(gap, gap + tot + 1, 0);
    for (int i = 0; i <= tot; ++i) ++gap[d[i]];
  }
  int dfs(int p, int flow) {
    if (p == t) return flow;
    for (int &i = iter[p]; i < SZ(G[p]); i++) {
      Edge &e = G[p][i];
      if (e.c && d[p] == d[e.v] + 1) {
        int f = dfs(e.v, min(flow, e.c));
        if (f) {
          e.c -= f;
          G[e.v][e.r].c += f;
          return f;
        }
      }
    }
    if (!--gap[d[p]]) d[s] = tot;
    else {
      d[p]++, iter[p] = 0, ++gap[d[p]];
    }
    return 0;
  }
  int solve() {
    int res = 0;
    fill(iter, iter + tot + 1, 0);
    bfs();
    for (; d[s] < tot; res += dfs(s, INF));
    return res;
  }
} flow;
