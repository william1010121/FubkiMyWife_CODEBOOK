#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template<class T> using V = vector<T>;

namespace gauss_test {
#include "../../codebook/6_Math/Gaussian_Eliminatin.cpp"
}

struct Mod {
  static constexpr ll P = 1000000007;
  ll v;
  Mod(ll x = 0) : v((x % P + P) % P) {}
  Mod &operator+=(Mod x) { return v = (v + x.v) % P, *this; }
  Mod &operator-=(Mod x) { return v = (v - x.v + P) % P, *this; }
  Mod &operator*=(Mod x) { return v = v * x.v % P, *this; }
  friend Mod operator+(Mod a, Mod b) { return a += b; }
  friend Mod operator-(Mod a, Mod b) { return a -= b; }
  friend Mod operator-(Mod a) { return Mod(-a.v); }
  friend Mod operator*(Mod a, Mod b) { return a *= b; }
  static Mod power(Mod a, ll e) {
    Mod r = 1;
    for (; e; e >>= 1, a *= a) if (e & 1) r *= a;
    return r;
  }
  friend Mod operator/(Mod a, Mod b) { return a * power(b, P - 2); }
  friend bool operator==(Mod a, Mod b) { return a.v == b.v; }
  friend bool operator!=(Mod a, Mod b) { return !(a == b); }
};

namespace bm_test {
#define SZ(x) int((x).size())
#define pb push_back
#include "../../codebook/6_Math/Berlekamp-Massey.cpp"
#undef pb
#undef SZ
}

namespace pi_test {
#include "../../codebook/6_Math/PiCount.cpp"
}

namespace fac_test {
#define MAXP 1000000
ll mpow(ll a, ll e, ll m) {
  ll r = 1 % m;
  for (; e; e >>= 1, a = (__int128)a * a % m)
    if (e & 1) r = (__int128)r * a % m;
  return r;
}
#include "../../codebook/6_Math/fac_no_p.cpp"
#undef MAXP
}

static void check_gaussian(const vector<vector<double>> &input) {
  auto a = input;
  gauss_test::GAS(a);
  const int n = (int)input.size();
  vector<int> pivot(n, -1);
  vector<double> x(n, 0);
  for (int i = 0; i < n; ++i) {
    int col = -1;
    for (int j = 0; j < n; ++j)
      if (abs(a[i][j]) > 1e-8) { col = j; break; }
    if (col < 0) {
      if (abs(a[i][n]) > 1e-7) {
        cerr << "Gaussian produced an inconsistent zero row\n";
        exit(1);
      }
      continue;
    }
    if (pivot[col] != -1 || abs(a[i][col] - 1) > 1e-7) {
      cerr << "Gaussian row is not normalized RREF\n";
      exit(1);
    }
    pivot[col] = i;
    x[col] = a[i][n]; // free variables are required to stay zero.
  }
  for (const auto &row : input) {
    double lhs = 0;
    for (int j = 0; j < n; ++j) lhs += row[j] * x[j];
    if (abs(lhs - row[n]) > 1e-7) {
      cerr << "Gaussian solution violates an original equation\n";
      exit(1);
    }
  }
}

static bool solve_mod(vector<vector<Mod>> a, vector<Mod> b,
                      int vars, vector<Mod> *answer = nullptr) {
  int row = 0;
  for (int col = 0; col < vars && row < (int)a.size(); ++col) {
    int who = row;
    while (who < (int)a.size() && a[who][col] == Mod(0)) ++who;
    if (who == (int)a.size()) continue;
    swap(a[who], a[row]); swap(b[who], b[row]);
    Mod inv = Mod(1) / a[row][col];
    for (int j = col; j < vars; ++j) a[row][j] *= inv;
    b[row] *= inv;
    for (int i = 0; i < (int)a.size(); ++i) if (i != row) {
      Mod z = a[i][col];
      for (int j = col; j < vars; ++j) a[i][j] -= z * a[row][j];
      b[i] -= z * b[row];
    }
    ++row;
  }
  for (int i = 0; i < (int)a.size(); ++i) {
    bool zero = true;
    for (int j = 0; j < vars; ++j) zero &= a[i][j] == Mod(0);
    if (zero && b[i] != Mod(0)) return false;
  }
  if (answer) {
    answer->assign(vars, Mod(0));
    for (int i = 0; i < (int)a.size(); ++i)
      for (int j = 0; j < vars; ++j)
        if (a[i][j] == Mod(1)) {
          bool unique = true;
          for (int k = 0; k < vars; ++k)
            if (k != j && a[i][k] != Mod(0)) unique = false;
          if (unique) (*answer)[j] = b[i];
        }
  }
  return true;
}

static int minimal_recurrence(const vector<Mod> &a, int maximum) {
  for (int d = 0; d <= maximum; ++d) {
    if (d == 0) {
      bool zero = all_of(a.begin(), a.end(), [](Mod x) { return x == Mod(0); });
      if (zero) return 0;
      continue;
    }
    vector<vector<Mod>> mat;
    vector<Mod> rhs;
    for (int i = d; i < (int)a.size(); ++i) {
      vector<Mod> row(d);
      for (int j = 0; j < d; ++j) row[j] = a[i - j - 1];
      mat.push_back(row); rhs.push_back(a[i]);
    }
    if (solve_mod(mat, rhs, d)) return d;
  }
  return maximum + 1;
}

static void check_bm(const vector<Mod> &seq, int maximum) {
  auto got = bm_test::BerlekampMassey(seq);
  int want = minimal_recurrence(seq, maximum);
  if ((int)got.size() != want) {
    cerr << "BM order mismatch: want=" << want << " got=" << got.size()
         << " length=" << seq.size() << '\n';
    exit(1);
  }
  for (int i = (int)got.size(); i < (int)seq.size(); ++i) {
    Mod rhs = 0;
    for (int j = 0; j < (int)got.size(); ++j) rhs += got[j] * seq[i - j - 1];
    if (rhs != seq[i]) {
      cerr << "BM recurrence mismatch at index " << i << '\n';
      exit(1);
    }
  }
}

