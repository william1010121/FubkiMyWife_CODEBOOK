#include <bits/stdc++.h>
using namespace std;

#define N 128
#define pb push_back
#define ALL(x) (x).begin(), (x).end()
vector<int> base[N];
int par[N], dep[N], dfn[N], timer_dfs;

int LCA(int u, int v) {
  while (dep[u] > dep[v]) u = par[u];
  while (dep[v] > dep[u]) v = par[v];
  while (u != v) u = par[u], v = par[v];
  return u;
}

static void dfs(int u, int p) {
  par[u] = p;
  dfn[u] = timer_dfs++;
  for (int v : base[u]) if (v != p) dep[v] = dep[u] + 1, dfs(v, u);
}

#include "../../codebook/2_Graph/Virtual_Tree.cpp"

struct Expected {
  int root = -1;
  set<int> nodes;
  set<pair<int, int>> edges;
};

static Expected expected_tree(const vector<int> &keys) {
  Expected out;
  out.nodes.insert(keys.begin(), keys.end());
  for (int i = 0; i < (int)keys.size(); ++i)
    for (int j = i + 1; j < (int)keys.size(); ++j)
      out.nodes.insert(LCA(keys[i], keys[j]));
  for (int u : out.nodes) {
    if (par[u] == u) {
      out.root = u;
      continue;
    }
    int p = par[u];
    while (p != par[p] && !out.nodes.count(p)) p = par[p];
    if (out.nodes.count(p)) out.edges.insert({p, u});
    else if (out.root == -1) out.root = u;
    else {
      cerr << "Virtual_Tree oracle found multiple roots\n";
      exit(1);
    }
  }
  return out;
}

static void clear_virtual(int n) {
  for (int i = 0; i < n; ++i) vG[i].clear();
}

static void fail(int tc, int n, const vector<int> &keys, const string &why) {
  cerr << "Virtual_Tree mismatch tc=" << tc << " n=" << n << ": " << why << '\n';
  cerr << "keys:";
  for (int x : keys) cerr << ' ' << x;
  cerr << '\n';
  exit(1);
}

static void check_unique(int tc, int n, vector<int> keys, int &cases) {
  Expected want = expected_tree(keys);
  clear_virtual(n);
  vector<int> input = keys;
  if (!input.empty())
    rotate(input.begin(), input.begin() + tc % input.size(), input.end());
  int got_root = build(input);
  if (got_root != want.root) fail(tc, n, keys, "root differs");
  set<pair<int, int>> got;
  for (int u = 0; u < n; ++u) for (int v : vG[u]) {
    if (u == v) fail(tc, n, keys, "self-loop in virtual tree");
    if (!got.insert({u, v}).second)
      fail(tc, n, keys, "repeated virtual edge");
  }
  if (got != want.edges) fail(tc, n, keys, "virtual edges differ");
  if (got_root != -1) reset(got_root);
  for (int u = 0; u < n; ++u)
    if (!vG[u].empty()) fail(tc, n, keys, "reset left stale edges");
  ++cases;
}

static void check_raw_duplicates(int tc, int n, const vector<int> &raw, int &cases) {
  vector<int> keys = raw;
  sort(keys.begin(), keys.end());
  keys.erase(unique(keys.begin(), keys.end()), keys.end());
  Expected want = expected_tree(keys);
  clear_virtual(n);
  vector<int> input = raw;
  int got_root = build(input);
  set<pair<int, int>> got;
  for (int u = 0; u < n; ++u) for (int v : vG[u]) {
    if (u == v) fail(tc, n, raw, "duplicate keys create a self-loop");
    if (!got.insert({u, v}).second)
      fail(tc, n, raw, "repeated virtual edge");
  }
  if (got_root != want.root || got != want.edges)
    fail(tc, n, raw, "duplicate keys differ from their set semantics");
  if (!is_sorted(input.begin(), input.end(),
                 [](int a, int b) { return dfn[a] < dfn[b]; }) ||
      adjacent_find(input.begin(), input.end()) != input.end() ||
      set<int>(input.begin(), input.end()) != set<int>(keys.begin(), keys.end()))
    fail(tc, n, raw, "in-place key ordering/deduplication differs");
  if (got_root != -1) reset(got_root);
  for (int u = 0; u < n; ++u)
    if (!vG[u].empty()) fail(tc, n, raw, "reset left stale edges");
  ++cases;
}

static void enumerate_keys(int n, int mask, int tc, int &cases) {
  vector<int> keys;
  for (int i = 0; i < n; ++i) if (mask >> i & 1) keys.push_back(i);
  check_unique(tc, n, keys, cases);
}

