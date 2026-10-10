// Sorted a*x=b (mod n) in [0,n); n>0, needs exgcd. O(log n+g).
vector<ll> mod_leq(ll a, ll b, ll n) {
  a %= n; if (a < 0) a += n;
  b %= n; if (b < 0) b += n;
  ll g = gcd(a, n), step = n / g;
  if (b % g) return {};
  ll x = (__int128)exgcd(a, n).X * (b / g) % step;
  if (x < 0) x += step;
  vector<ll> rt(g);
  for (ll i = 0; i < g; ++i) rt[i] = x + i * step;
  return rt;
}
