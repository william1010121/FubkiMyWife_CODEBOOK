#include <bits/stdc++.h>
using namespace std;
using u64 = uint64_t;
static u64 power(u64 a, u64 e, u64 p) {
  u64 r = 1;
  for (; e; e >>= 1, a = (u64)((__uint128_t)a * a % p))
    if (e & 1) r = (u64)((__uint128_t)r * a % p);
  return r;
}
static bool prime(u64 n) {
  if (n < 2) return false;
  for (u64 d = 2; d * d <= n; ++d) if (n % d == 0) return n == d;
  return true;
}
int main() {
  const char *template_path = "codebook/7_Polynomial/Number_Theory_Transform_Prime";
  ifstream input(template_path);
  if (!input) { cerr << "cannot open " << template_path << '\n'; return 1; }
  string source((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
  regex pair_pattern(R"((\d+)\s*&\s*(\d+))");
  vector<pair<u64, u64>> table;
  for (sregex_iterator it(source.begin(), source.end(), pair_pattern), end;
       it != end; ++it) {
    table.emplace_back(stoull((*it)[1].str()), stoull((*it)[2].str()));
  }
  if (table.size() != 18) {
    cerr << "expected 18 NTT prime/root pairs, got " << table.size() << '\n';
    return 1;
  }
  for (auto [p, g] : table) {
    if (!prime(p) || power(g, p - 1, p) != 1) {
      cerr << "invalid NTT prime/root pair: " << p << '\n'; return 1;
    }
    u64 x = p - 1;
    vector<u64> factors;
    for (u64 d = 2; d * d <= x; ++d) if (x % d == 0) {
      factors.push_back(d); while (x % d == 0) x /= d;
    }
    if (x > 1) factors.push_back(x);
    for (u64 q : factors) if (power(g, (p - 1) / q, p) == 1) {
      cerr << "root is not primitive for " << p << '\n'; return 1;
    }
  }
  cout << "ntt_prime: PASS\n";
}
