#include <bits/stdc++.h>
using namespace std;
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
#include "codebook/7_Polynomial/Number_Theory_Transform.cpp"

static vector<int> oracle(const vector<int> &a, const vector<int> &b) {
  vector<int> c(a.size() + b.size() - 1);
  for (int i = 0; i < (int)a.size(); ++i)
    for (int j = 0; j < (int)b.size(); ++j)
      c[i + j] = (c[i + j] + 1LL * a[i] * b[j]) % kMod;
  return c;
}
int main() {
  Init();
  mt19937 rng(0x91a2);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 1 << (rng() % 7);
    vector<int> a(n);
    for (int &x : a) x = rng() % kMod;
    auto original = a;
    Transform(a, n); InverseTransform(a, n);
    if (a != original) { cerr << "NTT round-trip mismatch\n"; return 1; }
    int na = 1 + rng() % 18, nb = 1 + rng() % 18, z = 1;
    while (z < na + nb - 1) z <<= 1;
    vector<int> x(z), y(z);
    for (int i = 0; i < na; ++i) x[i] = rng() % 1000;
    for (int i = 0; i < nb; ++i) y[i] = rng() % 1000;
    auto want = oracle(vector<int>(x.begin(), x.begin() + na),
                       vector<int>(y.begin(), y.begin() + nb));
    Transform(x, z); Transform(y, z);
    for (int i = 0; i < z; ++i) x[i] = 1LL * x[i] * y[i] % kMod;
    InverseTransform(x, z);
    for (int i = 0; i < (int)want.size(); ++i)
      if (x[i] != want[i]) { cerr << "NTT convolution mismatch\n"; return 1; }
  }
  cout << "number_theory_transform: PASS\n";
}
