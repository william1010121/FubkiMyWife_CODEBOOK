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
int main() {
  check<tiny::MaxClique>(0, 1);
  check<tiny::MaxClique>(1, 1);
  check<large::MaxClique>(100, 100);
  check<large::MaxClique>(100, 1);
  check<large::MaxClique>(100, 7);
  cout << "PASS clique capacity N=1/100 and solutions\n";
}
