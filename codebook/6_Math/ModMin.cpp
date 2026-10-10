// 0<=a<m, 0<=l<=r<m; min k with l<=ak%m<=r, or -1. O(log m).
ll mod_min(ll a, ll m, ll l, ll r) {
  if (!a) return l ? -1 : 0;
  ll k = (l + a - 1) / a;
  if (k * a <= r) return k;
  ll c = m % a, y = mod_min(c, a, a - r % a, a - l % a);
  return y == -1 ? -1 : (l + y*c + a - 1) / a + y*(m/a);
}
