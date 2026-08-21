#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using cpx = complex<ll>;

#include "../../codebook/6_Math/Gaussian_gcd.cpp"

ll norm2(cpx z) {
  return z.real() * z.real() + z.imag() * z.imag();
}

bool gaussian_divides(cpx z, cpx d) {
  const ll dn = norm2(d);
  if (dn == 0) return false;
  const ll dot = z.real() * d.real() + z.imag() * d.imag();
  const ll cross = z.imag() * d.real() - z.real() * d.imag();
  return dot % dn == 0 && cross % dn == 0;
}

ll brute_best_norm(cpx a, cpx b) {
  const ll na = norm2(a), nb = norm2(b);
  const ll bound = na == 0 ? nb : min(na, nb);
  const int lim = static_cast<int>(sqrt(static_cast<long double>(bound))) + 2;
  ll best = 0;
  for (int x = -lim; x <= lim; ++x) for (int y = -lim; y <= lim; ++y) {
    cpx d(x, y);
    const ll nd = norm2(d);
    if (nd != 0 && nd <= bound && gaussian_divides(a, d) &&
        gaussian_divides(b, d))
      best = max(best, nd);
  }
  return best;
}

void check(cpx a, cpx b) {
  if (b == cpx(0, 0)) return;  // the snippet has no b=0 guard by contract.
  const cpx got = gaussian_gcd(a, b);
  if (!gaussian_divides(a, got) || !gaussian_divides(b, got)) {
    cerr << "Gaussian gcd is not a common divisor a=" << a.real() << "+"
         << a.imag() << "i b=" << b.real() << "+" << b.imag() << "i\n";
    exit(1);
  }
  const ll want = brute_best_norm(a, b);
  if (norm2(got) != want) {
    cerr << "Gaussian gcd norm mismatch a=" << a.real() << "+" << a.imag()
         << "i b=" << b.real() << "+" << b.imag() << "i got=" << norm2(got)
         << " want=" << want << '\n';
    exit(1);
  }
}

int main() {
  for (ll ar = -6; ar <= 6; ++ar) for (ll ai = -6; ai <= 6; ++ai)
    for (ll br = -6; br <= 6; ++br) for (ll bi = -6; bi <= 6; ++bi)
      check(cpx(ar, ai), cpx(br, bi));

  mt19937_64 rng(190027);
  for (int tc = 0; tc < 3000; ++tc) {
    cpx a(static_cast<ll>(rng() % 141) - 70,
          static_cast<ll>(rng() % 141) - 70);
    cpx b(static_cast<ll>(rng() % 141) - 70,
          static_cast<ll>(rng() % 141) - 70);
    if (b == cpx(0, 0)) {
      --tc;
      continue;
    }
    check(a, b);
  }

  cout << "PASS Gaussian_gcd signed exhaustive box, zero-a, and seeded random oracle cases\n";
}
