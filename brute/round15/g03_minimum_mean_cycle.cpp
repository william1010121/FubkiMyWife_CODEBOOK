#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
const int N = 12;
const ll INF = 1LL << 60;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"

int main() {
  mt19937 rng(1503);
  MinimumMeanCycle solver;
  for (int trial = 0; trial < 1000; ++trial) {
    int n = trial % (N + 1);
    solver.init(n);
    for (int u = 0; u < n; ++u)
      for (int v = 0; v < n; ++v)
        road[u][v] = rng() % 3 ? INF : (ll)(rng() % 201) - 100;
    auto first = solver.solve();
    vector<vector<ll>> dp(n + 1, vector<ll>(n, INF));
    fill(dp[0].begin(), dp[0].end(), 0);
    // Independent matrix recurrence checks every public DP cell.
    for (int k = 1; k <= n; ++k)
      for (int v = 0; v < n; ++v)
        for (int u = 0; u < n; ++u)
          if (dp[k - 1][u] < INF && road[u][v] < INF)
            dp[k][v] = min(dp[k][v], dp[k - 1][u] + road[u][v]);
    for (int k = 0; k <= n; ++k)
      for (int v = 0; v < n; ++v) assert(solver.dp[k][v] == dp[k][v]);
    assert(solver.solve() == first);
  }
  cout << "round15 MMC: all DP cells and repeat solve PASS\n";
}
