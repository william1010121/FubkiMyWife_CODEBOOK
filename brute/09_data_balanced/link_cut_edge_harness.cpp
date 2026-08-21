#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define ALL(x) (x).begin(), (x).end()
#include "../../codebook/3_Data_Structure/link_cut_tree.cpp"

#ifndef LCT_TEST_LCA
#define LCT_TEST_LCA 1
#endif

[[noreturn]] static void fail(const string &msg) {
  cerr << "link_cut_edge_harness: " << msg << '\n';
  exit(1);
}

#define CHECK(cond) do { if (!(cond)) fail(string(__FILE__) + ":" + to_string(__LINE__) + ": " #cond); } while (0)

static string debug_context;

static int node_index(Splay *p, vector<Splay> &sp) {
  for (int i = 0; i < (int)sp.size(); ++i) if (p == &sp[i]) return i;
  return -1;
}

struct Forest {
  int n;
  vector<int> value, parent;
  vector<vector<int>> adj;

  explicit Forest(int n_ = 0) : n(n_), value(n_), parent(n_, -1), adj(n_) {}

  int root(int x) const {
    while (parent[x] != -1) x = parent[x];
    return x;
  }

  bool connected(int a, int b) const {
    vector<char> seen(n, false);
    queue<int> q;
    q.push(a), seen[a] = true;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      if (u == b) return true;
      for (int v : adj[u]) if (!seen[v]) seen[v] = true, q.push(v);
    }
    return false;
  }

  int path_xor(int a, int b) const {
    vector<int> prev(n, -2);
    queue<int> q;
    q.push(a), prev[a] = -1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : adj[u]) if (prev[v] == -2)
        prev[v] = u, q.push(v);
    }
    CHECK(prev[b] != -2);
    int result = 0;
    for (int u = b;; u = prev[u]) {
      result ^= value[u];
      if (u == a) break;
    }
    return result;
  }

  int lca(int a, int b) const {
    CHECK(connected(a, b));
    vector<char> seen(n, false);
    for (int u = a; u != -1; u = parent[u]) seen[u] = true;
    for (int u = b; u != -1; u = parent[u]) if (seen[u]) return u;
    return -1;
  }

  void link_roots(int p, int c) {
    CHECK(p != c && root(p) == p && root(c) == c);
    adj[p].push_back(c), adj[c].push_back(p);
    parent[c] = p;
  }

  void cut_edge(int p, int c) {
    CHECK(parent[c] == p);
    auto erase_one = [](vector<int> &v, int x) {
      auto it = find(v.begin(), v.end(), x);
      CHECK(it != v.end());
      v.erase(it);
    };
    erase_one(adj[p], c), erase_one(adj[c], p);
    parent[c] = -1;
    // split(p,c) starts with chroot(p); after the cut the two represented
    // trees are rooted at the two endpoints, not necessarily at the old
    // component root.
    reroot(p), reroot(c);
  }

  void reroot(int x) {
    vector<char> seen(n, false);
    queue<int> q;
    q.push(x), seen[x] = true, parent[x] = -1;
    while (!q.empty()) {
      int u = q.front(); q.pop();
      for (int v : adj[u]) if (!seen[v]) {
        seen[v] = true;
        parent[v] = u;
        q.push(v);
      }
    }
  }

  vector<pair<int, int>> oriented_edges() const {
    vector<pair<int, int>> edges;
    for (int c = 0; c < n; ++c) if (parent[c] != -1)
      edges.push_back({parent[c], c});
    return edges;
  }
};

static void check_forest(vector<Splay> &sp, Forest &f) {
  CHECK((int)sp.size() == f.n);
  for (int i = 0; i < f.n; ++i) {
    Splay *got_root = get_root(&sp[i]);
    int want_root = f.root(i);
    if (got_root != &sp[want_root]) {
      cerr << "root mismatch n=" << f.n << " i=" << i
           << " got=" << node_index(got_root, sp)
           << " want=" << want_root << " context=" << debug_context << " parent=";
      for (int z = 0; z < f.n; ++z) cerr << f.parent[z] << (z + 1 == f.n ? '\n' : ' ');
      fail("root contract mismatch");
    }
  }
  for (int i = 0; i < f.n; ++i) {
    for (int j = 0; j < f.n; ++j) {
      bool want_conn = f.connected(i, j);
      CHECK(conn(&sp[i], &sp[j]) == want_conn);
      if (!want_conn) continue;
      CHECK(query(&sp[i], &sp[j]) == f.path_xor(i, j));
      // query() calls split(), whose chroot(x) intentionally makes x the
      // represented root.  Keep the reference orientation in sync.
      f.reroot(i);
#if LCT_TEST_LCA
      Splay *got_lca = lca(&sp[i], &sp[j]);
      int want_lca = f.lca(i, j);
      if (got_lca != &sp[want_lca]) {
        Splay *reverse_lca = lca(&sp[j], &sp[i]);
        cerr << "lca mismatch n=" << f.n << " i=" << i << " j=" << j
             << " got=" << node_index(got_lca, sp)
             << " reverse=" << node_index(reverse_lca, sp)
             << " want=" << want_lca << " roots=";
        for (int z = 0; z < f.n; ++z) cerr << f.root(z) << (z + 1 == f.n ? '\n' : ' ');
        cerr << "parent=";
        for (int z = 0; z < f.n; ++z) cerr << f.parent[z] << (z + 1 == f.n ? '\n' : ' ');
        fail("lca contract mismatch");
      }
#endif
    }
  }
}

