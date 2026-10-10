// Field T: zero init, arithmetic, nonzero division, ==0; size<INT_MAX.
template<class T> vector<T> BerlekampMassey(const vector<T> &output) {
  vector<T> me, he; T last = 0;
  for (int f = 0, i = 1; i <= SZ(output); ++i) {
    T d = -output[i - 1];
    for (int j = 0; j < SZ(me); ++j) d += output[i - j - 2] * me[j];
    if (d == 0) continue;
    if (me.empty()) { me.resize(f = i), last = d; continue; }
    T k = -d / last; vector<T> o(max(i - f + SZ(he), SZ(me)));
    o[i - f - 1] = -k;
    for (int j = 0; j < SZ(he); ++j) o[i - f + j] = he[j] * k;
    for (int j = 0; j < SZ(me); ++j) o[j] += me[j];
    if (i - f + SZ(he) >= SZ(me)) he = move(me), f = i, last = d;
    me = move(o);
  } return me; // a[i]=sum(c[j]*a[i-j-1]); O(n^2).
}
