// All distinct LCS in lexical order; DP O(nm), output may be exponential.
// Reconstruction merges sorted lists, linear in their string data.
vector<string> all_lcs(const string &s, const string &t) {
  int n = (int)s.size(), m = (int)t.size();
  vector<vector<int>> dp(n + 1, vector<int>(m + 1));
  for (int i = n - 1; i >= 0; --i)
    for (int j = m - 1; j >= 0; --j)
      dp[i][j] = s[i] == t[j] ? 1 + dp[i + 1][j + 1]
        : max(dp[i + 1][j], dp[i][j + 1]);
  vector<vector<vector<string>>> memo(n + 1,
    vector<vector<string>>(m + 1));
  auto go = [&](auto &&self, int i, int j) -> vector<string> & {
    auto &ret = memo[i][j];
    if (!ret.empty()) return ret; // Even an empty LCS gives {""}.
    if (!dp[i][j]) return ret.push_back(""), ret;
    if (s[i] == t[j]) {
      for (const string &x : self(self, i + 1, j + 1))
        ret.push_back(s[i] + x);
    } else if (dp[i + 1][j] > dp[i][j + 1]) ret = self(self, i + 1, j);
    else if (dp[i + 1][j] < dp[i][j + 1]) ret = self(self, i, j + 1);
    else {
      auto &a = self(self, i + 1, j), &b = self(self, i, j + 1);
      set_union(ALL(a), ALL(b), back_inserter(ret));
    } return ret;
  };
  return move(go(go, 0, 0));
}
