#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/6_Math/Mod_Arithmetic.cpp"
#include "../../codebook/6_Math/Mod_Binomial.cpp"

int main() {
  mt19937_64 rng(19002);
  for (ll p : {2, 3, 97, 1009, 65537, 1000003}) {
    for (ll d = 2; d*d <= p; ++d) assert(p % d);
    ModBinomial b(p - 1, p);
    ll power = p;
    while (power <= LLONG_MAX / p) power *= p;
    clock_t start = clock();
    for (int q = 0; q < 200000; ++q) {
      int gap = 1 + q % 3;
      ll n = power - gap, k = rng() % (n + 1), r = k % p;
      ll expected;
      if (p == 2) expected = (k & n) == k;
      else {
        // C(p-j,r)=(-1)^r*C(r+j-1,j-1), j=1,2,3.
        expected = r > p-gap ? 0 : gap == 1 ? 1 : gap == 2 ?
          r + 1 : (ll)((__int128)(r+1)*(r+2)/2 % p);
        if (k & 1) expected = (p - expected) % p;
      }
      assert(b.lucas(n, k) == expected);
    }
    assert(b.lucas(LLONG_MIN, 0) == 0);
    assert(b.lucas(0, LLONG_MAX) == 0);
    assert(b.lucas(0, 0) == 1);
    cout << "p=" << p << ": 200000 Lucas identity queries PASS, "
         << 1000.*(clock()-start)/CLOCKS_PER_SEC << " ms CPU\n";
  }
}
