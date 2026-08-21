ll solve(ll x1, ll m1, ll x2, ll m2) {
  ll g = gcd(m1, m2); __int128 d = (__int128)x2 - x1;
  if (d % g) return -1; // no sol
  m1 /= g, m2 /= g; pll p = exgcd(m1, m2);
  __int128 t = (__int128)p.first * (d / g) % m2;
  if (t < 0) t += m2;
  __int128 lcm = (__int128)m1 * m2 * g;
  __int128 res = (__int128)x1 + (__int128)m1 * g * t;
  return (ll)((res % lcm + lcm) % lcm);
}
