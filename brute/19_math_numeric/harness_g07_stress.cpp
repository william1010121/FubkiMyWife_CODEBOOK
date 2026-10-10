#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/ax+by=gcd.cpp"
#include "../../codebook/6_Math/Linear_Congruence.cpp"
#include "../../codebook/6_Math/floor_sum.cpp"
#include "../../codebook/6_Math/Fraction.cpp"
constexpr int MAXN = 128;
#include "../../codebook/6_Math/Simultaneous_Equations.cpp"
ll mul(ll a, ll b, ll m) { return (__int128)a * b % m; }
#include "../../codebook/6_Math/Miller_Rabin.cpp"
bool prime(ll n) {
  if (n < 2) return false;
  for (ll a : {2,325,9375,28178,450775,9780504,1795265022})
    if (!Miller_Rabin(a, n)) return false;
  return true;
}
#include "../../codebook/6_Math/Pollard_Rho.cpp"
int main() {
  const ll n = 1000000;
  auto v = mod_leq(0, 0, n);
  assert((ll)v.size() == n);
  for (ll i = 0; i < n; ++i) assert(v[i] == i);
  assert(mod_leq(0, 1, n).empty());
  const ll m = 1000000007;
  for (int i = 0; i < 1000; ++i) {
    assert(floor_sum(m, m, m - 1, 0) == (m - 1)*(m - 2)/2);
    assert(floor_sum(m, m, m, 0) == m*(m - 1)/2);
    assert(floor_sum(0, m, m - 1, m - 1) == 0);
  }
  matrix a;
  a.n = a.m = MAXN;
  for (int i = 0; i < MAXN; ++i) {
    a.M[i][MAXN - 1 - i] = fraction(1);
    a.M[i][MAXN] = fraction(MAXN - i);
  }
  assert(a.solve() == MAXN);
  for (int i = 0; i < MAXN; ++i)
    assert(a.sol[i].n == i + 1 && a.sol[i].d == 1);
  for (int i = 0; i < MAXN; ++i) {
    for (int j = 0; j < MAXN; ++j) a.M[i][j] = fraction(j <= i);
    a.M[i][MAXN] = fraction((i + 1)*(i + 2)/2);
  }
  assert(a.solve() == MAXN);
  for (int i = 0; i < MAXN; ++i) {
    assert(a.sol[i].n == i + 1 && a.sol[i].d == 1);
    for (int j = 0; j < MAXN; ++j)
      assert(a.M[i][j].n == (i == j) && a.M[i][j].d == 1);
  }
  cnt.clear(); PollardRho(1LL << 62);
  assert((cnt == map<ll, int>{{2, 62}}));
  cnt.clear(); PollardRho(3LL << 60);
  assert((cnt == map<ll, int>{{2, 60}, {3, 1}}));
  cout << "PASS g07 stress: 1e6 solutions, 1e9 floor sums, "
       << "128x128 sparse/triangular systems, large powers of 2\n";
}
