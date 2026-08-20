#include <bits/stdc++.h>
using namespace std;
const long long MOD = 998244353;
long long pw(long long a, long long e) {
  long long r = 1;
  for (; e; e >>= 1, a = a * a % MOD) if (e & 1) r = r * a % MOD;
  return r;
}
vector<long long> mul(const vector<long long> &a, const vector<long long> &b, int n) {
  vector<long long> c(n);
  for (int i = 0; i < (int)a.size(); ++i) for (int j = 0; j < (int)b.size() && i + j < n; ++j)
    c[i + j] = (c[i + j] + a[i] * b[j]) % MOD;
  return c;
}
vector<long long> inv(const vector<long long> &a, int n) {
  vector<long long> b(n); b[0] = pw(a[0], MOD - 2);
  for (int i = 1; i < n; ++i) {
    for (int j = 1; j <= i && j < (int)a.size(); ++j)
      b[i] = (b[i] - a[j] * b[i - j]) % MOD;
    b[i] = (b[i] + MOD) % MOD * pw(a[0], MOD - 2) % MOD;
  }
  return b;
}
int main() {
  mt19937 rng(0x9e77);
  for (int tc = 0; tc < 300; ++tc) {
    const int n = 1 + rng() % 32;
    vector<long long> A(n); A[0] = 1;
    for (int i = 1; i < n; ++i) A[i] = rng() % 1000;
    vector<long long> exact(n); exact[0] = 1;
    for (int i = 1; i < n; ++i) {
      long long s = A[i];
      for (int j = 1; j < i; ++j) s = (s - exact[j] * exact[i - j]) % MOD;
      exact[i] = (s + MOD) % MOD * ((MOD + 1) / 2) % MOD;
    }
    vector<long long> q(1, 1);
    for (int len = 1; len < n; len <<= 1) {
      int m = min(n, len << 1);
      auto qq = mul(q, q, m);
      vector<long long> f(m);
      for (int i = 0; i < m; ++i) f[i] = ((i < (int)A.size() ? A[i] : 0) - (i < (int)qq.size() ? qq[i] : 0) + MOD) % MOD;
      auto iq = inv(q, m);
      auto delta = mul(f, iq, m);
      q.resize(m);
      long long half = (MOD + 1) / 2;
      for (int i = 0; i < m; ++i) q[i] = (q[i] + delta[i] * half) % MOD;
      for (long long &x : q) if (x < 0) x += MOD;
    }
    if (q != exact) { cerr << "Newton recurrence mismatch\n"; return 1; }
    if (mul(q, q, n) != A) { cerr << "Newton root mismatch\n"; return 1; }
  }
  cout << "newton: PASS\n";
}
