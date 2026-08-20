#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 8
const ll INF = (1LL << 60);
#include "../../../codebook/4_Flow_Matching/Kuhn_Munkres.cpp"

static ll brute(const vector<vector<ll>> &w, int row, int mask) {
  if (row == (int)w.size()) return 0;
  ll ans = -(1LL << 60);
  for (int col = 0; col < (int)w.size(); ++col)
    if (!(mask >> col & 1))
      ans = max(ans, w[row][col] + brute(w, row + 1, mask | (1 << col)));
  return ans;
}

int main() {
  mt19937 rng(0x4B4D);
  int tc = 0;
  for (int n = 1; n <= 8; ++n) {
    int rounds = (n == 8 ? 3 : 30);
    for (int z = 0; z < rounds; ++z) {
      vector<vector<ll>> w(n, vector<ll>(n));
      KM km;
      km.init(n);
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        w[i][j] = (int)(rng() % 81) - 40;
        km.add_edge(i, j, w[i][j]);
      }
      ll got = km.solve(), want = brute(w, 0, 0);
      if (got != want) {
        cerr << "KM mismatch on case " << tc << ": got=" << got
             << " want=" << want << '\n';
        return 1;
      }
      ++tc;
    }
  }
  cout << "PASS " << tc << " complete square assignments\n";
}
