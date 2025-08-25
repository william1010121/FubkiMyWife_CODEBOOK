struct ECC {
    int n, dft, ecnt, necc;
    vector<int> low, dfn, bln, is_bridge, stk;
    vector<vector<pii>> G;
    void dfs( int u, int f ) {
        dfn[u] = low[u] = ++dft, stk,pb(u);
    }
}
