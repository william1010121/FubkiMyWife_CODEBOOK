#include <bits/stdc++.h>
using namespace std;
using u64 = uint64_t;
using u128 = __uint128_t;

u64 mul(u64 a, u64 b, u64 m) { return (u128)a * b % m; }
u64 power(u64 a, u64 e, u64 m) {
  u64 r = 1 % m;
  for (; e; e >>= 1, a = mul(a, a, m)) if (e & 1) r = mul(r, a, m);
  return r;
}
bool is_prime(u64 n) {
  if (n < 2) return false;
  for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
                23ULL, 29ULL, 31ULL, 37ULL}) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  u64 d = n - 1, s = 0;
  while (!(d & 1)) d >>= 1, ++s;
  for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
                9780504ULL, 1795265022ULL}) {
    if (a % n == 0) continue;
    u64 x = power(a % n, d, n);
    if (x == 1 || x == n - 1) continue;
    bool ok = false;
    for (u64 r = 1; r < s; ++r) if ((x = mul(x, x, n)) == n - 1) ok = true;
    if (!ok) return false;
  }
  return true;
}

int main() {
  ifstream in("codebook/6_Math/Primes.cpp");
  string source((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  size_t begin = source.find("/*");
  if (begin == string::npos) { cerr << "missing prime-list comment\n"; return 1; }
  begin = source.find_first_not_of(" \t", begin + 2);
  size_t end = source.find('\n', begin);
  vector<u64> a;
  for (size_t i = begin; i < end;) {
    if (!isdigit((unsigned char)source[i])) { ++i; continue; }
    u128 x = 0;
    while (i < source.size() && isdigit((unsigned char)source[i]))
      x = x * 10 + source[i++] - '0';
    if (x > numeric_limits<u64>::max()) {
      cerr << "numeric token overflow\n";
      return 1;
    }
    a.push_back((u64)x);
  }
  if (a.empty()) { cerr << "no prime tokens found\n"; return 1; }
  for (size_t i = 0; i < a.size(); ++i) {
    if (!is_prime(a[i])) { cerr << "not prime: " << a[i] << '\n'; return 1; }
    if (i && a[i - 1] >= a[i]) {
      cerr << "list is not strictly increasing at index " << i << '\n';
      return 1;
    }
  }
  cout << "Primes.cpp OK: " << a.size() << " increasing prime values\n";
}
