#include <bits/stdc++.h>
using namespace std;

namespace dynamic_mst_template {
constexpr int maxn = 512;

struct RollbackDSU {
  struct Change { int child, parent_size; };
  vector<int> parent;
  vector<Change> history;
  vector<int> checkpoints;
  void init(int n) { parent.assign(n, -1), history.clear(), checkpoints.clear(); }
  int find(int x) const { while (parent[x] >= 0) x = parent[x]; return x; }
  void save() { checkpoints.push_back(history.size()); }
  bool merge(int a, int b) {
    a = find(a), b = find(b);
    if (a == b) { history.push_back({-1, 0}); return false; }
    if (parent[a] > parent[b]) swap(a, b);
    history.push_back({b, parent[a]});
    parent[a] += parent[b], parent[b] = a;
    return true;
  }
  void undo() {
    int checkpoint = checkpoints.back(); checkpoints.pop_back();
    while ((int)history.size() > checkpoint) {
      auto [child, parent_size] = history.back(); history.pop_back();
      if (child == -1) continue;
      int root = parent[child];
      parent[child] = parent[root] - parent_size;
      parent[root] = parent_size;
    }
  }
} djs;

vector<long long> output;
int capture_printf(const char *, long long value) {
  output.push_back(value);
  return 0;
}
#define printf capture_printf
#include "../../codebook/9_Else/DynamicMST.cpp"
#undef printf
}  // namespace dynamic_mst_template

static void require(bool condition, const string &message) {
  if (!condition) {
    cerr << "FAIL " << message << '\n';
    exit(1);
  }
}

struct Edge { int u, v, w; };

static long long brute_mst(int n, vector<Edge> edges) {
  sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b) {
    return tie(a.w, a.u, a.v) < tie(b.w, b.u, b.v);
  });
  vector<int> parent(n, -1);
  function<int(int)> find = [&](int x) {
    return parent[x] < 0 ? x : parent[x] = find(parent[x]);
  };
  long long answer = 0; int used = 0;
  for (auto edge : edges) {
    int a = find(edge.u), b = find(edge.v);
    if (a == b) continue;
    if (parent[a] > parent[b]) swap(a, b);
    parent[a] += parent[b], parent[b] = a;
    answer += edge.w, ++used;
  }
  return used == n - 1 ? answer : LLONG_MAX;
}

int main() {
  using namespace dynamic_mst_template;
  mt19937 rng(27182818);
  for (int test = 0; test < 4000; ++test) {
    int n = 2 + rng() % 6;
    vector<Edge> edges;
    for (int v = 1; v < n; ++v)
      edges.push_back({v, (int)(rng() % v), 1 + (int)(rng() % 30)});
    for (int u = 0; u < n; ++u) for (int v = u + 1; v < n; ++v)
      if (rng() % 3 == 0) edges.push_back({u, v, 1 + (int)(rng() % 30)});
    int m = edges.size(), q = 1 + rng() % 8;
    vector<pair<int, int>> updates(q);
    for (int i = 0; i < q; ++i)
      updates[i] = {static_cast<int>(rng() % m), 1 + static_cast<int>(rng() % 30)};

    fill_n(cnt, m, 0);
    for (int i = 0; i < m; ++i)
      st[i] = edges[i].u, ed[i] = edges[i].v, cost[i] = edges[i].w;
    for (int i = 0; i < q; ++i) {
      qr[i] = updates[i];
      ++cnt[updates[i].first];
    }
    vector<int> stable;
    for (int i = 0; i < m; ++i) if (!cnt[i]) stable.push_back(i);
    djs.init(n), output.clear();
    solve(0, q - 1, stable, 0);

    vector<long long> want;
    vector<Edge> current = edges;
    for (auto [id, weight] : updates) {
      current[id].w = weight;
      want.push_back(brute_mst(n, current));
    }
    if (output != want) {
      cerr << "dynamic MST mismatch test=" << test << " n=" << n
           << " m=" << m << " q=" << q << '\n';
      cerr << "got:"; for (auto x : output) cerr << ' ' << x; cerr << '\n';
      cerr << "want:"; for (auto x : want) cerr << ' ' << x; cerr << '\n';
      require(false, "dynamic MST offline updates");
    }
  }
  cout << "dynamic MST PASS\n";
}
