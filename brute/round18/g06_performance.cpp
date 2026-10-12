#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
const int N = 2000;
const ll INF = 1LL << 60;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"
static MinimumMeanCycle s;
int main() {
  s.init(N);
  for (int u = 0; u < N; ++u) fill_n(road[u], N, INF);
  assert(s.solve() == pll(-1, -1));
  for (int k = 1; k <= N; ++k)
    for (int v = 0; v < N; ++v) assert(s.dp[k][v] == INF);
  for (int u = 0; u < N; ++u) {
    int v = (u + 1) % N;
    road[u][v] = -7 + ll(v) * v - ll(u) * u;
  }
  auto start = chrono::steady_clock::now();
  assert(s.solve() == pll(-7, 1));
  for (int k = 0; k <= N; ++k) for (int v = 0; v < N; ++v) {
    int u = (v - k + N) % N;
    assert(s.dp[k][v] == -7LL * k + ll(v) * v - ll(u) * u);
  }
  mt19937 rng(18);
  for (int u = 0; u < N; ++u) for (int v = 0; v < N; ++v)
    if (rng() % 500 == 0)
      road[u][v] = -7 + ll(v) * v - ll(u) * u;
  assert(s.solve() == pll(-7, 1));
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "MMC 2000-node empty/cycle/sparse PASS " << seconds << "s\n";
}
