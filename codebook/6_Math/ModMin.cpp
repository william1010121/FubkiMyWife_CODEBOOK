// 0<=a<m, 0<=l<=r<m; min k with l<=ak%m<=r, or -1. O(log m).
// m>0; all evaluated ll products/sums must fit.
ll mod_min(ll a, ll m, ll l, ll r) {
  if (!a) return l ? -1 : 0; ll k = (l + a - 1) / a;
  if (k * a <= r) return k; // d: gap to next multiple of a
  ll c = m % a, d = k*a - l, y = mod_min(c, a, d - (r-l), d);
  return y == -1 ? -1 : (l + y*c + a - 1) / a + y*(m/a);
}
