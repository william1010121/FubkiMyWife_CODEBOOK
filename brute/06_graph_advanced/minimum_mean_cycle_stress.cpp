#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
const int N = 600;
const ll INF = 1LL << 60;
#include "../../codebook/2_Graph/MinimumMeanCycle.cpp"

static MinimumMeanCycle solver;

static void clear_graph(int n) {
  for (int u = 0; u < n; ++u) fill_n(road[u], n, INF);
  solver.init(n);
}

static void check(const char *name, pll want) {
  pll got = solver.solve();
  if (got != want) {
    cerr << name << ": got " << got.first << '/' << got.second
         << ", expected " << want.first << '/' << want.second << '\n';
    exit(1);
  }
  // An n-edge walk ending at v has a k-edge suffix for every k <= n.
  for (int v = 0; v < solver.n; ++v)
    if (solver.dp[solver.n][v] < INF)
      for (int k = 0; k < solver.n; ++k)
        if (solver.dp[k][v] == INF) {
          cerr << name << ": missing reachable suffix\n";
          exit(1);
        }
}

int main() {
  auto start = chrono::steady_clock::now();
  clear_graph(N);
  ll sum = 0;
  for (int u = 0; u < N; ++u) {
    ll w = (u * 37) % 101 - 50;
    road[u][(u + 1) % N] = w;
    sum += w;
  }
  ll g = gcd(sum, (ll)N);
  check("600-vertex cycle", {sum / g, N / g});

  clear_graph(N);
  for (int u = 0; u + 1 < N; ++u) road[u][u + 1] = u % 19 - 9;
  check("600-vertex DAG", {-1, -1});
  road[N / 2][N / 2] = -17;
  check("cycle with one-way tails", {-17, 1});

  clear_graph(N);
  road[0][0] = 23;
  for (int u = 1; u + 1 < N; ++u) road[u][u + 1] = -100;
  check("cycle disconnected from DAG", {23, 1});

  // Potential differences telescope: every cycle has mean exactly -7.
  const int dense_n = 240;
  clear_graph(dense_n);
  for (int u = 0; u < dense_n; ++u)
    for (int v = 0; v < dense_n; ++v)
      road[u][v] = -7 + (ll)v * v - (ll)u * u;
  check("dense potential graph", {-7, 1});
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "minimum_mean_cycle stress PASS 5 graphs in "
       << seconds << "s\n";
}
