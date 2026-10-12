// Hierholzer, O(V+E); 0-base; dir: directed edges.
// Walk from s using every edge once; {} if none exists.
// Circuit iff front()==back(). Self-loops/multi-edges OK.
struct Euler {
  int n, m = 0; bool dir; vector<vector<pii>> G;
  Euler(int n, bool dir) : n(n), dir(dir), G(n) {}
  void add_edge(int u, int v) {
    G[u].pb({v, m}); if (!dir) G[v].pb({u, m}); ++m;
  }
  vector<int> solve(int s) { // vertex sequence, m+1 long
    vector<int> D(n), it(n), used(m), path, st = {s};
    ++D[s]; // path may start at s
    while (!st.empty()) {
      int u = st.back(); auto &e = G[u];
      while (it[u] < SZ(e) && used[e[it[u]].Y]) ++it[u];
      if (it[u] == SZ(e)) {
        path.pb(u), st.pop_back(); continue; }
      auto [v, id] = e[it[u]++];
      used[id] = 1, --D[u], ++D[v], st.pb(v);
    }
    for (int x : D) if (x < 0) return {};
    if (SZ(path) != m + 1) return {};
    reverse(ALL(path)); return path;
  }
};
