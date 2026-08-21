#include <bits/stdc++.h>
using namespace std;

#define N 32
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "../../codebook/4_Flow_Matching/Bipartite_Matching.cpp"

static int oracle(const vector<vector<int>> &g, int u, int used,
                  vector<int> &memo, int states) {
  if (u == (int)g.size()) return 0;
  int &ans = memo[u * states + used];
  if (ans != -1) return ans;
  ans = oracle(g, u + 1, used, memo, states);
  for (int v : g[u]) if (!(used >> v & 1))
    ans = max(ans, 1 + oracle(g, u + 1, used | (1 << v), memo, states));
  return ans;
}

static void check(Bipartite_Matching &bm, int l, int r,
                  const vector<pair<int, int>> &edges, int tc,
                  const string &label) {
  vector<vector<int>> g(l);
  bm.init(l, r);
  for (auto [u, v] : edges) {
    g[u].push_back(v);
    bm.add_edge(u, v);
  }
  int states = 1 << r;
  vector<int> memo((l + 1) * states, -1);
  int want = oracle(g, 0, 0, memo, states);
  int got = bm.matching();
  if (got != want) {
    cerr << "Bipartite mismatch case " << tc << " (" << label << ")"
         << ": l=" << l << " r=" << r << " got=" << got
         << " want=" << want << '\n';
    for (auto [u, v] : edges) cerr << '(' << u << ',' << v << ") ";
    cerr << '\n';
    exit(1);
  }
}

int main() {
  Bipartite_Matching bm;
  int tc = 0;

  // Exhaust all small rectangular graphs in both orientations.  The
  // existing suite covers 4 x 4; these cases emphasize unequal sides and
  // empty sides without making one test needlessly expensive.
  for (int l = 0; l <= 5; ++l) for (int r = 0; r <= 5; ++r) {
    if (l * r > 15) continue;
    int bits = l * r;
    for (int mask = 0; mask < (1 << bits); ++mask) {
      vector<pair<int, int>> edges;
      for (int u = 0; u < l; ++u) for (int v = 0; v < r; ++v)
        if (mask >> (u * r + v) & 1) edges.push_back({u, v});
      check(bm, l, r, edges, tc++, "exhaustive");
    }
  }

  // Reinitialisation, duplicate edges, adversarial edge order, and isolated
  // vertices are common ways to expose stale-array or sentinel mistakes.
  vector<tuple<int, int, vector<pair<int, int>>, string>> fixed;
  fixed.push_back({12, 1, {}, "all-left-isolated"});
  fixed.push_back({1, 12, {}, "all-right-isolated"});
  fixed.push_back({12, 12, {}, "empty"});
  {
    vector<pair<int, int>> e;
    for (int i = 0; i < 12; ++i) e.push_back({i, 11 - i});
    fixed.push_back({12, 12, e, "reverse-perfect"});
  }
  {
    vector<pair<int, int>> e;
    for (int u = 0; u < 10; ++u) for (int v = 0; v < 10; ++v)
      if ((u + 2 * v) % 5 != 0) e.push_back({u, v});
    reverse(e.begin(), e.end());
    vector<pair<int, int>> with_duplicates = e;
    for (int i = 0; i < (int)e.size(); i += 3) with_duplicates.push_back(e[i]);
    fixed.push_back({10, 10, with_duplicates, "duplicates-and-reversed"});
  }
  for (auto &[l, r, edges, label] : fixed)
    check(bm, l, r, edges, tc++, label);

  mt19937 rng(0x14B17E);
  for (int z = 0; z < 3500; ++z) {
    int l = rng() % 16, r = rng() % 16;
    vector<pair<int, int>> edges;
    unsigned mode = z % 6;
    for (int u = 0; u < l; ++u) for (int v = 0; v < r; ++v) {
      bool take = mode == 0 ? (u == v) : mode == 1 ? (u + v == r - 1) :
                  mode == 2 ? (rng() % 4 == 0) : mode == 3 ? (rng() % 2 == 0) :
                  mode == 4 ? (rng() % 5 != 0) : (u * 7 + v * 11 + z) % 9 < 4;
      if (take) edges.push_back({u, v});
    }
    shuffle(edges.begin(), edges.end(), rng);
    if (z % 2 == 0)
      for (int i = 0; i < (int)edges.size(); i += 7) edges.push_back(edges[i]);
    check(bm, l, r, edges, tc++, "seeded-random");
  }
  cout << "PASS " << tc << " bipartite edge-case/oracle cases\n";
}
