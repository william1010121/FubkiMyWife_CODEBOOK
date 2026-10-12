#include "../../codebook/1_Basic/Default_code.cpp"
ll mul(ll a, ll b, ll m) { return (__int128)a * b % m; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
map<ll, int> trial(ll n) {
  map<ll, int> ans;
  for (ll p = 2; p <= n / p; ++p)
    while (n % p == 0) ++ans[p], n /= p;
  if (n > 1) ++ans[n];
  return ans;
}
int main() {
  // Every representable 2-adic exponent; odd parts independently factored.
  for (ll odd : {1,3,5,9,25,27,97,99991,999983}) {
    auto base = trial(odd);
    for (int e = 0; e <= 62 && odd <= (LLONG_MAX >> e); ++e) {
      ll n = odd << e;
      auto want = base;
      if (e) want[2] = e;
      cnt.clear(); PollardRho(n); assert(cnt == want);
      PollardRho(1); assert(cnt == want);
      PollardRho(n);
      for (auto &[p, k] : want) k *= 2;
      assert(cnt == want); // Includes absence of zero-count entries.
    }
  }
  mt19937 rng(1508);
  for (int i = 0; i < 2000; ++i) {
    ll odd = (rng() % 1000000) | 1;
    int e = rng() % 43;
    while (odd > (LLONG_MAX >> e)) --e;
    auto want = trial(odd);
    if (e) want[2] = e;
    cnt.clear(); PollardRho(odd << e); assert(cnt == want);
  }
  auto begin = chrono::steady_clock::now();
  for (int repeat = 0; repeat < 1000; ++repeat)
    for (int e = 1; e <= 62; ++e) {
      cnt.clear(); PollardRho(1LL << e);
      assert((cnt == map<ll, int>{{2, e}}));
    }
  cout << "PASS Pollard even/odd/accumulation; 62000 power factors in "
       << chrono::duration<double>(chrono::steady_clock::now()-begin).count()
       << " s\n";
}
