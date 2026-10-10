bool jizz(L l1, L l2, L l3) { P p = Intersect(l2, l3);
  return ((l1.pb - l1.pa) ^ (p - l1.pa)) < -eps; }
bool cmp(const L &a, const L &b) { return same(a.o, b.o) ?
  ((b.pb - b.pa) ^ (a.pb - b.pa)) > eps : a.o < b.o; }
// Left side feasible. Bounded, positive-area result; O(n log n).
vector<P> HPI(vector<L> &ls) {
  sort(ls.begin(), ls.end(), cmp); vector<L> pls;
  for (L l : ls)
    if (pls.empty() || !same(l.o, pls.back().o)) pls.push_back(l);
  if (pls.size() < 3) return {};
  deque<int> dq = {0, 1};
  auto outside = [&](int a, int b, int c) {
    return jizz(pls[a], pls[b], pls[c]); };
  for (int i = 2; i < (int)pls.size(); ++i) {
    while (dq.size() > 1 && outside(i, dq.back(), dq[dq.size() - 2]))
      dq.pop_back();
    while (dq.size() > 1 && outside(i, dq[0], dq[1])) dq.pop_front();
    dq.push_back(i);
  }
  while (dq.size() > 1 && outside(dq.front(), dq.back(),
      dq[dq.size() - 2])) dq.pop_back();
  while (dq.size() > 1 && outside(dq.back(), dq[0], dq[1])) dq.pop_front();
  if (dq.size() < 3) return {};
  vector<P> rt; for (int i = 0; i < (int)dq.size(); ++i)
    rt.push_back(Intersect(pls[dq[i]], pls[dq[(i + 1) % dq.size()]]));
  return rt;
}
