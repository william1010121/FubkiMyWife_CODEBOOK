struct Q { ll p, q;
  Q go(Q b, ll d) { return {p + b.p*d, q + b.q*d}; }
};
// Increasing bool pred; true at 1/0; search fits ll.
// Min feasible p/q, 0<=p,q<=N, N>=1; q=0 is infinity.
template<class Pred>
Q frac_bs(ll N, Pred pred) {
  Q lo{0, 1}, hi{1, 0};
  if (pred(lo)) return lo; assert(pred(hi));
  bool dir = 1, L = 1, H = 1;
  for (; L || H; dir = !dir) {
    ll len = 0, step = 1;
    for (int t = 0; t < 2 && (t ? step/=2 : step*=2);)
      if (Q mid = hi.go(lo, len + step);
          mid.p > N || mid.q > N || dir ^ pred(mid)) t++;
      else len += step;
    swap(lo, hi = hi.go(lo, len));
    (dir ? L : H) = !!len;
  } return dir ? hi : lo;
}
