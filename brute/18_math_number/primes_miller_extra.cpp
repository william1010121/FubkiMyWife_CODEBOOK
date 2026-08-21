#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using u64 = uint64_t;
using u128 = __uint128_t;

ll mul(ll a, ll b, ll m) {
  return static_cast<ll>((u128)static_cast<u64>(a) * static_cast<u64>(b) %
                         static_cast<u64>(m));
}

#include "../../codebook/6_Math/Miller_Rabin.cpp"

bool template_prime(ll n) {
  if (n < 2) return false;
  for (ll p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 23LL,
               29LL, 31LL, 37LL}) {
    if (n == p) return true;
    if (n % p == 0) return false;
  }
  for (ll a : {2LL, 325LL, 9375LL, 28178LL, 450775LL, 9780504LL,
               1795265022LL}) {
    if (!Miller_Rabin(a, n)) return false;
  }
  return true;
}

vector<u64> parse_prime_list() {
  ifstream in("codebook/6_Math/Primes.cpp");
  string source((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
  size_t begin = source.find("/*"), end = source.find("*/", begin + 2);
  if (begin == string::npos || end == string::npos) {
    cerr << "Primes.cpp is missing its list comment\n";
    exit(1);
  }
  vector<u64> values;
  for (size_t i = begin + 2; i < end;) {
    if (!isdigit(static_cast<unsigned char>(source[i]))) {
      ++i;
      continue;
    }
    u128 value = 0;
    while (i < end && isdigit(static_cast<unsigned char>(source[i])))
      value = value * 10 + static_cast<unsigned>(source[i++] - '0');
    if (value > numeric_limits<u64>::max()) {
      cerr << "prime-list token overflows uint64_t\n";
      exit(1);
    }
    values.push_back(static_cast<u64>(value));
  }
  return values;
}

int main() {
  constexpr int limit = 1'200'000;
  vector<char> sieve(limit + 1, true);
  sieve[0] = sieve[1] = false;
  for (int p = 2; 1LL * p * p <= limit; ++p)
    if (sieve[p])
      for (int x = p * p; x <= limit; x += p) sieve[x] = false;

  long long checked = 0;
  for (int n = 0; n <= limit; ++n) {
    if (template_prime(n) != static_cast<bool>(sieve[n])) {
      cerr << "Miller-Rabin small mismatch n=" << n
           << " want=" << static_cast<int>(sieve[n])
           << " got=" << template_prime(n) << '\n';
      return 1;
    }
    ++checked;
  }

  struct Boundary {
    ll n;
    bool want;
    ll factor;
  };
  const vector<Boundary> boundary = {
      {0, false, 0},
      {1, false, 0},
      {2, true, 0},
      {3, true, 0},
      {4, false, 2},
      {1000000007LL, true, 0},
      {1000000009LL, true, 0},
      {1000000007LL * 1000000009LL, false, 1000000007LL},
      {341550071728321LL, false, 0},
      {3825123056546413051LL, false, 0},
      {2305843009213693951LL, true, 0},
      {4611686018427387847LL, true, 0},
      {9223372036854775783LL, true, 0},
      {LLONG_MAX - 2, false, 5},
      {LLONG_MAX - 1, false, 2},
      {LLONG_MAX, false, 7},
  };
  for (const auto &tc : boundary) {
    bool got = template_prime(tc.n);
    if (got != tc.want || (tc.factor && tc.n % tc.factor != 0)) {
      cerr << "Miller-Rabin boundary mismatch n=" << tc.n
           << " want=" << tc.want << " got=" << got << '\n';
      return 1;
    }
    ++checked;
  }

  mt19937_64 rng(0x18A11CE5EEDULL);
  for (int tc = 0; tc < 1200; ++tc) {
    ll n = 2 + static_cast<ll>(rng() % (limit - 1));
    if (template_prime(n) != static_cast<bool>(sieve[n])) {
      cerr << "Miller-Rabin seeded mismatch n=" << n << '\n';
      return 1;
    }
    ++checked;
  }
  for (int tc = 0; tc < 600; ++tc) {
    ll factor = 2 + static_cast<ll>(rng() % 100000);
    ll n = factor * 1000000007LL;
    if (template_prime(n)) {
      cerr << "Miller-Rabin accepted constructed composite n=" << n
           << " factor=" << factor << '\n';
      return 1;
    }
    ++checked;
  }

  const vector<u64> listed = parse_prime_list();
  if (listed.size() < 20 || listed.front() != 12721ULL ||
      listed.back() != 18446744073709551557ULL) {
    cerr << "Primes.cpp list shape changed unexpectedly\n";
    return 1;
  }
  for (size_t i = 0; i < listed.size(); ++i) {
    if (i && listed[i - 1] >= listed[i]) {
      cerr << "Primes.cpp is not strictly increasing at index " << i << '\n';
      return 1;
    }
    if (listed[i] <= static_cast<u64>(limit) && !sieve[listed[i]]) {
      cerr << "Primes.cpp contains a small composite " << listed[i] << '\n';
      return 1;
    }
    if (listed[i] <= static_cast<u64>(LLONG_MAX) &&
        !template_prime(static_cast<ll>(listed[i]))) {
      cerr << "Miller-Rabin rejected listed value " << listed[i] << '\n';
      return 1;
    }
  }

  cout << "PASS primes/Miller-Rabin extra: " << checked
       << " cases plus " << listed.size() << " listed values\n";
}
