/*
Mo's algorithm with point updates.
`add_time(t, L, R)` applies update t,
  `sub_time` rolls it back, and the other
callbacks maintain the current [L,
  R] answer.  Updates are numbered from 1.
*/
struct Query {
  int L, R, LBid, RBid, T, id;
  Query(int l, int r, int t,
    int block = 1, int query_id = -1)
      : L(l), R(r), LBid(l / block),
        RBid(r / block), T(t), id(query_id) {}
  bool operator<(const Query &q) const {
    return tie(LBid, RBid,
      T) < tie(q.LBid, q.RBid, q.T);
  }
};

template<class AddTime, class SubTime,
  class Add, class Sub, class Answer>
void solve(vector<Query> query,
  AddTime add_time, SubTime sub_time,
           Add add, Sub sub, Answer answer) {
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
