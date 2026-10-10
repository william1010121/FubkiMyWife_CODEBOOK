// All distinct LCS, lexicographically sorted. DP O(nm), output may grow
// exponentially; reconstruction merges sorted lists in linear time.
vector<string> all_lcs(const string &s, const string &t) {
  int n = (int)s.size(), m = (int)t.size();
  vector<vector<int>> dp(n + 1, vector<int>(m + 1));
  for (int i = n - 1; i >= 0; --i)
    for (int j = m - 1; j >= 0; --j)
      dp[i][j] = s[i] == t[j] ? 1 + dp[i + 1][j + 1]
        : max(dp[i + 1][j], dp[i][j + 1]);
  vector<vector<vector<string>>> memo(n + 1,
    vector<vector<string>>(m + 1));
  vector<vector<char>> seen(n + 1, vector<char>(m + 1));
  auto go = [&](auto &&self, int i, int j) -> vector<string> & {
    if (seen[i][j]) return memo[i][j];
    seen[i][j] = 1;
    auto &ret = memo[i][j];
    if (!dp[i][j]) return ret.push_back(""), ret;
    if (s[i] == t[j]) {
      for (const string &x : self(self, i + 1, j + 1))
        ret.push_back(s[i] + x);
    } else {
      if (dp[i + 1][j] == dp[i][j]) ret = self(self, i + 1, j);
      if (dp[i][j + 1] == dp[i][j]) {
        auto &b = self(self, i, j + 1);
        vector<string> merged;
        set_union(ret.begin(), ret.end(), b.begin(), b.end(),
                  back_inserter(merged));
        ret = move(merged);
      }
    } return ret;
  };
  return go(go, 0, 0);
}
