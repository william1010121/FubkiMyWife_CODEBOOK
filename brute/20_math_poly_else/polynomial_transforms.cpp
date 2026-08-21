#include <bits/stdc++.h>
using namespace std;

namespace fft_test {
#include "../../codebook/7_Polynomial/Fast_Fourier_Transform.cpp"
}

namespace fwt_test {
#include "../../codebook/7_Polynomial/Fast_Walsh_Transform.cpp"
}

namespace ntt_test {
constexpr int kN = 1 << 10;
constexpr int kMod = 998244353;
constexpr int kRoot = 3;
long long fpow(long long a, long long e) {
  long long r = 1;
  for (; e; e >>= 1, a = a * a % kMod) if (e & 1) r = r * a % kMod;
  return r;
}
void BitReverse(vector<int> &a, int n) {
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }
}
#include "../../codebook/7_Polynomial/Number_Theory_Transform.cpp"
}

namespace ntt2_test {
#define main ntt2_demo_main
#include "../../codebook/7_Polynomial/NTT.2.cpp"
#undef main
}

static vector<long long> naive_conv(const vector<int> &a, const vector<int> &b,
                                    long long mod = 0) {
  vector<long long> c(a.size() + b.size() - 1);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j) {
      c[i + j] += 1LL * a[i] * b[j];
      if (mod) c[i + j] %= mod;
    }
  return c;
}

static void test_fft() {
  fft_test::prefft();
  for (int n = 1; n <= 18; ++n) for (int m = 1; m <= 18; ++m) {
    vector<int> a(n), b(m);
    for (int i = 0; i < n; ++i) a[i] = (i % 5 == 0 ? 0 : (i * 17 % 31) - 15);
    for (int i = 0; i < m; ++i) b[i] = (i % 4 == 0 ? 0 : (i * 11 % 23) - 11);
    auto got = fft_test::multiply(a, b);
    auto want = naive_conv(a, b);
    int d = 1;
    while (d < max(n, m)) d <<= 1;
    d <<= 1;
    if ((int)got.size() != d || !equal(want.begin(), want.end(), got.begin()) ||
        any_of(got.begin() + want.size(), got.end(), [](int x) { return x != 0; })) {
      cerr << "FFT exhaustive-size mismatch n=" << n << " m=" << m << '\n';
      exit(1);
    }
    auto trim = fft_test::multiply(a, b, true);
    vector<long long> tw = want;
    while (!tw.empty() && tw.back() == 0) tw.pop_back();
    if (trim.size() != tw.size() || !equal(trim.begin(), trim.end(), tw.begin())) {
      cerr << "FFT trim mismatch n=" << n << " m=" << m << '\n';
      exit(1);
    }
  }
  mt19937 rng(0xff7);
  for (int tc = 0; tc < 350; ++tc) {
    int n = 1 + rng() % 70, m = 1 + rng() % 70;
    vector<int> a(n), b(m);
    for (int &x : a) x = (int)(rng() % 2001) - 1000;
    for (int &x : b) x = (int)(rng() % 2001) - 1000;
    auto got = fft_test::multiply(a, b);
    auto want = naive_conv(a, b);
    if (!equal(want.begin(), want.end(), got.begin())) {
      cerr << "FFT seeded random mismatch case=" << tc << '\n';
      exit(1);
    }
  }
}

