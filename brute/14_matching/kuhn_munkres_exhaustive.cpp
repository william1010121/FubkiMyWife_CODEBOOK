#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 10
const ll INF = 1LL << 60;
#include "../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"

static const ll NEG = -(1LL << 62);

static ll oracle(const vector<vector<ll>> &w, const vector<vector<char>> &ok,
                 int row, int used) {
  int n = (int)w.size();
  if (row == n) return 0;
  ll ans = NEG;
  for (int col = 0; col < n; ++col) if (ok[row][col] && !(used >> col & 1))
    ans = max(ans, w[row][col] + oracle(w, ok, row + 1, used | (1 << col)));
  return ans;
}

static void check(const vector<vector<ll>> &w, const vector<vector<char>> &ok,
                  int tc, const char *label) {
  int n = (int)w.size();
  KM km;
  km.init(n);
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
    if (ok[i][j]) km.add_edge(i, j, w[i][j]);
  ll want = oracle(w, ok, 0, 0);
  if (want == NEG) return;  // The template's contract requires a perfect one.
  ll got = km.solve();
  if (got != want) {
    cerr << "KM mismatch case " << tc << " (" << label << ") n=" << n
         << " got=" << got << " want=" << want << '\n';
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j)
        cerr << (ok[i][j] ? to_string(w[i][j]) : string("X"))
             << (j + 1 == n ? '\n' : ' ');
    }
    exit(1);
  }
}

int main() {
  int tc = 0;
  const ll values[] = {-2, -1, 0, 1, 2};

  // Every complete 3 x 3 matrix over a tie-heavy signed alphabet.
  for (int n = 0; n <= 3; ++n) {
    int cells = n * n;
    long long total = 1;
    for (int i = 0; i < cells; ++i) total *= 5;
    for (long long code = 0; code < total; ++code) {
      long long x = code;
      vector<vector<ll>> w(n, vector<ll>(n));
      vector<vector<char>> ok(n, vector<char>(n, 1));
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        w[i][j] = values[x % 5];
        x /= 5;
      }
      check(w, ok, tc++, "complete-exhaustive");
    }
  }

  // Every small missing-edge pattern, with a diagonal forced so the contract
  // remains satisfied.  This exercises -INF initialisation and zero ties.
  for (int n = 1; n <= 3; ++n) {
    int cells = n * n;
    for (int mask = 0; mask < (1 << cells); ++mask) {
      vector<vector<ll>> w(n, vector<ll>(n));
      vector<vector<char>> ok(n, vector<char>(n));
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        ok[i][j] = (mask >> (i * n + j) & 1) || i == j;
        w[i][j] = values[(i * 3 + j * 2 + 1) % 5];
      }
      check(w, ok, tc++, "missing-edge-exhaustive");
    }
  }

  mt19937_64 rng(0x14C0FFEE);
  for (int z = 0; z < 2200; ++z) {
    int n = 4 + rng() % 5;
    vector<vector<ll>> w(n, vector<ll>(n));
    vector<vector<char>> ok(n, vector<char>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      ok[i][j] = (i == j) || rng() % 5 != 0;
      w[i][j] = (long long)(rng() % 2001) - 1000;
    }
    check(w, ok, tc++, "seeded-missing-random");
  }
  for (int z = 0; z < 80; ++z) {
    int n = 1 + z % 7;
    vector<vector<ll>> w(n, vector<ll>(n));
    vector<vector<char>> ok(n, vector<char>(n, 1));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
      w[i][j] = ((i + j + z) & 1) ? 1000000000000LL : -1000000000000LL;
    check(w, ok, tc++, "large-magnitude-ties");
  }
  cout << "PASS " << tc << " Kuhn-Munkres exhaustive/oracle cases\n";
}
