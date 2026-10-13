#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
#define X first
#define Y second
#define pb push_back
const int N = 200005;
#include "../../codebook/3_Data_Structure/Centroid_Decomposition.cpp"
static Cent_Dec cd;
int main() {
  const int n = 200000;
  for (string shape : {"random", "star", "balanced"}) {
    mt19937 rng(903); cd.init(n);
    vector<int> p(n + 1), w(n + 1), size(n + 1, 1);
    vector<ll> depth(n + 1), expected(n + 1);
    for (int v = 2; v <= n; ++v) {
      p[v] = shape == "star" ? 1 : shape == "balanced" ? v / 2
                                                   : 1 + rng() % (v - 1);
      w[v] = v % 2 ? INT_MIN + int(rng() % 100)
                   : INT_MAX - int(rng() % 100);
      cd.add_edge(p[v], v, w[v]);
      depth[v] = depth[p[v]] + w[v];
      expected[1] += depth[v];
    }
    for (int v = n; v > 1; --v) size[p[v]] += size[v];
    for (int v = 2; v <= n; ++v)
      expected[v] = expected[p[v]] + ll(n - 2 * size[v]) * w[v];
    cd.build();
    for (int u = 1; u <= n; ++u) {
      assert(cd.query(u) == 0 && cd.done[u] == 0);
      assert(cd.layer[u] <= __lg(n));
    }
    for (int u = 1; u <= n; ++u) cd.modify(u);
    for (int u = 1; u <= n; ++u)
      assert(cd.query(u) == expected[u]);
    for (int copies = 1; copies <= n; ++copies) {
      cd.modify(1);
      int u = 1 + rng() % n;
      assert(cd.query(u) == expected[u] + ll(copies) * depth[u]);
    }
  }
  cout << "centroid scale: PASS\n";
}
