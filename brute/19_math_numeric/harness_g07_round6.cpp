#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"
#include "../../codebook/6_Math/Fraction.cpp"
constexpr int MAXN = 128, MAXP = 1000001;
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
#include "../../codebook/6_Math/fac_no_p.cpp"
ll mul(ll a, ll b, ll m) { return (__int128)a*b%m; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a,n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
void check_factor(ll n) {
  cnt.clear(); PollardRho(n); __int128 product = 1;
  for (auto [p,e] : cnt) {
    assert(e > 0 && prime(p)); while (e--) product *= p;
  }
  assert(product == n);
}
int main() {
  mt19937_64 rng(60607);
  for (ll p : {1000000007LL, 9223372036854775783LL}) {
    ModBinomial c(200000,p); ModArithmetic mod(p);
    for (int i = 1; i <= 200000; ++i) {
      assert(c.fac[i] == mod.mul(c.fac[i-1],i));
      assert(mod.mul(c.fac[i],c.ifac[i]) == 1);
    }
    for (int test = 0; test < 300; ++test) {
      int n = rng()%200001, k = min(n,int(rng()%20));
      ll want = 1;
      for (int j = 1; j <= k; ++j)
        want = mod.mul(mod.mul(want,n-j+1),mod.inv(j));
      assert(c.C(n,k) == want && c.C(n,n-k) == want);
    }
  }
  auto a = make_unique<matrix>(); a->n = a->m = MAXN;
  for (int i = 0; i < MAXN; ++i) {
    ll rhs = 0;
    for (int j = 0; j < MAXN; ++j) {
      ll x = min(i,j)+1; a->M[i][j] = fraction(x); rhs += x;
    }
    a->M[i][MAXN] = fraction(rhs);
  }
  assert(a->solve() == MAXN);
  for (int i = 0; i < MAXN; ++i) {
    assert(a->sol[i].n == 1 && a->sol[i].d == 1);
    for (int j = 0; j < MAXN; ++j)
      assert(a->M[i][j].n == (i == j) && a->M[i][j].d == 1);
  }
  for (auto [p,pk] : {pll{2,524288}, pll{3,531441}}) {
    for (ll n : {0,1,1000,10000}) {
      fill_n(prod,MAXP,-7); ll want = 1, prefix = 1;
      ll got = fac_no_p(n,p,pk);
      assert(prod[0] == 1);
      for (ll i = 1; i <= n; ++i) {
        ll x = i; while (x%p == 0) x /= p;
        want = want*x%pk;
        if (i%p) prefix = prefix*i%pk;
        assert(prod[i] == prefix);
      }
      assert(got == want);
    }
  }
  ll p = 1000000000;
  for (int test = 0; test < 16; ++test) {
    while (!prime(++p)) {} ll q = p+1000;
    while (!prime(++q)) {} check_factor(p*q); p += 1000;
  }
  for (ll p : {3,5,7,11}) {
    ll n = 1;
    while (n <= LLONG_MAX/p) { n *= p; check_factor(n); }
  }
  cout << "PASS g07 round6: 200k binomial tables, dense exact system, "
       << "small-n/large-pk factorials, 1e18 semiprimes/prime powers\n";
}
