#include <bits/stdc++.h>
using namespace std;
#define N 128
#define pb push_back
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"

int main() {
  mt19937 rng(5);
  for (int tc = 0; tc < 500; ++tc) {
    int n = 2 + rng() % 80;
    Heavy_light_Decomposition h;
    h.init(n);
    vector<vector<int>> g(n + 1);
    for (int v = 2; v <= n; ++v) {
      int p = 1 + rng() % (v - 1);
      h.add_edge(p, v), g[p].push_back(v), g[v].push_back(p);
      h.val[v] = (int)(rng() % 21) - 10;
    }
    h.val[1] = (int)(rng() % 21) - 10;
    h.build();
    for (int q = 0; q < 300; ++q) {
      int a = 1 + rng() % n, b = 1 + rng() % n;
      vector<int> par(n + 1), st = {a};
      par[a] = -1;
      for (int z = 0; z < (int)st.size(); ++z) for (int v : g[st[z]])
        if (v != par[st[z]]) par[v] = st[z], st.push_back(v);
      int want = 0;
      for (int x = b; x != -1; x = par[x]) {
        want += h.val[x];
        if (x == a) break;
      }
      assert(h.query(a, b) == want);
    }
  }
}
