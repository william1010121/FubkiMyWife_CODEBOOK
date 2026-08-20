#include <bits/stdc++.h>
using namespace std;
int n, m, deg[16], cnt[16];
vector<int> E[16], E1[16];
long long total;
#define main four_circle_template_main
#include "../../../codebook/2_Graph/FourCircleCount.cpp"
#undef main

static long long oracle(const vector<vector<char>> &a) {
  int n = (int)a.size();
  long long ordered = 0;
  for (int a0 = 0; a0 < n; ++a0) for (int b = 0; b < n; ++b)
    for (int c = 0; c < n; ++c) for (int d = 0; d < n; ++d)
      if (a0 != b && a0 != c && a0 != d && b != c && b != d && c != d &&
          a[a0][b] && a[b][c] && a[c][d] && a[d][a0]) ++ordered;
  return ordered / 8;
}

int main() {
  mt19937 rng(0x34435943);
  int cases = 0;
  for (n = 1; n <= 10; ++n) for (int tc = 0; tc < 400; ++tc) {
    vector<vector<char>> a(n, vector<char>(n));
    vector<pair<int,int>> edges;
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      if ((int)(rng() % 100) < 38) a[i][j] = a[j][i] = 1, edges.push_back({i + 1, j + 1});
    long long got = count_four_cycles(n, edges);
    long long want = oracle(a);
    ++cases;
    if (got != want) {
      cerr << "four_circle_count mismatch case=" << cases << " n=" << n
           << " m=" << m << " got=" << got << " want=" << want << '\n';
      return 1;
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-ordered-4-cycles/8\n";
}