static ll fac_oracle(ll n, ll p, ll pk) {
  ll ret = 1 % pk;
  for (ll i = 1; i <= n; ++i) {
    ll x = i;
    while (x % p == 0) x /= p;
    ret = (__int128)ret * (x % pk) % pk;
  }
  return ret;
}

int main() {
  check_gaussian({});
  for (int mask = 0; mask < 81; ++mask) {
    int z = mask;
    vector<vector<double>> a(2, vector<double>(3));
    for (auto &row : a) for (double &x : row) { x = z % 3 - 1; z /= 3; }
    for (int x0 = -2; x0 <= 2; ++x0) for (int x1 = -2; x1 <= 2; ++x1) {
      a[0][2] = a[0][0] * x0 + a[0][1] * x1;
      a[1][2] = a[1][0] * x0 + a[1][1] * x1;
      check_gaussian(a);
    }
  }
  mt19937 rng(0x6a551u);
  for (int tc = 0; tc < 300; ++tc) {
    int n = 1 + rng() % 5;
    vector<vector<double>> a(n, vector<double>(n + 1));
    vector<int> x(n);
    for (int &v : x) v = (int)(rng() % 9) - 4;
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) a[i][j] = (int)(rng() % 7) - 3;
      for (int j = 0; j < n; ++j) a[i][n] += a[i][j] * x[j];
    }
    check_gaussian(a);
  }

  check_bm({}, 0);
  check_bm(vector<Mod>(40, Mod(0)), 8);
  check_bm(vector<Mod>(40, Mod(7)), 8);
  for (int mask = 0; mask < 81; ++mask) {
    int z = mask;
    vector<Mod> c(4);
    for (auto &v : c) { v = z % 3 - 1; z /= 3; }
    vector<Mod> a(40);
    for (int i = 0; i < 4; ++i) a[i] = (i & 1) ? Mod(-2) : Mod(i + 1);
    for (int i = 4; i < (int)a.size(); ++i)
      for (int j = 0; j < 4; ++j) a[i] += c[j] * a[i - j - 1];
    check_bm(a, 4);
  }
  for (int tc = 0; tc < 220; ++tc) {
    int k = 1 + rng() % 8;
    vector<Mod> c(k), a(72);
    for (auto &v : c) v = (int)(rng() % 2001) - 1000;
    for (int i = 0; i < k; ++i) a[i] = (int)(rng() % 2001) - 1000;
    for (int i = k; i < (int)a.size(); ++i)
      for (int j = 0; j < k; ++j) a[i] += c[j] * a[i - j - 1];
    check_bm(a, k);
  }

  const int LIM = 2000000;
  vector<char> composite(LIM + 1);
  vector<int> pi(LIM + 1);
  for (int p = 2; 1LL * p * p <= LIM; ++p)
    if (!composite[p]) for (int q = p * p; q <= LIM; q += p) composite[q] = 1;
  for (int i = 1; i <= LIM; ++i)
    pi[i] = pi[i - 1] + (i >= 2 && !composite[i]);
  for (int n = 0; n <= 50000; ++n)
    if (pi_test::PrimeCount(n) != pi[n]) {
      cerr << "PrimeCount exhaustive mismatch n=" << n << '\n';
      return 1;
    }
  for (int tc = 0; tc < 240; ++tc) {
    int n = rng() % (LIM + 1);
    if (pi_test::PrimeCount(n) != pi[n]) {
      cerr << "PrimeCount random mismatch n=" << n << '\n';
      return 1;
    }
  }
  for (int q = 2; q <= 1200; ++q) for (int d : {-1, 0, 1}) {
    ll n = 1LL * q * q + d;
    if (n >= 0) {
      if (pi_test::PrimeCount(n) != pi[n]) {
        cerr << "PrimeCount square-boundary mismatch n=" << n << '\n';
        return 1;
      }
    }
  }
  for (auto [n, want] : vector<pair<ll, ll>>{
           {1000000000LL, 50847534LL},
           {1000000000000LL, 37607912018LL},
           {10000000000000LL, 346065536839LL}})
    if (pi_test::PrimeCount(n) != want) {
      cerr << "PrimeCount reference mismatch n=" << n << '\n';
      return 1;
    }

  long long fac_cases = 0;
  for (ll p : {2LL, 3LL, 5LL, 7LL, 11LL, 13LL, 17LL, 19LL, 31LL}) {
    ll pk = 1;
    for (int k = 1; k <= 5 && pk <= 1000000 / p; ++k) {
      pk *= p;
      set<ll> ns;
      for (ll n = 0; n <= 350; ++n) ns.insert(n);
      for (ll d = -2; d <= 2; ++d) if (pk + d >= 0) ns.insert(pk + d);
      for (ll d = -2; d <= 2; ++d) if (2 * pk + d >= 0) ns.insert(2 * pk + d);
      for (int tc = 0; tc < 40; ++tc) ns.insert(rng() % 5000);
      for (ll n : ns) {
        ll got = fac_test::fac_no_p(n, p, pk), want = fac_oracle(n, p, pk);
        if (got != want) {
          cerr << "fac_no_p mismatch p=" << p << " pk=" << pk << " n=" << n
               << " want=" << want << " got=" << got << '\n';
          return 1;
        }
        ++fac_cases;
      }
    }
  }
  cout << "math_boundaries: PASS gaussian/BM/PiCount/fac_no_p; fac cases="
       << fac_cases << '\n';
}
