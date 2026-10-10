vector<vector<int>> a(n, vector<int>(3));  // n points, k = 3 dims
auto c = dominance(a);  // c[i] = #{j != i : a[j] <= a[i] in every dim}
vector<int> level(n);   // e.g. "how many points have exactly d below"
for (int i = 0; i < n; ++i) ++level[c[i]];
