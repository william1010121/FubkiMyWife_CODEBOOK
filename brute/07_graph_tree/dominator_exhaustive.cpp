#include <bits/stdc++.h>
using namespace std;

#define N 16
#define pb push_back
#include "../../codebook/2_Graph/Dominator_Tree.cpp"

struct TestCase {
  int n, root;
  vector<pair<int, int>> edges;
};

static vector<char> reachable(const vector<vector<int>> &g, int root) {
  vector<char> seen(g.size());
  queue<int> q;
  seen[root] = 1;
  q.push(root);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : g[u]) if (!seen[v]) seen[v] = 1, q.push(v);
  }
  return seen;
}

static vector<set<int>> brute_dominators(int n,
                                         const vector<pair<int, int>> &edges,
                                         int root) {
  vector<vector<int>> rg(n);
  for (auto [u, v] : edges) rg[v].pb(u);
  vector<vector<int>> g(n);
  for (auto [u, v] : edges) g[u].pb(v);
  vector<char> seen = reachable(g, root);
  set<int> all;
  for (int i = 0; i < n; ++i) if (seen[i]) all.insert(i);

  vector<set<int>> dom(n);
  for (int v = 0; v < n; ++v) if (seen[v])
    dom[v] = v == root ? set<int>{root} : all;

  bool changed = true;
  while (changed) {
    changed = false;
    for (int v = 0; v < n; ++v) if (seen[v] && v != root) {
      bool first = true;
      set<int> next;
      for (int p : rg[v]) if (seen[p]) {
        if (first) next = dom[p], first = false;
        else {
          set<int> intersection;
          set_intersection(next.begin(), next.end(), dom[p].begin(),
                           dom[p].end(), inserter(intersection, intersection.begin()));
          next.swap(intersection);
        }
      }
      next.insert(v);
      if (next != dom[v]) dom[v] = next, changed = true;
    }
  }
  return dom;
}

static vector<int> brute_idom(const vector<set<int>> &dom, const vector<char> &seen,
                              int root) {
  vector<int> idom(dom.size(), -1);
  for (int v = 0; v < (int)dom.size(); ++v) if (seen[v] && v != root) {
    for (int x : dom[v]) if (x != v &&
        (idom[v] == -1 || dom[x].size() > dom[idom[v]].size())) idom[v] = x;
  }
  return idom;
}

static void fail(const TestCase &tc, const string &why) {
  cerr << "Dominator_Tree mismatch: " << why << " n=" << tc.n
       << " root=" << tc.root + 1 << " edges=" << tc.edges.size() << '\n';
  for (auto [u, v] : tc.edges) cerr << u + 1 << ' ' << v + 1 << '\n';
  exit(1);
}

static void check(const TestCase &tc, int &cases) {
  vector<vector<int>> g(tc.n);
  for (auto [u, v] : tc.edges) g[u].pb(v);
  vector<char> seen = reachable(g, tc.root);
  vector<set<int>> dom = brute_dominators(tc.n, tc.edges, tc.root);
  vector<int> want = brute_idom(dom, seen, tc.root);

  dominator_tree dt;
  dt.init(tc.n);
  for (auto [u, v] : tc.edges) dt.add_edge(u + 1, v + 1);
  for (int repeat = 0; repeat < 2; ++repeat) {
    dt.tarjan(tc.root + 1);
    vector<int> got(tc.n + 1, 0);
    for (int u = 1; u <= tc.n; ++u) for (int v : dt.tree[u]) {
      if (v < 1 || v > tc.n || got[v]) fail(tc, "tree parent is invalid or duplicated");
      got[v] = u;
    }
    for (int v = 0; v < tc.n; ++v) {
      int expected = !seen[v] ? 0 : v == tc.root ? 0 : want[v] + 1;
      if (got[v + 1] != expected)
        fail(tc, "immediate dominator differs at vertex " + to_string(v + 1));
    }
  }
  ++cases;
}

static void enumerate_graphs(int n, int &cases) {
  vector<pair<int, int>> universe;
  for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
    universe.push_back({u, v}); // self-loops are intentional boundary cases
  uint64_t limit = uint64_t{1} << universe.size();
  for (uint64_t mask = 0; mask < limit; ++mask) {
    vector<pair<int, int>> edges;
    for (int i = 0; i < (int)universe.size(); ++i)
      if (mask >> i & 1) edges.push_back(universe[i]);
    for (int root = 0; root < n; ++root) check({n, root, edges}, cases);
  }
}

int main() {
  int cases = 0;
  for (int n = 1; n <= 4; ++n) enumerate_graphs(n, cases);

  mt19937 rng(0xD07A2026);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 5 + rng() % 6;
    vector<pair<int, int>> edges;
    for (int u = 0; u < n; ++u) for (int v = 0; v < n; ++v)
      if (rng() % 100 < 31) {
        edges.push_back({u, v});
        if (rng() % 13 == 0) edges.push_back({u, v}); // parallel edge
      }
    shuffle(edges.begin(), edges.end(), rng); // insertion order is arbitrary
    for (int root : {0, n - 1, (int)(rng() % n)})
      check({n, root, edges}, cases);
  }
  cout << "Dominator_Tree PASS cases=" << cases
       << " exhaustive-n<=4 plus seeded multigraphs\n";
}
