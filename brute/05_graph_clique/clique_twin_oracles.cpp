#include <bits/stdc++.h>
using namespace std;
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) int((v).size())
#define pb push_back
namespace maximum {
constexpr int N = 100;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}
namespace counting {
constexpr int N = 151;
#include "../../codebook/2_Graph/NumberofMaximalClique.cpp"
}
namespace covering {
constexpr int N = 20;
void fwt(int *a, int n, int) {
  for (int step = 1; step < n; step *= 2)
    for (int mask = 0; mask < n; ++mask)
      if (mask & step) a[mask] += a[mask ^ step];
}
#include "../../codebook/2_Graph/Minimum_Clique_Cover.cpp"
}

static void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}

// Enumerate only the small quotient graph, independently of all templates.
// Replacing a vertex by a clique weights maximum-clique size, preserves
// maximal-clique count and minimum cover. Independent twins weight the
// maximal-clique count by the product of multiplicities in each clique.
int main() {
  mt19937 rng(0x808C);
  maximum::MaxClique mc;
  static counting::BronKerbosch bk;
  static covering::Clique_Cover cc;
  int cases = 0;
  for (int tc = 0; tc < 80; ++tc) {
    int n = 1 + tc % 8, full = (1 << n) - 1;
    vector<vector<bool>> adj(n, vector<bool>(n));
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      adj[u][v] = adj[v][u] = rng() % 100 < (tc % 4) * 25;
    vector<bool> clique(1 << n, true);
    for (int mask = 0; mask <= full; ++mask)
      for (int u = 0; u < n; ++u) if (mask >> u & 1)
        for (int v = u + 1; v < n; ++v)
          if ((mask >> v & 1) && !adj[u][v]) clique[mask] = false;
    vector<int> cover(1 << n, n);
    cover[0] = 0;
    for (int mask = 1; mask <= full; ++mask)
      for (int sub = mask; sub; sub = (sub - 1) & mask)
        if (clique[sub])
          cover[mask] = min(cover[mask], cover[mask ^ sub] + 1);

    for (bool independent : {false, true}) {
      vector<int> weight(n), group;
      for (int i = 0; i < n; ++i) {
        weight[i] = 1 + rng() % (independent ? 3 : 11);
        for (int j = 0; j < weight[i]; ++j) group.push_back(i);
      }
      int used = group.size(), best = 0, count = 0;
      for (int mask = 1; mask <= full; ++mask) if (clique[mask]) {
        int size = 0, ways = 1;
        bool maximal = true;
        for (int i = 0; i < n; ++i) {
          if (mask >> i & 1)
            size += independent ? 1 : weight[i],
            ways *= independent ? weight[i] : 1;
          else if (clique[mask | (1 << i)]) maximal = false;
        }
        best = max(best, size);
        if (maximal) count += ways;
      }
      // Isolated padding plus relabelling crosses the 64-bit boundary.
      group.resize(100, -1);
      shuffle(group.begin(), group.end(), rng);
      auto edge = [&](int u, int v) {
        int a = group[u], b = group[v];
        return a >= 0 && b >= 0 &&
            (a == b ? !independent : bool(adj[a][b]));
      };
      mc.init(100), bk.init(100);
      for (int u = 0; u < 100; ++u)
        for (int v = u + 1; v < 100; ++v) if (edge(u, v))
          mc.add_edge(u, v), bk.add_edge(u + 1, v + 1);
      for (int repeat = 0; repeat < 2; ++repeat) {
        require(mc.solve() == best, "weighted maximum clique");
        set<int> seen;
        for (int i = 0; i < best; ++i) {
          int v = mc.sol[i];
          require(0 <= v && v < 100 && seen.insert(v).second,
                  "weighted clique solution vertex");
          for (int j = 0; j < i; ++j)
            require(edge(v, mc.sol[j]), "weighted clique solution edge");
        }
        require(bk.solve() == count + 100 - used,
                "weighted maximal clique count");
      }
      ++cases;
    }

    vector<int> group;
    for (int i = 0; i < n; ++i)
      for (int j = 0, k = 1 + rng() % 2; j < k; ++j)
        group.push_back(i);
    int used = group.size();
    group.resize(20, -1);
    shuffle(group.begin(), group.end(), rng);
    cc.init(20);
    for (int u = 0; u < 20; ++u) for (int v = u + 1; v < 20; ++v) {
      int a = group[u], b = group[v];
      if (a >= 0 && b >= 0 && (a == b || adj[a][b]))
        cc.add_edge(u, v), cc.add_edge(v, u);
    }
    require(cc.solve() == cover[full] + 20 - used,
            "clique-twin cover oracle");
    require(cc.solve() == cover[full] + 20 - used,
            "clique-twin repeated cover");
    ++cases;
  }
  cout << "PASS clique twin oracles cases=" << cases << '\n';
}
