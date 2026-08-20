#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#define N 32
#define SZ(x) ((int)(x).size())
#define pb push_back
#define INF (1LL << 60)
#include "../../../codebook/4_Flow_Matching/MincostMaxflow.cpp"

struct Result { int flow; ll cost; };

static Result brute(const vector<vector<int>> &cost, const vector<vector<char>> &ok,
                    int u, int used) {
  if (u == (int)cost.size()) return {0, 0};
  Result ans = brute(cost, ok, u + 1, used); // leave this left vertex unused
  for (int v = 0; v < (int)cost.size(); ++v) if (ok[u][v] && !(used >> v & 1)) {
    Result nxt = brute(cost, ok, u + 1, used | (1 << v));
    Result cand{nxt.flow + 1, nxt.cost + cost[u][v]};
    if (cand.flow > ans.flow || (cand.flow == ans.flow && cand.cost < ans.cost))
      ans = cand;
  }
  return ans;
}

static void check(const vector<vector<int>> &cost, const vector<vector<char>> &ok,
                  int tc) {
  int n = (int)cost.size(), s = 0, left = 1, right = 1 + n, t = 1 + 2 * n;
  MinCostMaxFlow mf;
  mf.init(t + 1);
  for (int i = 0; i < n; ++i) {
    mf.add_edge(s, left + i, 1, 0);
    mf.add_edge(right + i, t, 1, 0);
  }
  for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j)
    if (ok[i][j]) mf.add_edge(left + i, right + j, 1, cost[i][j]);
  ll flow, got_cost;
  mf.solve(s, t, flow, got_cost, true);
  Result want = brute(cost, ok, 0, 0);
  if (flow != want.flow || got_cost != want.cost) {
    cerr << "MCMF mismatch on case " << tc << ": got=(" << flow << ','
         << got_cost << ") want=(" << want.flow << ',' << want.cost << ")\n";
    exit(1);
  }
}

int main() {
  int tc = 0;
  for (int n = 1; n <= 3; ++n) {
    int bits = n * n;
    for (int mask = 0; mask < (1 << bits); ++mask) {
      vector<vector<int>> cost(n, vector<int>(n));
      vector<vector<char>> ok(n, vector<char>(n));
      for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
        ok[i][j] = mask >> (i * n + j) & 1;
        cost[i][j] = (i * 7 + j * 11) % 13 - 6;
      }
      check(cost, ok, tc++);
    }
  }
  mt19937 rng(0xC057);
  for (int z = 0; z < 3000; ++z) {
    int n = 1 + rng() % 6;
    vector<vector<int>> cost(n, vector<int>(n));
    vector<vector<char>> ok(n, vector<char>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) {
      ok[i][j] = rng() % 2;
      cost[i][j] = (int)(rng() % 31) - 15;
    }
    check(cost, ok, tc++);
  }
  cout << "PASS " << tc << " exhaustive/random unit transport networks\n";
}
