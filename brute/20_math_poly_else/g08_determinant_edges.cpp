#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 256;
ll P;
#include "../../codebook/6_Math/Determinant.cpp"

ll permutation_det(const vector<vector<ll>> &a) {
  int n = a.size();
  vector<int> perm(n); iota(perm.begin(), perm.end(), 0);
  ll ans = 0;
  do {
    ll term = 1 % P; int parity = 0;
    for (int i = 0; i < n; ++i) {
      term = (__int128)term * a[i][perm[i]] % P;
      for (int j = 0; j < i; ++j) parity ^= perm[j] > perm[i];
    }
    ans = ((__int128)ans + (parity ? P - term : term)) % P;
  } while (next_permutation(perm.begin(), perm.end()));
  return ans;
}

void check(const vector<vector<ll>> &a) {
  Matrix mat; mat.n = mat.m = a.size();
  for (int i = 0; i < mat.n; ++i)
    copy(a[i].begin(), a[i].end(), mat.M[i]);
  ll want = permutation_det(a), got = mat.det();
  if (got != want) {
    cerr << "det mismatch n=" << a.size() << " P=" << P
         << " want=" << want << " got=" << got << '\n';
    exit(1);
  }
}

int main() {
  mt19937_64 rng(0xD372);
  for (ll mod : {1LL, 2LL, 4LL, 6LL, 12LL, 25LL, 101LL,
                 1000000007LL, LLONG_MAX - 1, LLONG_MAX}) {
    P = mod;
    check({}); check({{0}}); check({{P - 1}});
    check({{0, 1 % P}, {1 % P, 0}});
    check({{0, P - 1, 0}, {0, 0, P - 1}, {P - 1, 0, 0}});
    check({{0, 1 % P, 2 % P}, {0, 3 % P, 4 % P},
           {0, 5 % P, 6 % P}});
    check({{2 % P, 4 % P}, {4 % P, 8 % P}});
    for (int tc = 0; tc < 180; ++tc) {
      int n = 1 + rng() % 6;
      vector<vector<ll>> a(n, vector<ll>(n));
      for (auto &row : a) for (ll &x : row) x = rng() % P;
      if (tc % 3 == 0) a[0][0] = 0;
      if (tc % 7 == 0 && n > 1) a.back() = a.front();
      check(a);
    }
  }
  // Exhaust all 2x2 matrices over small composite rings.
  for (P = 2; P <= 6; P += 2)
    for (int mask = 0; mask < P * P * P * P; ++mask) {
      int z = mask;
      vector<vector<ll>> a(2, vector<ll>(2));
      for (auto &row : a) for (ll &x : row) x = z % P, z /= P;
      check(a);
    }
  // Consecutive Fibonacci pivots force a long Euclidean swap chain.
  P = LLONG_MAX;
  ll a = 1, b = 1;
  while (a <= LLONG_MAX - b) { ll t = a + b; a = b; b = t; }
  check({{b, P - 1}, {a, P - 2}});
  check({{P - 1, P - 1}, {1, P - 1}}); // quotient near P
  // row_swap is also a public helper.
  Matrix rows{}; rows.n = rows.m = 2;
  rows.M[0][0] = 1; rows.M[1][1] = P - 1;
  if (rows.row_swap(0, 0) != 0 || rows.row_swap(0, 1) != 1 ||
      rows.M[1][0] != 1 || rows.M[0][1] != P - 1) return 1;
  // Full-size permutation matrix: repeated zero-pivot row swaps.
  P = LLONG_MAX; Matrix mat{}; mat.n = mat.m = MAXN;
  for (int i = 0; i < MAXN; ++i) mat.M[i][MAXN - 1 - i] = 1;
  ll want = (MAXN * (MAXN - 1) / 2 & 1) ? P - 1 : 1;
  if (mat.det() != want) return 1;
  // A singular first column permits immediate termination.
  fill_n(mat.M[0], MAXN, 0);
  if (mat.det() != 0) return 1;
  cout << "g08_determinant_edges: PASS composite/LLONG_MAX moduli, "
          "zero pivots, singular/empty matrices, full-size stress\n";
}