static void enumerate_trees(int at, int n, vector<int> &parents, int &tc, int &cases) {
  if (at == n) {
    for (int i = 0; i < n; ++i) base[i].clear();
    for (int i = 1; i < n; ++i) base[i].pb(parents[i]), base[parents[i]].pb(i);
    timer_dfs = 0;
    dep[0] = 0;
    dfs(0, 0);
    for (int mask = 0; mask < (1 << n); ++mask) enumerate_keys(n, mask, tc++, cases);
    return;
  }
  for (int p = 0; p < at; ++p) {
    parents[at] = p;
    enumerate_trees(at + 1, n, parents, tc, cases);
  }
}

int main() {
  int cases = 0, tc = 0;
  for (int n = 1; n <= 6; ++n) {
    vector<int> parents(n);
    parents[0] = 0;
    enumerate_trees(1, n, parents, tc, cases);
  }

  mt19937 rng(0x71A2026);
  mt19937 reroot_rng(0x5026);
  int repeated_inputs = 0;
  for (int it = 0; it < 9000; ++it) {
    int n = 2 + rng() % 38;
    for (int i = 0; i < n; ++i) base[i].clear();
    for (int i = 1; i < n; ++i) {
      int p = rng() % i;
      base[i].pb(p), base[p].pb(i);
    }
    timer_dfs = 0;
    dep[0] = 0;
    dfs(0, 0);

    int raw_size = rng() % (2 * n + 1);
    vector<int> raw;
    for (int i = 0; i < raw_size; ++i) raw.pb(rng() % n);
    vector<int> keys = raw;
    sort(keys.begin(), keys.end());
    keys.erase(unique(keys.begin(), keys.end()), keys.end());
    if (keys.size() != raw.size()) ++repeated_inputs;
    check_unique(tc++, n, keys, cases);
    // Root and adjacency order are independent of vertex labels.
    for (int i = 0; i < n; ++i)
      shuffle(base[i].begin(), base[i].end(), reroot_rng);
    int root = reroot_rng() % n;
    timer_dfs = 0;
    dep[root] = 0;
    dfs(root, root);
    check_raw_duplicates(tc++, n, raw, cases);
  }
  vector<int> empty;
  clear_virtual(1);
  if (build(empty) != -1) return 1;
  // All N vertices, both deepest and widest trees, stress the stack bound.
  for (bool chain : {false, true}) {
    for (int i = 0; i < N; ++i) base[i].clear();
    for (int i = 1; i < N; ++i) {
      int p = chain ? i - 1 : 0;
      base[i].pb(p), base[p].pb(i);
    }
    timer_dfs = 0;
    dep[N - 1] = 0;
    dfs(N - 1, N - 1);
    vector<int> raw;
    for (int i = N - 1; i >= 0; --i) raw.pb(i), raw.pb(i);
    check_raw_duplicates(tc++, N, raw, cases);
    // Exercise the public insert() path in its required preorder.
    vector<int> ordered = raw;
    sort(ordered.begin(), ordered.end(),
         [](int a, int b) { return dfn[a] < dfn[b]; });
    ordered.erase(unique(ordered.begin(), ordered.end()), ordered.end());
    top = -1;
    for (int u : ordered) insert(u);
    while (top > 0) vG[st[top - 1]].pb(st[top]), --top;
    Expected want = expected_tree(ordered);
    set<pair<int, int>> got;
    for (int u = 0; u < N; ++u) for (int v : vG[u]) {
      if (!got.insert({u, v}).second)
        fail(tc, N, ordered, "direct insert repeats edge");
    }
    if (st[0] != want.root || got != want.edges)
      fail(tc, N, ordered, "direct insert differs");
    reset(st[0]);
    ++cases;
  }
  // Directly exercise the repeated-key boundary before calling reset().
  int duplicate_n = 5;
  for (int i = 0; i < duplicate_n; ++i) base[i].clear();
  for (int i = 1; i < duplicate_n; ++i) base[i].pb(i - 1), base[i - 1].pb(i);
  timer_dfs = 0;
  dep[0] = 0;
  dfs(0, 0);
  check_raw_duplicates(tc++, duplicate_n, {4, 4, 2, 2, 0, 0}, cases);
  cout << "Virtual_Tree PASS cases=" << cases
       << " exhaustive rooted trees/subsets plus " << repeated_inputs
       << " raw repeated-key cases, rerooting and N-boundary/direct insert\n";
}
