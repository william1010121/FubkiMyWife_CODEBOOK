#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
using i128 = __int128;
const int N = 12;
const ll INF = LLONG_MAX;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"

[[noreturn]] static void fail(int tc, const string &why) {
  cerr << "minimum_mean_cycle extremes tc=" << tc << ": " << why << '\n';
  exit(1);
}

static pll oracle(const vector<vector<ll>> &w) {
  int n = w.size();
  pll best = {-1, -1};
  vector<bool> used(n);
  function<void(int, int, int, i128)> visit =
      [&](int start, int u, int length, i128 sum) {
    for (int v = 0; v < n; ++v) if (w[u][v] != INF) {
      i128 next = sum + w[u][v];
      if (v == start) {
        ll den = length + 1;
        if (best.second < 0 || next * best.second < i128(best.first) * den)
          best = {ll(next), den};
      } else if (!used[v]) {
        used[v] = true;
        visit(start, v, length + 1, next);
        used[v] = false;
      }
    }
  };
  for (int start = 0; start < n; ++start) {
    fill(used.begin(), used.end(), false);
    used[start] = true;
    visit(start, start, 0, 0);
  }
  if (best.second < 0) return best;
  ll g = gcd(best.first, best.second);
  return {best.first / g, best.second / g};
}

static void check(int tc, const vector<vector<ll>> &w) {
  int n = w.size();
  // Independently enforce the documented arithmetic domain in 128 bits.
  const i128 unreachable = i128(1) << 120;
  vector<vector<i128>> lo(n + 1, vector<i128>(n, unreachable));
  vector<vector<i128>> hi(n + 1, vector<i128>(n, -unreachable));
  fill(lo[0].begin(), lo[0].end(), 0);
  fill(hi[0].begin(), hi[0].end(), 0);
  for (int k = 1; k <= n; ++k) {
    for (int u = 0; u < n; ++u) if (lo[k - 1][u] != unreachable)
      for (int v = 0; v < n; ++v) if (w[u][v] != INF) {
        lo[k][v] = min(lo[k][v], lo[k - 1][u] + w[u][v]);
        hi[k][v] = max(hi[k][v], hi[k - 1][u] + w[u][v]);
      }
    for (int v = 0; v < n; ++v) if (lo[k][v] != unreachable)
      if (lo[k][v] <= -i128(INF) || hi[k][v] >= INF)
        fail(tc, "test violates walk-sum precondition");
  }
  for (int v = 0; v < n; ++v) if (lo[n][v] != unreachable)
    for (int k = 0; k < n; ++k) {
      if (lo[k][v] == unreachable) fail(tc, "missing suffix in oracle");
      i128 difference = lo[n][v] - lo[k][v];
      if (difference < LLONG_MIN || difference > LLONG_MAX)
        fail(tc, "test violates DP-difference precondition");
    }
  for (int u = 0; u < n; ++u)
    for (int v = 0; v < n; ++v) road[u][v] = w[u][v];
  MinimumMeanCycle solver;
  solver.init(n);
  pll got = solver.solve(), want = oracle(w);
  if (got != want) {
    cerr << "got=" << got.first << '/' << got.second << " want="
         << want.first << '/' << want.second << '\n';
    fail(tc, "cycle fraction differs");
  }
  for (int k = 0; k <= n; ++k) for (int v = 0; v < n; ++v) {
    ll want_dp = lo[k][v] == unreachable ? INF : ll(lo[k][v]);
    if (solver.dp[k][v] != want_dp) fail(tc, "public DP table differs");
  }
  if (solver.solve() != want) fail(tc, "repeated solve differs");
}

int main() {
  int tc = 0;
  check(++tc, {});
  for (ll w : {INF, INF - 1, -INF + 1, 0LL, 1LL, -1LL})
    check(++tc, {{w}});
  ll h = INF / 2 - 1;
  check(++tc, {{INF, h}, {-h + 1, INF}});  // mean 1/2
  check(++tc, {{INF, -h}, {h - 1, INF}});  // mean -1/2
  check(++tc, {{INF, h}, {h, INF}});
  check(++tc, {{INF, -h}, {-h, INF}});
  vector<vector<ll>> dag(N, vector<ll>(N, INF));
  for (int u = 0; u + 1 < N; ++u) dag[u][u + 1] = INF / N - 1;
  check(++tc, dag);

  // The comparison helper must not overflow even for extreme numerators.
  vector<pll> fractions = {{LLONG_MIN, 1}, {LLONG_MAX, 1},
      {LLONG_MIN, INT_MAX}, {LLONG_MAX, INT_MAX}, {0, 1}, {-1, 2}, {1, 2}};
  for (pll a : fractions) for (pll b : fractions)
    if (MinimumMeanCycle::less_frac(a, b) !=
        (i128(a.first) * b.second < i128(b.first) * a.second))
      fail(tc, "extreme comparison differs");

  mt19937_64 rng(0x5AA551);
  for (int round = 0; round < 2400; ++round) {
    int n = 1 + rng() % 6;
    vector<vector<ll>> w(n, vector<ll>(n, INF));
    if (round & 1) {
      // Potential differences telescope, so large edges still have bounded
      // walk sums. Every cycle has exactly the same small mean.
      vector<ll> potential(n);
      ll span = INF - 1024;
      for (ll &p : potential) p = rng() % span;
      if (n > 1) potential[0] = 0, potential[1] = span;
      ll mean = ll(rng() % 31) - 15;
      for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        if (rng() % 100 < 55)
          w[u][v] = potential[v] - potential[u] + mean;
    } else {
      ll bound = INF / (2 * n) - 1;
      for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
        if (rng() % 100 < 55) {
          ll magnitude = rng() % (bound + 1);
          w[u][v] = (rng() & 1) ? magnitude : -magnitude;
        }
    }
    check(++tc, w);
  }
  cout << "minimum_mean_cycle extremes PASS cases=" << tc
       << " INF=LLONG_MAX, exact cycle/DP oracles\n";
}
