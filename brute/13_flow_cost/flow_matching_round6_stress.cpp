#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)(a).size())
#define pb push_back
namespace costflow {
const int N = 200000;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/MincostMaxflow.cpp"
static MinCostMaxFlow mf;
}
namespace circulation {
const int N = 4000;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/MinCostCirculation.cpp"
}
namespace globalcut {
#include "codebook/4_Flow_Matching/SW-mincut.cpp"
}
static void require(bool ok) { if (!ok) abort(); }
int main() {
  auto start = chrono::steady_clock::now();
  auto &mf = costflow::mf;
  const int n = costflow::N;
  mf.init(n);
  for (int u = 0; u + 1 < n; ++u) mf.add_edge(u, u + 1, 7, -2);
  for (int query = 0; query < 3; ++query) {
    int s = query == 1 ? n - 1 : 0, t = n - 1 - s;
    ll flow, cost;
    mf.solve(s, t, flow, cost, query == 0);
    require(flow == 7 && cost == 14LL * (n - 1) * (query == 1 ? 1 : -1));
    for (int u = 0; u < n; ++u) for (int i = 0; i < SZ(mf.G[u]); ++i) {
      auto &e = mf.G[u][i], &rev = mf.G[e.to][e.rev];
      require(e.from == u && rev.to == u && rev.rev == i);
      require(e.flow == -rev.flow);
    }
  }
  // The working residual pair has indices well above 65535.
  mf.init(2);
  for (int k = 0; k < n; ++k) mf.add_edge(0, 1, 0, 0);
  mf.add_edge(0, 1, 7, -2);
  ll flow, cost;
  mf.solve(0, 1, flow, cost);
  require(flow == 7 && cost == -14 && mf.G[0].back().rev == n);
  mf.solve(1, 0, flow, cost, false);
  require(flow == 7 && cost == 14 && mf.G[0].back().flow == 0);

  auto &cc = circulation::mcmf;
  cc.init(circulation::N);
  for (int u = 0; u < circulation::N; ++u)
    cc.add_edge(u, (u + 1) % circulation::N, 1023, -1);
  cc.solve(9);
  for (int u = 0; u < circulation::N; ++u)
    for (auto &e : cc.G[u]) if (e.fcap) {
      auto &rev = cc.G[e.to][e.rev];
      require(e.cap == 1023 && e.flow == 1023 && rev.flow == -1023);
    }
  cc.init(1);
  for (int k = 0; k < n; ++k) cc.add_edge(0, 0, 0, 0);
  ll cap = (1LL << 40) + 7;
  cc.add_edge(0, 0, cap, -1), cc.solve(40);
  auto &e = cc.G[0][2 * n], &rev = cc.G[0][e.rev];
  require(e.cap == cap && e.flow == cap && e.rev == 2 * n + 1);
  require(rev.rev == 2 * n && rev.flow == -cap);

  int size = globalcut::SW::MXN;
  for (bool isolated : {false, true}) {
    auto &sw = globalcut::sw; sw.init(size);
    int connected = size - isolated;
    for (int u = 0; u < connected; ++u)
      for (int v = u + 1; v < connected; ++v) sw.addEdge(u, v, 1);
    require(sw.solve(size) == (isolated ? 0 : size - 1));
  }
  cout << "PASS round6 stress: 200000-node MCMF, wide reverse indices, "
       << "4000-node circulation, 41-bit loop, 514-node SW; "
       << chrono::duration<double>(
              chrono::steady_clock::now() - start).count() << " s\n";
}
