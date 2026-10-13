#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define N 60000
#define INF (1LL << 60)
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"

static MinCostMaxFlow mf;

int main() {
  auto start = chrono::steady_clock::now();
  for (int round = 0; round < 5; ++round) {
    mf.init(N);
    for (int i = 0; i + 1 < N; ++i) mf.add_edge(i, i + 1, 7, -2);
    // Negative path: BF must supply its predecessor and bottleneck.
    // Reverse it using nonnegative residual costs, then send it again
    // with neg=false to exercise automatic BF detection on reused flow.
    for (int query = 0; query < 3; ++query) {
      int s = query == 1 ? N - 1 : 0, t = N - 1 - s;
      ll flow, cost, want = 14LL * (N - 1) * (query == 1 ? 1 : -1);
      mf.solve(s, t, flow, cost, query == 0);
      if (flow != 7 || cost != want) return 1;
      for (int u = 0; u + 1 < N; ++u) {
        const auto &e = mf.G[u].back();
        if (e.to != u + 1 || e.flow != (query == 1 ? 0 : 7))
          return 2;
        if (mf.G[e.to][e.rev].flow != -e.flow) return 3;
      }
    }
  }
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "PASS MincostMaxflow stress: 15 solves, 60000 vertices, "
       << "59999 edges, " << seconds << " s\n";
}
