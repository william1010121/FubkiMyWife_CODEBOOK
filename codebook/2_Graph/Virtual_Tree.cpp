// Rooted tree: LCA, preorder dfn, dep; IDs [0,N).
vector<int> vG[N]; int top, st[N];
void insert(int u) { // top=-1 first; unique preorder keys
  if (top == -1) { st[++top] = u; return; }
  int p = LCA(st[top], u);
  while (top >= 1 && dep[st[top - 1]] >= dep[p])
    vG[st[top - 1]].pb(st[top]), --top;
  if (st[top] != p) vG[p].pb(st[top]), st[top] = p;
  st[++top] = u;
}
void reset(int u) { // valid root; enough recursion stack
  for (int i : vG[u]) reset(i); vG[u].clear();
}
// Clear previous vG first; build O(k log k+k*T_LCA).
int build(vector<int> &v) {
  if (v.empty()) return -1;
  top = -1;
  sort(ALL(v), [&](int a, int b) {
    return dfn[a] < dfn[b]; });
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
