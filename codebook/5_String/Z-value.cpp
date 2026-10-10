int z[MAXn]; // O(n); z[0] unchanged; |s| <= min(MAXn, INT_MAX)
void make_z(const string &s) {
  for (int i = 1, l = 0, r = 0; i < SZ(s); ++i) {
    z[i] = i < r ? min(r - i, z[i - l]) : 0;
    while (i + z[i] < SZ(s) && s[i + z[i]] == s[z[i]]) ++z[i];
    if (i + z[i] > r) l = i, r = i + z[i]; } }
