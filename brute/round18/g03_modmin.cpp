#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ModMin.cpp"
// Independent extended-gcd oracle; 200000 contest-scale singleton queries.
ll oracle(ll a, ll m, ll target) {
  ll x = 1, y = 0, b = m;
  while (b) {
    ll q = a / b, rem = a % b, next = x - q*y;
    a = b, b = rem, x = y, y = next;
  }
  if (target % a) return -1;
  ll period = m / a;
  return (( (__int128)x * (target / a) % period) + period) % period;
}
int main() {
  mt19937_64 rng(180003);
  for (int i = 0; i < 200000; ++i) {
    ll m = 1 + rng() % 2000000000, a = rng() % m, l = rng() % m;
    assert(mod_min(a, m, l, l) == oracle(a, m, l));
  }
  // Intervals between consecutive multiples exercise both new endpoints.
  for (ll m = 1; m <= 80; ++m) for (ll a = 0; a < m; ++a)
    for (ll l = 0; l < m; ++l) for (ll r = l; r < m; ++r) {
      ll want = -1;
      for (ll k = 0; k < m; ++k) if (l <= a*k%m && a*k%m <= r) {
        want = k; break;
      }
      assert(mod_min(a, m, l, r) == want);
    }
  cout << "modmin singleton scale and interval audit: PASS\n";
}
