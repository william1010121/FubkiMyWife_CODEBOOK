// 1 <= n <= LLONG_MAX; needs mul() and prime(). Clear cnt per input.
map<ll, int> cnt;
void PollardRho(ll n) {
	if (n == 1) return;
	if (prime(n)) return ++cnt[n], void();
  if (n % 2 == 0) return PollardRho(n / 2),
    ++cnt[2], void();
  ll x = 2, y = 2, d = 1, p = 1;
  auto f = [&](ll z) {
    return (ll)(((__int128)mul(z, z, n) + p) % n);
  };
  while (true) {
		if (d != n && d != 1) { PollardRho(n / d);
		  PollardRho(d); return;
    } if (d == n) ++p;
		x = f(x), y = f(f(y));
		d = gcd(abs(x - y), n);
  }
}
