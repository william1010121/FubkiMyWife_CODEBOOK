#include <bits/stdc++.h>
using namespace std;

#include "codebook/7_Polynomial/Fast_Fourier_Transform.cpp"
namespace chris_fft {
#include "codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}

static vector<long long> oracle(const vector<int> &a, const vector<int> &b) {
  vector<long long> c(a.size() + b.size() - 1);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j) c[i + j] += 1LL * a[i] * b[j];
  return c;
}

int main() {
  prefft();
  mt19937 rng(0x5eed);
  for (int tc = 0; tc < 800; ++tc) {
    int n = 1 + rng() % 24, m = 1 + rng() % 24;
    vector<int> a(n), b(m);
    for (int &x : a) x = (int)(rng() % 41) - 20;
    for (int &x : b) x = (int)(rng() % 41) - 20;
    auto got = multiply(a, b);
    auto got_chris = chris_fft::mult(a, b);
    auto want = oracle(a, b);
    if (got.size() != 2 * (1u << (int)ceil(log2(max(n, m)))) ||
        !equal(want.begin(), want.end(), got.begin(),
               [](long long x, int y) { return x == y; }) ||
        got_chris.size() != want.size() ||
        !equal(want.begin(), want.end(), got_chris.begin(),
               [](long long x, int y) { return x == y; })) {
      cerr << "FFT mismatch at case " << tc << '\n';
      return 1;
    }
  }
  cout << "fft: PASS\n";
}
