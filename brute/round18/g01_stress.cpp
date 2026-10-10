#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
#include "../../codebook/3_Data_Structure/Treap.cpp"

void collect(node *o, vector<pii> &out) {
  if (!o) return;
  assert(!o->l || o->priority <= o->l->priority);
  assert(!o->r || o->priority <= o->r->priority);
  assert(o->sz == 1 + sz(o->l) + sz(o->r));
  collect(o->l, out); out.pb({o->data, o->priority});
  collect(o->r, out);
}
int main() {
  // Many isolates must not initialize a BFS workspace for every vertex.
  const int n = 20000;
  Matching g(n);
  assert(g.solve() == 0);
  for (int u = 0; u < n; ++u) assert(g.match[u] == n);
  g.add_edge(n - 2, n - 1); g.add_edge(n - 2, n - 1);
  assert(g.solve() == 1 && g.solve() == 0);
  g.add_edge(0, 1);
  assert(g.solve() == 1 && g.solve() == 0);
  for (int u = 0; u < n; ++u) {
    int want = u < 2 || u >= n - 2 ? (u ^ 1) : n;
    assert(g.match[u] == want);
  }

  // Insertion preserves the order of existing equal-key nodes.
  mt19937 rng(1818);
  for (int mode = 0; mode < 2; ++mode) {
    vector<int> keys(200000);
    for (int &x : keys) x = mode ? 7 : int(rng() % 1000) - 500;
    vector<pii> want;
    srand(1818);
    for (int x : keys) want.pb({x, rand()});
    stable_sort(ALL(want), [](pii a, pii b) { return a.X < b.X; });
    node *root = nullptr;
    srand(1818);
    for (int x : keys) insert(root, x);
    vector<pii> got; collect(root, got);
    assert(got == want && sz(root) == 200000);
    for (int i = 0; i < 200000; i += 997)
      assert(kth(root, i + 1)->data == want[i].X);
    for (int x : keys) assert(erase(root, x));
    assert(!root);
  }
  cout << "PASS round18 isolates/reuse and stable treap insertion\n";
}
