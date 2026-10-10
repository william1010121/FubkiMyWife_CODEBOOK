#include <bits/stdc++.h>
using namespace std;
const int N = 200005;
#define pb push_back
#include "../../codebook/3_Data_Structure/Dynamic_2D_Segment_Tree.cpp"
#include "../../codebook/3_Data_Structure/Heavy_light_Decomposition.cpp"

static Heavy_light_Decomposition h;

int invariant(Seg::Node *o, int l, int r) {
  if (!o) return 0;
  int m = l + (r - l) / 2, sum = 0;
  if (r - l > 1)
    sum = invariant(o->l, l, m) + invariant(o->r, m, r);
  assert(o->sum == sum + o->lazy * (r - l));
  return o->sum;
}

int main() {
  const int n = 200000;
  mt19937 rng(0x601d);
  for (int mode = 0; mode < 4; ++mode) {
    h.init(n);
    vector<int> p(n + 1), d(n + 1), v(n + 1);
    d[1] = 1;
    for (int u = 2; u <= n; ++u) {
      p[u] = mode & 1 ? 1 : u / 2;
      d[u] = d[p[u]] + 1, h.add_edge(p[u], u);
    }
    for (int u = 1; u <= n; ++u)
      h.val[u] = v[u] = mode < 2 ? u % 3 - 1 : 0;
    h.build();
    invariant(h.seg.root, 0, n);
    if (mode >= 2) assert(!h.seg.root);
    for (int u = 1; u <= n; ++u)
      assert(h.query(u, u) == v[u]);
    h.seg.update(0, n, 2);
    for (int q = 0; q < n; ++q) {
      int a = 1 + rng() % n, b = 1 + rng() % n;
      int x = a, y = b, want = 0;
      while (x != y) {
        if (d[x] < d[y]) swap(x, y);
        want += v[x] + 2, x = p[x];
      }
      want += v[x] + 2;
      assert(h.query(a, b) == want);
    }
    invariant(h.seg.root, 0, n);
  }
  cout << "hld scale: PASS\n";
}
