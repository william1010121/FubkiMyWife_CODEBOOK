// Sorted a*x=b (mod n) in [0,n); n>0, needs exgcd. O(log n+g).
vector<ll> mod_leq(ll a, ll b, ll n) { // g answers must fit memory
  a %= n; if (a < 0) a += n;
  b %= n; if (b < 0) b += n;
  auto [u, v] = exgcd(a, n);
  ll g = (__int128)a * u + (__int128)n * v, step = n / g; // Bezout
  if (b % g) return {};
  ll x = (__int128)u * (b / g) % step;
  if (x < 0) x += step;
  vector<ll> rt(g); for (ll i = 0; i < g; ++i) rt[i] = x + i * step;
  return rt;
}
