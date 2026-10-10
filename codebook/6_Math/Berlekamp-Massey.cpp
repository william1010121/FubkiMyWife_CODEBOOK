// Field: 0 init, arithmetic, nonzero /, ==0; size<INT_MAX.
template<class T>
vector<T> BerlekampMassey(const vector<T> &a) {
  vector<T> me, he, old; T last = 0;
  for (int f = 0, i = 1; i <= SZ(a); ++i) {
    T d = -a[i - 1];
    for (int j = 0; j < SZ(me); ++j) d += a[i-j-2] * me[j];
    if (d == 0) continue;
    if (me.empty()) {
      me.resize(f = i), last = d; continue; }
    T k = -d / last; int s = i - f + SZ(he);
    bool grow = s >= SZ(me);
    if (grow) old.resize(SZ(me)),
      copy(me.begin(), me.end(), old.begin());
    me.resize(max(s, SZ(me))), me[i - f - 1] += -k;
    for (int j = 0; j < SZ(he); ++j)
      me[i - f + j] += he[j] * k;
    if (grow) he.swap(old), f = i, last = d;
  } return me; // a[i]=sum(c[j]*a[i-j-1]); O(n^2).
}
