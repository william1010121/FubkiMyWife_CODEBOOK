#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using cpx = complex<ll>;
#include "../../../codebook/6_Math/Gaussian_gcd.cpp"

bool gaussian_divides(cpx z, cpx d) {
  ll x = d.real(), y = d.imag();
  ll nr = x * x + y * y;
  if (!nr) return false;
  ll zr = z.real(), zi = z.imag();
  return (zr * x + zi * y) % nr == 0 &&
         (zi * x - zr * y) % nr == 0;
}
int main() {
  for (ll ar = -8; ar <= 8; ++ar) for (ll ai = -8; ai <= 8; ++ai)
    for (ll br = -8; br <= 8; ++br) for (ll bi = -8; bi <= 8; ++bi) {
      cpx a(ar, ai), b(br, bi);
      if (b == cpx(0, 0)) continue;
      cpx g = gaussian_gcd(a, b);
      if (!gaussian_divides(a, g) || !gaussian_divides(b, g)) {
        cerr << "Gaussian gcd non-divisor a=" << ar << "+" << ai
             << "i b=" << br << "+" << bi << "i\n";
        return 1;
      }
      ll best = 0;
      for (ll x = -20; x <= 20; ++x) for (ll y = -20; y <= 20; ++y) {
        cpx d(x, y);
        if (d != cpx(0, 0) && gaussian_divides(a, d) && gaussian_divides(b, d))
          best = max(best, x * x + y * y);
      }
      ll got = g.real() * g.real() + g.imag() * g.imag();
      if (got != best) {
        cerr << "Gaussian gcd norm mismatch a=" << ar << "+" << ai
             << "i b=" << br << "+" << bi << "i got=" << got
             << " want=" << best << '\n';
        return 1;
      }
    }
  cout << "PASS Gaussian_gcd exhaustive box [-8,8]^4\n";
}
