int z[MAXN]; // O(n); 0-base; MAXN >= 2*SZ(tmp)+1
// Center i: z[2*i+1]/2; gap i,i+1: z[2*i+2]/2.
// Both aba and abba have radius 2.
void Manacher(const string &tmp) {
  string s = "%"; s.reserve(2 * SZ(tmp) + 1);
  for (char c : tmp) s.pb(c), s.pb('%');
  for (int i = 0, l = 0, r = 0; i < SZ(s); ++i) {
    z[i] = r > i ? min(z[2 * l - i], r - i) : 1;
    while (i - z[i] >= 0 && i + z[i] < SZ(s) &&
      s[i + z[i]] == s[i - z[i]]) ++z[i];
    if (z[i] + i > r) r = z[i] + i, l = i; }
}
