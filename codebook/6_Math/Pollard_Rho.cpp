// 1<=n<=LLONG_MAX; needs mul(),prime(). Clear cnt per input.
map<ll, int> cnt;
void PollardRho(ll n) {
  while (n % 2 == 0) ++cnt[2], n /= 2;
  if (n == 1) return;
  if (prime(n)) return ++cnt[n], void();
  ll x=0, y=0, q=1, d=1, seed=1; int batch=1;
  auto f = [&](ll z) { return (ll)(((__int128)mul(z,z,n)+seed)%n); };
  while (d == 1) {
    for (int i = 0; i < batch; ++i) {
      if (x == y) { x = seed = (seed + 1) % n; y = f(x); }
      ll t = mul(q, abs(x - y), n);
      if (t) q = t; // Keep q!=0, so gcd(q,n)<n; no batch replay.
      x = f(x), y = f(f(y));
    } d = gcd(q, n); batch = min(64, batch * 2);
  }
  PollardRho(d), PollardRho(n / d);
}
