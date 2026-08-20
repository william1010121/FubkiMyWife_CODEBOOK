#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pll = pair<ll, ll>;

const int N = 8;
const ll INF = (1LL << 60);
#include "../../../codebook/2_Graph/MinimumMeanCycle.cpp"

static pll oracle(const vector<vector<ll>> &w) {
  int n = (int)w.size();
  bool found = false;
  ll best_num = 0, best_den = 1;
  vector<int> path;
  vector<char> used(n);
  function<void(int, int, ll)> dfs = [&](int s, int u, ll sum) {
    for (int v = 0; v < n; ++v) {
      if (w[u][v] == INF) continue;
      if (v == s && path.size() >= 2) {
        ll den = (ll)path.size();
        ll cycle_sum = sum + w[u][v];
        if (!found || (__int128)cycle_sum * best_den < (__int128)best_num * den)
          found = true, best_num = cycle_sum, best_den = den;
      } else if (!used[v] && v != s) {
        used[v] = 1, path.push_back(v);
        dfs(s, v, sum + w[u][v]);
        path.pop_back(), used[v] = 0;
      }
    }
  };
  for (int s = 0; s < n; ++s) {
    fill(used.begin(), used.end(), 0);
    used[s] = 1, path = {s};
    dfs(s, s, 0);
  }
  if (!found) return {-1, -1};
  ll g = gcd(llabs(best_num), best_den);
  return {best_num / g, best_den / g};
}

int main() {
  mt19937 rng(0x4D4D43);
  int cases = 0;
  for (int n = 2; n <= 7; ++n) {
    for (int tc = 0; tc < 700; ++tc) {
      vector<vector<ll>> w(n, vector<ll>(n, INF));
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
          if (i != j && (int)(rng() % 100) < 42)
            w[i][j] = (int)(rng() % 17) - 8;
      for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) road[i][j] = w[i][j];
      MinimumMeanCycle mmc;
      mmc.init(n);
      pll got = mmc.solve(), want = oracle(w);
      ++cases;
      if (got != want) {
        cerr << "minimum_mean_cycle mismatch case=" << cases << " n=" << n
             << " got=" << got.first << '/' << got.second
             << " want=" << want.first << '/' << want.second << '\n';
        return 1;
      }
    }
  }
  cout << "PASS cases=" << cases << " oracle=enumerated-simple-directed-cycles\n";
}
