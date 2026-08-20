#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define pb push_back
#define N 128
#include "../../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"

int main() {
  mt19937 rng(6);
  for (int tc = 0; tc < 300; ++tc) {
    int n = 2 + rng() % 45;
    Cent_Dec cd;
    cd.init(n);
    vector<vector<pair<int, int>>> g(n + 1);
    for (int v = 2; v <= n; ++v) {
      int p = 1 + rng() % (v - 1), w = 1 + rng() % 20;
      cd.add_edge(p, v, w), g[p].push_back({v, w}), g[v].push_back({p, w});
    }
    cd.build();
    vector<int> marked;
    for (int q = 0; q < 100; ++q) {
      int u = 1 + rng() % n;
      cd.modify(u), marked.push_back(u);
      for (int s = 1; s <= n; ++s) {
        vector<ll> d(n + 1, -1);
        d[s] = 0;
        vector<int> st = {s};
        for (int z = 0; z < (int)st.size(); ++z) for (auto [v, w] : g[st[z]])
          if (d[v] == -1) d[v] = d[st[z]] + w, st.push_back(v);
        ll want = 0;
        for (int x : marked) want += d[x];
        if (cd.query(s) != want) {
          cerr << "centroid mismatch tc=" << tc << " q=" << q
               << " s=" << s << " got=" << cd.query(s) << " want=" << want << '\n';
          return 1;
        }
      }
    }
  }
}
