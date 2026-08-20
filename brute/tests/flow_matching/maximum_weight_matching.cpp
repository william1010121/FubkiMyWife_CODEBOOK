#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define INF 0x3f3f3f3f
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define pb push_back

#include "codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"

static int n;
static int weight_[9][9];
static long long oracle(int mask, int &best_count) {
  int u = 0;
  while (u < n && (mask >> u & 1)) ++u;
  if (u == n) return 0;
  long long ans = oracle(mask | (1 << u), best_count);
  int ans_count = best_count;
  for (int v = u + 1; v < n; ++v) if (!(mask >> v & 1)) {
    int child_count = 0;
    long long got = weight_[u][v] + oracle(mask | (1 << u) | (1 << v), child_count);
    if (got > ans) ans = got, ans_count = child_count + 1;
  }
  best_count = ans_count;
  return ans;
}

int main() {
  mt19937 rng(0x51f0a7);
  for (int tc = 0; tc < 2500; ++tc) {
    n = 1 + (rng() % 8);
    WeightGraph graph(n);
    memset(weight_, 0, sizeof(weight_));
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j) {
      weight_[i][j] = weight_[j][i] = (rng() % 5 == 0 ? 0 : 1 + rng() % 30);
      graph.add_edge(i + 1, j + 1, weight_[i][j]);
    }
    int ignored = 0;
    long long want = oracle(0, ignored);
    auto got = graph.solve();
    if (got.first != want) {
      cerr << "maximum-weight matching mismatch tc=" << tc
           << " got=" << got.first << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "maximum_weight_matching: PASS (2500 exhaustive-oracle random cases)\n";
}
