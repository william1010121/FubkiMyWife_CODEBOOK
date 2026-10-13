int z[MAXN]; // O(n); 0-base; MAXN >= 2*|tmp|+1
// Center i: z[2*i+1]/2; gap i,i+1: z[2*i+2]/2.
// Index arithmetic fits int; aba, abba have radius 2.
void Manacher(const string &tmp) {
  string s(2 * SZ(tmp) + 1, '%');
  for (int i = 0; i < SZ(tmp); ++i) s[2 * i + 1] = tmp[i];
  for (int i = 0, l = 0, r = 0; i < SZ(s); ++i) {
    z[i] = r > i ? min(z[2 * l - i], r - i) : 1;
    while (i - z[i] >= 0 && i + z[i] < SZ(s) &&
      s[i + z[i]] == s[i - z[i]]) ++z[i];
    if (z[i] + i > r) r = z[i] + i, l = i; }
}
