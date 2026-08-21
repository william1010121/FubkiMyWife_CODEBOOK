int n = s.size(), m = t.size(); vector<int> nxt(m);
if (!m) for (int i = 0; i <=
  n; ++i) cout << i + 1 << endl;
else {
  for (int i = 1, j = 0; i < m; i++) {
	  while (j && t[i] != t[j]) j = nxt[j - 1];
	  if (t[i] == t[j]) j++; nxt[i] = j;
  } for (int i = 0, j = 0; i < n; i++) {
    while (j && s[i] != t[j]) j = nxt[j - 1];
  if (s[i] == t[j]) j++;
  if (j == m) cout << i - m + 2 << endl,
    j = nxt[j - 1];
  }
}
