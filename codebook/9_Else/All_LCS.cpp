// Return every distinct longest common subsequence, in
// lexicographic order.
vector<string> all_lcs(const string &s,
  const string &t) {
  int n = (int)s.size(), m = (int)t.size();
  vector<vector<int>> dp(n + 1, vector<int>(m + 1));
  for (int i = n - 1; i >= 0; --i)
    for (int j = m - 1; j >= 0; --j)
      dp[i][j] = s[i] == t[j] ? 1 + dp[i + 1][j + 1]
        : max(dp[i + 1][j], dp[i][j + 1]);
  vector<vector<vector<string>>> memo(n + 1,
    vector<vector<string>>(m + 1));
  vector<vector<char>> seen(n + 1,
    vector<char>(m + 1));
  auto go = [&](auto &&self, int i,
    int j) -> vector<string> & {
    if (seen[i][j]) return memo[i][j];
    seen[i][j] = 1;
    auto &ret = memo[i][j];
    if (!dp[i][j]) return ret.push_back(""), ret;
    if (i < n && j < m && s[i] == t[j] &&
        dp[i][j] == 1 + dp[i + 1][j + 1]) {
      for (string x : self(self, i + 1,
        j + 1)) ret.push_back(s[i] + x);
    } else {
      if (i < n && dp[i + 1][j] == dp[i][j])
        for (string x : self(self,
          i + 1, j)) ret.push_back(x);
      if (j < m && dp[i][j + 1] == dp[i][j])
        for (string x : self(self, i,
          j + 1)) ret.push_back(x);
    }
    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(),
      ret.end()), ret.end());
    return ret;
  };
  return go(go, 0, 0);
}
