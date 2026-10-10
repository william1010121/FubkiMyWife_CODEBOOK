// 1<=n<=LLONG_MAX; needs mul(),prime(). Clear cnt per input.
map<ll, int> cnt;
void PollardRho(ll n) {
  while (n % 2 == 0) ++cnt[2], n /= 2;
  if (n == 1) return;
  if (prime(n)) return ++cnt[n], void();
  for (ll p = 1; ; ++p) {
    ll x = 2, y = 2, d = 1, u, v; int batch = 1;
    auto f = [&](ll z) { return (ll)(((__int128)mul(z,z,n)+p)%n); };
    while (d == 1) {
      u = x, v = y; ll q = 1;
      for (int i = 0; i < batch && q; ++i) {
        x = f(x), y = f(f(y)); q = mul(q, abs(x - y), n);
      } d = gcd(q, n); batch = min(64, batch * 2);
    }
    if (d == n) do { // Replay last batch to isolate a factor.
      u = f(u), v = f(f(v)); d = gcd(abs(u - v), n);
    } while (d == 1);
    if (d == n) continue;
    PollardRho(d), PollardRho(n / d); return;
  }
}
