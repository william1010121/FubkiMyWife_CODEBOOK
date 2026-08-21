vector<int> vG[N];
int top, st[N];
void insert(int u) {
  if (top == -1) return st[++top] = u, void();
  int p = LCA(st[top], u);
  if (p == st[top]) return st[++top] = u, void();
  while (top >= 1 && dep[st[top - 1]] >= dep[p])
    vG[st[top - 1]].pb(st[top]), --top;
  if (st[top] != p)
    vG[p].pb(st[top]), --top, st[++top] = p;
  st[++top] = u;
}

void reset(int u) {
  for (int i : vG[u]) reset(i); vG[u].clear();
}
int build(vector<int> &v) {
  if (v.empty()) return -1;
  top = -1;
  sort(ALL(v), [&](int a, int b) { return dfn[a] < dfn[b]; });
  v.erase(unique(ALL(v)), v.end());
  for (int i : v) insert(i);
  while (top > 0) vG[st[top - 1]].pb(st[top]), --top;
  return st[0];
}
void solve(vector<int> &v) {
  int root = build(v);
  if (root == -1) return;
  // do something
  reset(root);
}
