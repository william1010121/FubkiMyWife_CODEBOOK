// 1<=n<=LLONG_MAX; needs mul(),prime(). Clear cnt per input.
map<ll, int> cnt;
void PollardRho(ll n) {
  while (n % 2 == 0) ++cnt[2], n /= 2;
  if (n == 1) return;
  if (prime(n)) return ++cnt[n], void();
  for (ll p = 1; ; ++p) {
    ll x = 2, y = 2, d = 1;
    auto f = [&](ll z) { return (ll)(((__int128)mul(z,z,n)+p)%n); };
    while (d == 1) {
      x = f(x), y = f(f(y)); d = gcd(abs(x - y), n);
    }
    if (d == n) continue;
    PollardRho(d), PollardRho(n / d); return;
  }
}
