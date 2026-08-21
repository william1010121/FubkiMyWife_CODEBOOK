#include <bits/stdc++.h>
using namespace std;

#define SZ(x) static_cast<int>((x).size())
#include "../../codebook/4_Flow_Matching/isap.cpp"
#include "flow_oracle.hpp"

struct Arc { int u, v, c; };

static void check_case(int n, const vector<Arc> &arcs, int case_id) {
  const int s = n + 1, t = n + 2;
  vector<vector<int>> cap(n + 3, vector<int>(n + 3));
  flow.init(n);
  for (const Arc &e : arcs) {
    flow12::require(0 <= e.u && e.u < n + 3 && 0 <= e.v && e.v < n + 3 && e.u != e.v,
                    "invalid ISAP test arc");
    flow12::require(e.c >= 0, "negative ISAP test capacity");
    cap[e.u][e.v] += e.c;
    flow.addEdge(e.u, e.v, e.c);  // Explicit zero edges exercise residual setup.
  }
  const int want = flow12::directed_mincut(cap, s, t);
  const int got = flow.solve();
  if (got != want) {
    cerr << "ISAP mismatch case=" << case_id << " got=" << got
         << " want=" << want << " arcs=";
    for (const Arc &e : arcs) cerr << '(' << e.u << ',' << e.v << ',' << e.c << ')';
    cerr << '\n';
    exit(1);
  }
}

static void exhaustive_binary_graphs() {
  int case_id = 0;
  // n is the number of ordinary vertices; n+1 and n+2 are the reserved s,t.
  for (int n = 0; n <= 4; ++n) {
    const int s = n + 1, t = n + 2;
    vector<pair<int, int>> pairs;
    for (int u = 0; u < n + 3; ++u)
      for (int v = 0; v < n + 3; ++v)
        if (u != v && u != t && v != s && u != n && v != n)
          pairs.push_back({u, v});
    // The unused slot n is intentionally excluded; all other relevant
    // directions, including zero-capacity source/sink arcs, are exhaustive.
    for (int mask = 0; mask < (1 << pairs.size()); ++mask) {
      vector<Arc> arcs;
      for (int i = 0; i < static_cast<int>(pairs.size()); ++i)
        arcs.push_back({pairs[i].first, pairs[i].second, (mask >> i) & 1});
      check_case(n, arcs, case_id++);
    }
  }
  cout << "ISAP exhaustive binary terminal graphs: " << case_id << '\n';
}

static void parallel_and_random_graphs() {
  int case_id = 100000;
  check_case(4,
             {{0, 1, 0}, {0, 1, 2}, {0, 1, 5}, {1, 2, 3},
              {1, 2, 0}, {1, 2, 1}, {2, 3, 4}, {0, 3, 1},
              {5, 0, 2}, {3, 6, 3}, {6, 1, 0}},
             case_id++);

  mt19937 rng(0x15A912);
  for (int tc = 0; tc < 1800; ++tc) {
    const int n = static_cast<int>(rng() % 7);  // Includes n=0 and isolated graphs.
    const int s = n + 1, t = n + 2;
    vector<Arc> arcs;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v) if (u != v) {
      const int copies = 1 + static_cast<int>(rng() % 3);
      for (int k = 0; k < copies; ++k)
        arcs.push_back({u, v, static_cast<int>(rng() % 7)});
    }
    for (int v = 0; v < n; ++v) {
      for (int k = 0, copies = 1 + static_cast<int>(rng() % 3); k < copies; ++k)
        arcs.push_back({s, v, static_cast<int>(rng() % 7)});
      for (int k = 0, copies = 1 + static_cast<int>(rng() % 3); k < copies; ++k)
        arcs.push_back({v, t, static_cast<int>(rng() % 7)});
    }
    check_case(n, arcs, case_id++);
  }
  cout << "ISAP parallel/disconnected random graphs: 1801\n";
}

int main() {
  exhaustive_binary_graphs();
  parallel_and_random_graphs();
  cout << "CONTRACT: ISAP exposes fixed distinct terminals n+1 and n+2; s==t is not an API case\n";
  cout << "PASS ISAP edge-case oracle suite\n";
}
