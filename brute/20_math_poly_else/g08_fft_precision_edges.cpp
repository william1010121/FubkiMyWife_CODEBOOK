#include <bits/stdc++.h>
using namespace std;
#define ALL(v) v.begin(), v.end()

namespace tested {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}

static void check(const vector<int> &a, const vector<int> &b) {
  vector<long long> want(a.size() + b.size() - 1);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j)
      want[i + j] += 1LL * a[i] * b[j];
  for (auto x : want) if (x < INT_MIN || x > INT_MAX) abort();
  auto got = tested::mult(a, b);
  if (got.size() != want.size() ||
      !equal(got.begin(), got.end(), want.begin())) {
    cerr << "FFT precision mismatch: " << a.size() << ", "
         << b.size() << '\n';
    exit(1);
  }
}

int main() {
  mt19937 rng(0x804ff7);
  int cases = 0;
  for (int n : {1, 2, 3, 31, 32, 33, 127, 128, 129, 511, 2048}) {
    vector<int> a(n), b(n + 3);
    for (int &x : a) x = int(rng() % 2000000001u) - 1000000000;
    // Zero, identity and signed sparse products all fit int. Large
    // unbalanced coefficients expose cancellation in packed squaring.
    check(a, b); check(b, a); cases += 2;
    b[0] = 1;
    check(a, b); check(b, a); cases += 2;
    b.back() = -1;
    check(a, b); check(b, a); cases += 2;
    check(a, {1}); check(a, {-1}); cases += 2;
  }
  check({INT_MAX, INT_MIN, 0, INT_MAX}, {1}); ++cases;
  check({INT_MAX, INT_MIN, 0, INT_MAX}, {0, 0, 0}); ++cases;

  // Same-object and equal-value squares, then a late difference.
  mt19937 square_rng(0x16ff7);
  for (int n : {1, 2, 3, 31, 32, 33, 127, 128, 129, 511, 2048}) {
    vector<int> a(n);
    for (int &x : a) x = int(square_rng() % 2001) - 1000;
    auto b = a;
    check(a, a); check(a, b); cases += 2;
    ++b.back();
    check(a, b); check(b, a); cases += 2;
  }

  // Direct complex DFT checks ordering and sign, not just round trips.
  for (int n : {1, 2, 4, 8, 16, 32}) {
    vector<tested::cd> a(n);
    for (auto &x : a)
      x = {double(int(rng() % 27) - 13),
           double(int(rng() % 27) - 13)};
    for (bool inv : {false, true}) {
      auto got = a;
      tested::fft(got, inv);
      for (int k = 0; k < n; ++k) {
        tested::cd want = 0;
        for (int j = 0; j < n; ++j)
          want += a[j] * polar(1.0, 2 * tested::pi * j * k / n *
                                     (inv ? -1 : 1));
        if (inv) want /= n;
        if (abs(got[k] - want) > 1e-8) {
          cerr << "Direct complex DFT mismatch\n"; return 1;
        }
      }
      ++cases;
    }
  }
  cout << "g08_fft_precision_edges: PASS " << cases
       << " convolution and direct DFT cases\n";
}
