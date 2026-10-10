map<ll, int> cnt; // Clear per input; mul returns exact (a*b)%n.
void PollardRho(ll n) { // 1<=n<=LLONG_MAX; needs correct prime().
  while (n % 2 == 0) ++cnt[2], n /= 2;
  if (n == 1) return;
  if (prime(n)) return ++cnt[n], void();
  ll x=0, y=0, q=1, d=1, seed=1; int batch=1;
  auto f = [&](ll z) { ll t = mul(z,z,n);
    return t >= n-seed ? t-(n-seed) : t+seed; };
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
