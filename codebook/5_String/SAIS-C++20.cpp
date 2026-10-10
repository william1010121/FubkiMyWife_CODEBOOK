// sais: positive integers + unique final 0; O(n+max(s)), C++20
// sa: suffix order; ra: inverse; hi[i]: LCP(sa[i],sa[i-1])
auto sais(const auto &s) {
  int n = SZ(s); if (n == 1) return vector{0};
  int z = ranges::max(s) + 1;
  vector<int> c(z), sa(n), lms, q(n); vector<bool> t(n, true);
  for (int x : s) ++c[x]; partial_sum(ALL(c), c.begin());
  for (int i = n - 2; i >= 0; --i)
    t[i] = s[i] == s[i+1] ? t[i+1] : s[i] < s[i+1];
  auto is_lms = views::filter([&](int i) {
    return i && t[i] && !t[i-1]; });
  auto induce = [&] {
    for (auto x = c; int y : sa)
      if (y-- && !t[y]) sa[x[s[y]-1]++] = y;
    for (auto x = c; int y : sa | views::reverse)
      if (y-- && t[y]) sa[--x[s[y]]] = y;
  }; lms.reserve(n/2);
  for (auto x = c; int i : views::iota(0, n) | is_lms)
    q[i] = SZ(lms), lms.pb(sa[--x[s[i]]] = i);
  induce(); int nz = 0; vector<int> ns(SZ(lms)), nsa;
  for (int j = -1; int i : sa | is_lms) {
    if (j >= 0) { int len = min({n-i, n-j, lms[q[i]+1]-i});
      nz += lexicographical_compare(begin(s)+j, begin(s)+j+len,
                                    begin(s)+i, begin(s)+i+len);
    } ns[q[i]] = nz, j = i; }
  if (nz + 1 == SZ(lms)) { // invert unique names without recursion
    nsa.resize(SZ(lms)); for (int i = 0; i < SZ(lms); ++i) nsa[ns[i]] = i;
  } else nsa = sais(ns); fill(ALL(sa), 0);
  for (auto x = c; int y : nsa | views::reverse)
    y = lms[y], sa[--x[s[y]]] = y;
  return induce(), sa; }
struct Suffix { // _s[0,n) positive; empty input is allowed
  int n; vector<int> sa, hi, ra;
  Suffix(const auto &_s, int _n) : n(_n), hi(n), ra(n) {
    vector<int> s(n+1); copy_n(_s, n, s.begin());
    sa = sais(s), sa.erase(sa.begin());
    for (int i = 0; i < n; ++i) ra[sa[i]] = i;
    for (int i = 0, h = 0; i < n; ++i) {
      if (!ra[i]) { h = 0; continue; }
      for (int j = sa[ra[i]-1]; max(i,j)+h < n &&
           s[i+h] == s[j+h];) ++h;
      hi[ra[i]] = h ? h-- : 0; }
  }
};
