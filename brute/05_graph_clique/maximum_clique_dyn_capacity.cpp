#include <bits/stdc++.h>
using namespace std;
#define ALL(v) (v).begin(), (v).end()
#define SZ(v) ((int)(v).size())
#define pb push_back
namespace tiny {
constexpr int N = 1;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}
namespace large {
constexpr int N = 100;
#include "../../codebook/2_Graph/Maximum_Clique_Dyn.cpp"
}

template<class Clique> void check(int n, int group) {
  Clique mc;
  mc.init(n);
  for (int u = 0; u < n; ++u)
    for (int v = u + 1; v < n; ++v)
      if (u / group == v / group) mc.add_edge(u, v);
  int want = min(n, group), got = mc.solve();
  if (got != want) throw runtime_error("capacity clique size");
  vector<int> seen(n);
  for (int i = 0; i < got; ++i) {
    int v = mc.sol[i];
    if (v < 0 || v >= n || seen[v]++)
      throw runtime_error("capacity clique vertex");
    for (int j = 0; j < i; ++j)
      if (v / group != mc.sol[j] / group)
        throw runtime_error("capacity solution is not a clique");
  }
  if (mc.solve() != want) throw runtime_error("repeat solve");
}

static void reuse_and_relabel() {
  large::MaxClique mc;
  mt19937 rng(5001);
  for (int n : {100, 1, 99, 0, 64, 2, 100}) {
    for (int groups : {1, 2, 7, 23, 100}) {
      mc.init(n);
      vector<int> label(n);
      iota(label.begin(), label.end(), 0);
      shuffle(label.begin(), label.end(), rng);
      for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
        if (label[u] % groups != label[v] % groups) {
          mc.add_edge(u, v), mc.add_edge(v, u); // duplicate edge
        }
      int want = min(n, groups);
      for (int repeat = 0; repeat < 2; ++repeat) {
        if (mc.solve() != want) throw runtime_error("reused multipartite");
        set<int> found;
        for (int i = 0; i < want; ++i) {
          int v = mc.sol[i];
          if (v < 0 || v >= n || !found.insert(v).second)
            throw runtime_error("reused solution vertex");
          for (int j = 0; j < i; ++j)
            if (!mc.G[v][mc.sol[j]]) throw runtime_error("reused solution");
        }
      }
    }
  }
}
int main() {
  reuse_and_relabel();
  check<tiny::MaxClique>(0, 1);
  check<tiny::MaxClique>(1, 1);
  check<large::MaxClique>(100, 100);
  check<large::MaxClique>(100, 1);
  check<large::MaxClique>(100, 7);
  cout << "PASS clique capacity N=1/100 and solutions\n";
}
