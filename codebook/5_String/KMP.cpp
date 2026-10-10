// 1-base matches; empty t: all n+1 boundaries; |s|+1, |t| fit int.
int n = s.size(), m = t.size(); vector<int> nxt(m); // O(n+m)
if (!m) for (int i = 0; i <= n; ++i) cout << i + 1 << '\n';
else {
  for (int i = 1, j = 0; i < m; i++) {
    while (j && t[i] != t[j]) j = nxt[j - 1];
    nxt[i] = (j += t[i] == t[j]);
  } for (int i = 0, j = 0; i < n; i++) {
    while (j && s[i] != t[j]) j = nxt[j - 1];
    j += s[i] == t[j];
    if (j == m) cout << i - m + 2 << '\n', j = nxt[j - 1];
  }
}
