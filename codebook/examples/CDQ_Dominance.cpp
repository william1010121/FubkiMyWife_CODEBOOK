vector<vector<int>> a(n, vector<int>(3)); // n 3D points
auto c = dominance(a); // Other points <= in all dims.
vector<int> level(n); // Histogram of dominance counts
for (int i = 0; i < n; ++i) ++level[c[i]];
