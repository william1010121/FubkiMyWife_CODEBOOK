struct BCC { // 0-base; O(V+E); handles multi-edges, ignores loops
  int n, dft, ecnt, nbcc;
  vector<int> low, dfn, bln, stk, is_ap, cir;
  vector<vector<pii>> G;
  vector<vector<int>> bcc, nG;
  void make_bcc(int u) { bcc.emplace_back(1, u);
    for (; stk.back() != u; stk.pop_back())
      bln[stk.back()] = nbcc, bcc[nbcc].pb(stk.back());
    stk.pop_back(), bln[u] = nbcc++;
  }
  void dfs(int u, int pe) {
    int child = 0; low[u] = dfn[u] = ++dft, stk.pb(u);
    for (auto [v, e] : G[u])
      if (!dfn[v]) {
        dfs(v, e), ++child, low[u] = min(low[u], low[v]);
        if (dfn[u] <= low[v]) {
          is_ap[u] = 1, bln[u] = nbcc, make_bcc(v), bcc.back().pb(u);
        }
      } else if (e != pe && dfn[v] < dfn[u])
        low[u] = min(low[u], dfn[v]);
    if (pe == -1 && child < 2) is_ap[u] = 0;
    if (pe == -1 && child == 0) make_bcc(u);
  }
  BCC(int _n): n(_n), dft(), ecnt(), nbcc(), // n>=0; counts fit int
    low(n), dfn(n), bln(n), is_ap(n), G(n) {}
  void add_edge(int u, int v) {
    G[u].pb({v, ecnt}), G[v].pb({u, ecnt++}); }
  void solve() { // add all edges first; IDs [0,n); enough DFS stack
    for (int i = 0; i < n; ++i) if (!dfn[i]) dfs(i, -1); }
  void block_cut_tree() { // once after solve; changes nbcc/bln
    int base = nbcc; cir.assign(base, 1);
    for (int i = 0; i < n; ++i) if (is_ap[i]) bln[i] = nbcc++;
    nG.assign(nbcc, {});
    for (int i = 0; i < base; ++i) for (int j : bcc[i])
      if (is_ap[j]) nG[i].pb(bln[j]), nG[bln[j]].pb(i);
  } // <= max(n, 2*n-2) nodes; bln[i] maps vertex i
};
