// Common block>0; valid 0-based 2n tour/LCA; 2n fits int.
// Entry/exit tour; flip toggles; start empty.
// block~sqrt(n), q=O(n): O(n sqrt(n)).
struct Query {
  int L, R, LBid, lca, id;
  Query(int u, int v, int block, const vector<int> &in,
        const vector<int> &out, auto get_lca,
        int query_id = -1)
    : id(query_id) {
    if (in[u] > in[v]) swap(u, v);
    int c = get_lca(u, v);
    if (c == u) L = in[u], R = in[v], lca = -1;
    else L = out[u], R = in[v], lca = c;
    LBid = L / block;
  }
  bool operator<(const Query &q) const {
    if (LBid != q.LBid) return LBid < q.LBid;
    return LBid & 1 ? R > q.R : R < q.R;
  }
};
void solve(vector<Query> query, const vector<int> &ord,
           auto flip, auto answer) {
  sort(query.begin(), query.end());
  int L = 0, R = -1;
  for (const Query &q : query) {
    while (R < q.R) flip(ord[++R]);
    while (L > q.L) flip(ord[--L]);
    while (R > q.R) flip(ord[R--]);
    while (L < q.L) flip(ord[L++]);
    if (q.lca != -1) flip(q.lca);
    answer(q);
    if (q.lca != -1) flip(q.lca);
  }
}
