// block>0; valid 0<=L<=R (zero-based); T>=0 is #updates applied.
// add_time/sub_time apply/undo update t (1-based) in current [L,R].
// Start empty at T=0; add/sub positions; answer(q) reads the state.
// Block ~ N^(2/3), O(N^(5/3)) for O(N) queries/updates.
struct Query {
  int L, R, LBid, RBid, T, id;
  Query(int l, int r, int t, int block = 1, int query_id = -1)
    : L(l), R(r), LBid(l / block), RBid(r / block), T(t), id(query_id) {}
  bool operator<(const Query &q) const {
    if (LBid != q.LBid) return LBid < q.LBid;
    if (RBid != q.RBid) return LBid & 1 ? RBid > q.RBid : RBid < q.RBid;
    return (LBid ^ RBid) & 1 ? T > q.T : T < q.T;
  }
};
void solve(vector<Query> query, auto add_time, auto sub_time,
           auto add, auto sub, auto answer) {
  sort(query.begin(), query.end());
  int L = 0, R = -1, T = 0;
  for (const Query &q : query) {
    while (T < q.T) add_time(++T, L, R);
    while (T > q.T) sub_time(T--, L, R);
    while (R < q.R) add(++R);
    while (L > q.L) add(--L);
    while (R > q.R) sub(R--);
    while (L < q.L) sub(L++);
    answer(q);
  }
}
