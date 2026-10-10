#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace fft_test {
#include "../../codebook/7_Polynomial/FFT_chrislaiisme.cpp"
}
namespace ntt_test {
#include "../../codebook/7_Polynomial/NTT_chrislaiisme.cpp"
}
int main() {
  const int n = 1 << 17;
  auto f = fft_test::mult(vector<int>(n, 1), vector<int>(n, -1));
  auto g = ntt_test::mult(vector<ll>(n, ntt_test::mod - 1),
                          vector<ll>(n, ntt_test::mod - 1));
  if (f.size() != 2 * n - 1 || g.size() != f.size()) return 1;
  for (int i = 0; i < 2 * n - 1; ++i) {
    int want = min(i + 1, 2 * n - 1 - i);
    if (f[i] != -want || g[i] != want) return 1;
  }
  vector<int> a(n, 1), b = a;
  auto square = fft_test::mult(a, a), equal = fft_test::mult(a, b);
  if (square != equal || square.size() != 2 * n - 1) return 1;
  for (int i = 0; i < 2 * n - 1; ++i)
    if (square[i] != min(i + 1, 2 * n - 1 - i)) return 1;
  for (int sz : {1, 2, 1 << 18}) {
    vector<fft_test::cd> a(sz);
    vector<ll> b(sz);
    for (int i = 0; i < sz; ++i)
      a[i] = i % 13 - 6, b[i] = i * 1234567LL % ntt_test::mod;
    auto x = a; auto y = b;
    fft_test::fft(a); fft_test::ifft(a);
    ntt_test::ntt(b); ntt_test::intt(b);
    if (b != y) return 1;
    for (int i = 0; i < sz; ++i) if (abs(a[i] - x[i]) > 1e-6)
      return 1;
  }
  if (ntt_test::pwr(3, 0, 7) != 7 ||
      ntt_test::pwr(3, 5, 7) != 1701) return 1;
  cout << "g08_transform_stress: PASS large triangular convolutions "
          "and direct inverse round trips\n";
}
