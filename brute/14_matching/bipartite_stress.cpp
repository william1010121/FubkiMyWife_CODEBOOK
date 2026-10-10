#include <bits/stdc++.h>
using namespace std;
#define N 60000
#define SZ(a) ((int)(a).size())
#define pb push_back
#include "codebook/4_Flow_Matching/Bipartite_Matching.cpp"

static Bipartite_Matching bm;

int main() {
  // One length-1 augmenting path and one length-3 alternating path.
  // A shortest-path phase must leave the latter for the next BFS.
  bm.init(4, 4);
  bm.add_edge(0, 0), bm.add_edge(1, 1);
  bm.add_edge(2, 2), bm.add_edge(3, 3);
  bm.add_edge(2, 1), bm.add_edge(3, 2);
  bm.mp[0] = bm.mp[1] = -1, bm.mp[2] = 1, bm.mp[3] = 2;
  bm.mq[0] = bm.mq[3] = 4, bm.mq[1] = 2, bm.mq[2] = 3;
  fill_n(bm.cur, 4, 0);
  if (!bm.bfs() || bm.dis[4] != 1 || bm.dis[3] != -1) return 4;
  if (!bm.dfs(0) || bm.dfs(1)) return 5;
  fill_n(bm.cur, 4, 0);
  if (!bm.bfs() || bm.dis[4] != 3 || !bm.dfs(1)) return 6;
  auto start = chrono::steady_clock::now();
  // Disjoint chains of differing lengths need alternating paths/phases.
  // A chain's first pass shifts matches right, leaving its last node free.
  for (int round = 0; round < 5; ++round) {
    bm.init(N, N);
    for (int base = 0; base < N;) {
      int len = min(N - base, 2 + (base * 17 + round * 31) % 199);
      for (int i = base; i < base + len; ++i) {
        if (i + 1 < base + len) bm.add_edge(i, i + 1);
        bm.add_edge(i, i);
      }
      base += len;
    }
    if (bm.matching() != N || bm.matching() != N) return 1;
    vector<int> seen(N);
    for (int u = 0; u < N; ++u) {
      int v = bm.mp[u];
      if (v < 0 || v >= N || seen[v]++ || bm.mq[v] != u) return 2;
      if (find(bm.G[u].begin(), bm.G[u].end(), v) == bm.G[u].end())
        return 3;
    }
  }
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "PASS bipartite stress: 10 solves, 60000+60000 vertices, "
       << "~120000 edges, " << seconds << " s\n";
}
