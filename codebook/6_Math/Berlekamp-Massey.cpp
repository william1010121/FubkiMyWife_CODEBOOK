template<class T> vector<T> BerlekampMassey(const vector<T> &output) {
  vector<T> me, he; T last = 0;
  for (int f = 0, i = 1; i <= SZ(output); ++i) {
    T d = -output[i - 1];
    for (int j = 0; j < SZ(me); ++j) d += output[i - j - 2] * me[j];
    if (d == 0) continue;
    if (me.empty()) { me.resize(f = i), last = d; continue; }
    T k = -d / last; vector<T> o(i - f - 1); o.pb(-k);
    for (T x : he) o.pb(x * k);
    o.resize(max(SZ(o), SZ(me)));
    for (int j = 0; j < SZ(me); ++j) o[j] += me[j];
    if (i - f + SZ(he) >= SZ(me)) he = me, f = i, last = d;
    me = move(o);
  }
  return me; // Field T; a[i]=sum(c[j]*a[i-j-1]); O(n^2).
}
