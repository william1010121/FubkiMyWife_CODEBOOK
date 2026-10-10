// All solutions of a*x == b (mod n); n > 0. Needs exgcd.
vector<ll> mod_leq(ll a, ll b, ll n) {
  a %= n; if (a < 0) a += n;
  b %= n; if (b < 0) b += n;
  vector<ll> rt;
  ll g = gcd(a, n);
  pll p = exgcd(a, n);
  if (!(b % g)) {
    ll step = n / g;
    ll x = (__int128)p.X * (b / g) % step;
    if (x < 0) x += step;
    for (ll i = 0; i < g; ++i) rt.pb(x + i * step);
  }
  return rt;
}
