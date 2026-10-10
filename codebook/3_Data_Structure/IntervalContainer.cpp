// Nonempty disjoint, nonadjacent [L,R); updates need L <= R.
// O(log s + erased intervals) per update, amortized O(log s).
set<pii>::iterator addInterval(set<pii>& is, int L, int R) {
  if (L == R) return is.end();
  auto it = is.upper_bound({L, INT_MAX}), before = it;
  if (it != is.begin() && prev(it)->Y >= R) return prev(it);
  while (it != is.end() && it->X <= R)
    R = max(R, it->Y), before = it = is.erase(it);
  if (it != is.begin() && (--it)->Y >= L)
    L = min(L, it->X), R = max(R, it->Y), is.erase(it);
  return is.insert(before, {L, R}); }
void removeInterval(set<pii>& is, int L, int R) {
  if (L == R) return;
  auto it = is.lower_bound({L, INT_MIN});
  if (it != is.begin() && prev(it)->Y > L) --it;
  while (it != is.end() && it->X < R) {
    auto [l, r] = *it; it = is.erase(it);
    if (l < L) is.emplace_hint(it, l, L);
    if (r > R) { is.emplace_hint(it, R, r); break; }
  }
}
