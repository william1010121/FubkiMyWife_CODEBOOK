#include "../../codebook/1_Basic/Default_code.cpp"
ll zero_products = 0;
ll mul(ll a, ll b, ll m) {
  ll r = (__int128)a * b % m;
  zero_products += a && b && a != b && !r;
  return r;
}
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
int checked = 0;
void check(const map<ll, int> &want) {
  __int128 product = 1;
  for (auto [p, e] : want) {
    assert(prime(p));
    for (int i = 0; i < e; ++i) product *= p;
  }
  assert(product <= LLONG_MAX);
  cnt.clear(); PollardRho((ll)product);
  assert(cnt == want); ++checked;
}
int main() {
  // Fixed x*x+1 has equal-length cycles for both of these primes.
  check({{352523, 1}, {352817, 1}});
  check({{5, 2}}); check({{7, 2}}); check({{11, 4}});
  for (ll p : {3,5,7,11,13,17,19,23,29,31,37,41,43}) {
    ll power = p;
    for (int e = 1; power <= LLONG_MAX / p; ++e, power *= p)
      check({{p, e}});
  }
  vector<ll> ps;
  for (ll p = 1000000; ps.size() < 200; ++p)
    if (prime(p)) ps.pb(p);
  mt19937 rng(70707);
  for (int i = 0; i < 600; ++i) {
    map<ll, int> want;
    ++want[ps[rng() % ps.size()]]; ++want[ps[rng() % ps.size()]];
    if (i % 3 == 0) ++want[ps[rng() % ps.size()]];
    check(want);
  }
  ll p = 2000000000;
  for (int i = 0; i < 16; ++i) {
    while (!prime(++p)) {} ll q = p + 2000;
    while (!prime(++q)) {} check({{p, 1}, {q, 1}}); p = q;
  }
  assert(zero_products > 0); // Nonsquare zeros exercise batch recovery.
  cout << "PASS round7: " << checked
       << " rho collision/power/stress cases\n";
}
