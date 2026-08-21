using tree_hash_u64 = unsigned long long;
constexpr tree_hash_u64 tree_hash_default_seed =
  0x9e3779b97f4a7c15ULL;

tree_hash_u64 tree_hash_shift(tree_hash_u64 x) {
  x ^= x << 13, x ^= x >> 7, x ^= x << 17;
  return x;
}

tree_hash_u64 tree_hash_dfs(
  const vector<vector<int>> &g, int u, int parent,
  tree_hash_u64 seed) {
  if (!seed) seed = tree_hash_default_seed;
  tree_hash_u64 sum = seed;
  for (int v : g[u]) if (v != parent)
    sum += tree_hash_shift(tree_hash_dfs(g,
      v, u, seed));
  return sum;
}

tree_hash_u64 rooted_tree_hash(
  const vector<vector<int>> &g, int root = 0,
  tree_hash_u64 seed = tree_hash_default_seed) {
  return tree_hash_dfs(g, root, -1, seed);
}
