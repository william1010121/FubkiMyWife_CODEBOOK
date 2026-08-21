#include <bits/stdc++.h>
using namespace std;

#define N 8
#define T 4
#define SZ(v) ((int)(v).size())
#define copy_n std::copy_n
const int INF = 1000000000;
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"

struct Edge { int u, v, w; };

static vector<int> unique_terms(vector<int> term) {
  sort(term.begin(), term.end());
  term.erase(unique(term.begin(), term.end()), term.end());
  return term;
}

// Enumerate selected directed edges and a common sink reachable from every
// terminal.  This is deliberately tiny and independent of the DP template.
static int oracle(int n, const vector<Edge> &es, vector<int> term,
                  const vector<int> &vc) {
  term = unique_terms(term);
  if (term.empty()) return 0;
  int ans = INF;
  for (int mask = 0; mask < (1 << (int)es.size()); ++mask) {
    int cost = 0;
    vector<char> used(n);
    for (int x : term) used[x] = 1;
    vector<vector<int>> g(n);
    for (int i = 0; i < (int)es.size(); ++i) if (mask >> i & 1) {
      cost += es[i].w;
      used[es[i].u] = used[es[i].v] = 1;
      g[es[i].u].push_back(es[i].v);
    }
    for (int i = 0; i < n; ++i) cost += used[i] * vc[i];
    if (cost >= ans) continue;
    for (int sink = 0; sink < n; ++sink) {
      bool ok = true;
      for (int source : term) {
        vector<char> seen(n);
        queue<int> q;
        seen[source] = 1;
        q.push(source);
        while (!q.empty()) {
          int u = q.front();
          q.pop();
          for (int v : g[u]) if (!seen[v]) seen[v] = 1, q.push(v);
        }
        if (!seen[sink]) { ok = false; break; }
      }
      if (ok) {
        ans = cost;
        break;
      }
    }
  }
  return ans;
}

static int failures = 0;
static int cases = 0;

static void check(int n, const vector<Edge> &es, const vector<int> &term,
                  const vector<int> &vc, const string &label) {
  SteinerTree st;
  st.init(n);
  for (int i = 0; i < n; ++i) st.vcst[i] = vc[i];
  for (auto e : es) st.add_edge(e.u, e.v, e.w);
  int got = st.solve(term);
  int want = oracle(n, es, term, vc);
  ++cases;
  if (got == want) return;
  if (failures++ < 12) {
    cerr << "MinimumSteinerTree mismatch label=" << label << " case=" << cases
         << " n=" << n << " got=" << got << " want=" << want << " vc=";
    for (int x : vc) cerr << x << ',';
    cerr << " terminals=";
    for (int x : term) cerr << x << ',';
    cerr << " edges=";
    for (auto e : es) cerr << '(' << e.u << ',' << e.v << ',' << e.w << ')';
    cerr << '\n';
  }
}

int main() {
  // Direct edge cases: empty and repeated terminals are interpreted as sets.
  check(1, {}, {}, {0}, "empty-terminals");
  check(1, {}, {0}, {0}, "single-zero-cost-vertex");
  check(1, {}, {0}, {7}, "single-positive-cost-vertex");
  check(1, {}, {0, 0}, {7}, "repeated-single-terminal");
  check(3, {}, {0, 2}, {0, 0, 0}, "disconnected-terminals");
  check(3, {{0, 1, 2}, {1, 2, 3}}, {0, 2}, {1, 4, 2}, "directed-path");
  check(3, {{2, 1, 1}, {1, 0, 1}}, {0, 2}, {2, 0, 3}, "reverse-directed-path");
  check(4, {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {2, 3, 1}},
        {1, 2, 1}, {0, 5, 5, 1}, "repeated-terminal-diamond");

  // Exhaust all simple directed graphs on three vertices for several cost
  // vectors and every nonempty terminal subset.
  vector<pair<int, int>> universe;
  for (int u = 0; u < 3; ++u) for (int v = 0; v < 3; ++v)
    if (u != v) universe.push_back({u, v});
  for (int mask = 0; mask < (1 << (int)universe.size()); ++mask) {
    vector<Edge> es;
    for (int i = 0; i < (int)universe.size(); ++i)
      if (mask >> i & 1) es.push_back({universe[i].first, universe[i].second, 1 + (i % 3)});
    for (const vector<int> &vc : vector<vector<int>>{{0, 0, 0}, {0, 1, 2}, {2, 0, 3}})
      for (int tmask = 1; tmask < (1 << 3); ++tmask) {
        vector<int> term;
        for (int i = 0; i < 3; ++i) if (tmask >> i & 1) term.push_back(i);
        check(3, es, term, vc, "exhaustive-n3");
      }
  }

  mt19937 rng(0x571E2026);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 2 + rng() % 5;
    vector<int> vc(n);
    for (int &x : vc) x = rng() % 6;
    vector<Edge> es;
    for (int i = 0; i < 8; ++i) {
      int u = rng() % n, v = rng() % n;
      if (u == v) continue;
      es.push_back({u, v, 1 + (int)(rng() % 8)});
    }
    int k = rng() % (T + 1);
    vector<int> term;
    for (int i = 0; i < k; ++i) term.push_back(rng() % n);
    check(n, es, term, vc, "seeded-random");
  }
  if (failures) {
    cerr << "MinimumSteinerTree FAIL mismatches=" << failures
         << " cases=" << cases << '\n';
    return 1;
  }
  cout << "MinimumSteinerTree PASS cases=" << cases
       << " edge-subset oracle, disconnected and repeated-terminal cases\n";
}
