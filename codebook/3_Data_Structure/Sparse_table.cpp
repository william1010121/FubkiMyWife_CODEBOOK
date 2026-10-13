template<class T> struct SparseTable { // [l,r], 0-base
  vector<vector<T>> st;
  SparseTable(const vector<T> &a) : st(1, a) {
    for (int k = 1; (1 << k) <= SZ(a); ++k) {
      st.emplace_back(SZ(a) - (1 << k) + 1);
      for (int i = 0; i < SZ(st[k]); ++i) st[k][i] =
        max(st[k - 1][i], st[k - 1][i + (1 << k >> 1)]);
    } }
  T query(int l, int r) { int k = __lg(r - l + 1);
    return max(st[k][l], st[k][r - (1 << k) + 1]); } };