static void test_fwt() {
  mt19937 rng(0xf771);
  for (int L = 0; L <= 7; ++L) {
    int n = 1 << L;
    vector<int> a(n), b(n), want(n);
    for (int &x : a) x = (int)(rng() % 9) - 4;
    for (int &x : b) x = (int)(rng() % 9) - 4;
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      want[i | j] += a[i] * b[j];
    auto x = a;
    fwt_test::fwt(x.data(), n, 1);
    fwt_test::fwt(b.data(), n, 1);
    for (int i = 0; i < n; ++i) x[i] *= b[i];
    fwt_test::fwt(x.data(), n, -1);
    if (x != want) { cerr << "FWT OR mismatch L=" << L << '\n'; exit(1); }
  }
  for (int L = 0; L <= 6; ++L) {
    int n = 1 << L;
    vector<int> a(n), b(n), got(n), want(n);
    for (int &x : a) x = (int)(rng() % 7) - 3;
    for (int &x : b) x = (int)(rng() % 7) - 3;
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      if (!(i & j)) want[i | j] += a[i] * b[j];
    fwt_test::subset_convolution(a.data(), b.data(), got.data(), L);
    if (got != want) { cerr << "subset convolution mismatch L=" << L << '\n'; exit(1); }
  }
}

static void test_number_transform() {
  ntt_test::Init();
  mt19937 rng(0x9911);
  for (int n = 1; n <= 512; n <<= 1) {
    vector<int> a(n);
    for (int &x : a) x = (int)(rng() % ntt_test::kMod);
    auto original = a;
    ntt_test::Transform(a, n); ntt_test::InverseTransform(a, n);
    if (a != original) { cerr << "NTT round-trip mismatch n=" << n << '\n'; exit(1); }
  }
  for (int na = 1; na <= 23; ++na) for (int nb = 1; nb <= 19; ++nb) {
    int n = 1;
    while (n < na + nb - 1) n <<= 1;
    vector<int> a(n), b(n);
    for (int i = 0; i < na; ++i) a[i] = (i & 1) ? ntt_test::kMod - 1 : i * 31;
    for (int i = 0; i < nb; ++i) b[i] = (i % 3) ? i * 17 : ntt_test::kMod - 2;
    auto want = naive_conv(vector<int>(a.begin(), a.begin() + na),
                           vector<int>(b.begin(), b.begin() + nb), ntt_test::kMod);
    ntt_test::Transform(a, n); ntt_test::Transform(b, n);
    for (int i = 0; i < n; ++i) a[i] = 1LL * a[i] * b[i] % ntt_test::kMod;
    ntt_test::InverseTransform(a, n);
    for (int i = 0; i < (int)want.size(); ++i)
      if (a[i] != want[i]) { cerr << "NTT convolution mismatch\n"; exit(1); }
  }
}

static void test_ntt2() {
  mt19937 rng(0x2a7u);
  for (int n = 1; n <= 256; n <<= 1) {
    vector<int> a(n);
    for (int &x : a) x = rng() % 998244353;
    auto original = a;
    ntt2_test::ctx1::ntt.apply(a.data(), n);
    ntt2_test::ctx1::ntt.apply(a.data(), n, true);
    if (a != original) { cerr << "NTT.2 round-trip mismatch n=" << n << '\n'; exit(1); }
  }
  for (int na = 1; na <= 15; ++na) for (int nb = 1; nb <= 15; ++nb) {
    int n = 1;
    while (n < na + nb - 1) n <<= 1;
    vector<int> a(n), b(n);
    for (int i = 0; i < na; ++i) a[i] = (i * i + 7) % 1000;
    for (int i = 0; i < nb; ++i) b[i] = (i * 13 + 3) % 1000;
    auto want = naive_conv(vector<int>(a.begin(), a.begin() + na),
                           vector<int>(b.begin(), b.begin() + nb), 998244353);
    ntt2_test::ctx1::ntt.apply(a.data(), n);
    ntt2_test::ctx1::ntt.apply(b.data(), n);
    for (int i = 0; i < n; ++i) a[i] = 1LL * a[i] * b[i] % 998244353;
    ntt2_test::ctx1::ntt.apply(a.data(), n, true);
    for (int i = 0; i < (int)want.size(); ++i)
      if (a[i] != want[i]) { cerr << "NTT.2 convolution mismatch\n"; exit(1); }
  }
}

int main() {
  test_fft();
  test_fwt();
  test_number_transform();
  test_ntt2();
  cout << "polynomial_transforms: PASS FFT/FWT/NTT/NTT.2 edge and oracle cases\n";
}
