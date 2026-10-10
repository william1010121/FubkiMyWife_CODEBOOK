vector<int> vG[N]; int top, st[N]; // needs LCA, dfn, dep
void insert(int u) {
  if (top == -1) { st[++top] = u; return; }
  int p = LCA(st[top], u);
  if (p == st[top]) { st[++top] = u; return; }
  while (top >= 1 && dep[st[top - 1]] >= dep[p])
    vG[st[top - 1]].pb(st[top]), --top;
  if (st[top] != p) vG[p].pb(st[top]), st[top] = p;
  st[++top] = u;
}
void reset(int u) {
  for (int i : vG[u]) reset(i); vG[u].clear();
}
int build(vector<int> &v) { // O(k log k + k * LCA cost)
  if (v.empty()) return -1;
  top = -1;
  sort(ALL(v), [&](int a, int b) { return dfn[a] < dfn[b]; });
  v.erase(unique(ALL(v)), v.end());
  for (int i : v) insert(i);
  while (top > 0) vG[st[top - 1]].pb(st[top]), --top;
  return st[0];
}
void solve(vector<int> &v) {
  int root = build(v); if (root == -1) return;
  // do something
  reset(root);
}
