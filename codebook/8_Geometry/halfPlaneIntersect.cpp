bool jizz(L l1, L l2, L l3) { P p = Intersect(l2, l3);
  return ((l1.pb - l1.pa) ^ (p - l1.pa)) < -eps; }
// Exact ordering/deduplication; epsilon only for feasibility.
bool cmp(const L &a, const L &b) { return a.o == b.o ?
  a.c / hypot(a.a, a.b) < b.c / hypot(b.a, b.b) : a.o < b.o; }
// Nonzero lines, left side feasible; bounded positive area. O(n log n).
vector<P> HPI(vector<L> &ls) {
  sort(ls.begin(), ls.end(), cmp); deque<L> dq;
  for (L l : ls) {
    if (!dq.empty() && l.o == dq.back().o) continue;
    while (dq.size() > 1 && jizz(l, dq.back(), dq[dq.size() - 2]))
      dq.pop_back();
    while (dq.size() > 1 && jizz(l, dq[0], dq[1])) dq.pop_front();
    dq.push_back(l);
  }
  while (dq.size() > 1 && jizz(dq.front(), dq.back(),
      dq[dq.size() - 2])) dq.pop_back();
  while (dq.size() > 1 && jizz(dq.back(), dq[0], dq[1])) dq.pop_front();
  if (dq.size() < 3) return {};
  vector<P> rt; for (int i = 0; i < (int)dq.size(); ++i)
    rt.push_back(Intersect(dq[i], dq[(i + 1) % dq.size()]));
  return rt;
}