static vector<Splay> make_nodes(Forest &f) {
  vector<Splay> sp(f.n);
  for (int i = 0; i < f.n; ++i) {
    f.value[i] = (i * 17 + 3) ^ (i & 1 ? 7 : 0);
    sp[i].val = sp[i].sum = f.value[i];
  }
  return sp;
}

static void exhaustive_root_attachments() {
  // Every sequence of attaching the next singleton either above or below
  // the existing tree, followed by every possible reroot, cut and relink.
  for (int n = 1; n <= 8; ++n) {
    for (int mask = 0; mask < (1 << max(0, n - 1)); ++mask) {
      Forest f(n);
      vector<Splay> sp = make_nodes(f);
      debug_context = "exhaustive n=" + to_string(n) + " mask=" + to_string(mask) + " initial";
      check_forest(sp, f);
      for (int i = 1; i < n; ++i) {
        int old_root = f.root(0);
        if (mask & (1 << (i - 1))) {
          link(&sp[i], &sp[old_root]);
          f.link_roots(i, old_root);
        } else {
          link(&sp[old_root], &sp[i]);
          f.link_roots(old_root, i);
        }
        debug_context = "exhaustive n=" + to_string(n) + " mask=" + to_string(mask) +
                        " attach=" + to_string(i);
        check_forest(sp, f);
      }
      for (int x = 0; x < n; ++x) {
        chroot(&sp[x]);
        f.reroot(x);
        debug_context = "exhaustive n=" + to_string(n) + " mask=" + to_string(mask) +
                        " reroot=" + to_string(x);
        check_forest(sp, f);
      }

      vector<pair<int, int>> edges = f.oriented_edges();
      for (auto [p, c] : edges) {
        cut(&sp[p], &sp[c]);
        f.cut_edge(p, c);
        debug_context = "exhaustive n=" + to_string(n) + " mask=" + to_string(mask) +
                        " cut=" + to_string(p) + "," + to_string(c);
        check_forest(sp, f);
        // Cutting an already removed edge must be harmless.
        cut(&sp[p], &sp[c]);
        f.reroot(p), f.reroot(c);
        check_forest(sp, f);
      }
      for (auto [p, c] : edges) {
        int rp = f.root(p), rc = f.root(c);
        if (rp == rc) continue;
        link(&sp[rp], &sp[rc]);
        f.link_roots(rp, rc);
        debug_context = "exhaustive n=" + to_string(n) + " mask=" + to_string(mask) +
                        " relink=" + to_string(rp) + "," + to_string(rc);
        check_forest(sp, f);
      }
    }
  }
}

static void randomized_stress() {
  mt19937 rng(0x09c071);
  for (int tc = 0; tc < 120; ++tc) {
    int n = 2 + rng() % 9;
    Forest f(n);
    vector<Splay> sp = make_nodes(f);
    for (int i = 0; i < n; ++i) {
      f.value[i] = (int)(rng() % 33) - 16;
      sp[i].val = sp[i].sum = f.value[i];
    }
    check_forest(sp, f);
    for (int step = 0; step < 230; ++step) {
      int action = rng() % 100;
      if (action < 20) {
        int x = rng() % n, value = (int)(rng() % 41) - 20;
        change(&sp[x], value);
        f.value[x] = value;
      } else if (action < 42) {
        int x = rng() % n;
        chroot(&sp[x]);
        f.reroot(x);
      } else if (action < 66) {
        int x = rng() % n, y = rng() % n;
        int rx = f.root(x), ry = f.root(y);
        if (rx != ry) {
          link(&sp[rx], &sp[ry]);
          f.link_roots(rx, ry);
        }
      } else if (action < 86) {
        vector<pair<int, int>> edges = f.oriented_edges();
        if (!edges.empty()) {
          auto [p, c] = edges[rng() % edges.size()];
          cut(&sp[p], &sp[c]);
          f.cut_edge(p, c);
          cut(&sp[p], &sp[c]);
          f.reroot(p), f.reroot(c);
        }
      } else {
        // Direct queries include singleton paths and duplicate values.
        int x = rng() % n, y = rng() % n;
        CHECK(conn(&sp[x], &sp[y]) == f.connected(x, y));
        if (f.connected(x, y)) {
          CHECK(query(&sp[x], &sp[y]) == f.path_xor(x, y));
          f.reroot(x);
#if LCT_TEST_LCA
          CHECK(lca(&sp[x], &sp[y]) == &sp[f.lca(x, y)]);
#endif
        }
      }
      debug_context = "random tc=" + to_string(tc) + " step=" + to_string(step);
      check_forest(sp, f);
    }
  }
}

int main() {
  exhaustive_root_attachments();
  randomized_stress();
  return 0;
}
