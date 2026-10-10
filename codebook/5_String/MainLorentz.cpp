// rep[l]: 0-based inclusive starts of period-l squares; n/2<kN
// n and sft+p (0<=p<n) fit int; empty input and all bytes allowed.
vector<pair<int, int>> rep[kN];
void main_lorentz_impl(const string &s, int sft) {
  int n = s.size(); if (n <= 1) return;
  if (n == 2) { if (s[0] == s[1]) rep[1].emplace_back(sft, sft); return; }
  int nu = n/2, nv = n-nu;
  string u = s.substr(0, nu), v = s.substr(nu),
         ru(u.rbegin(), u.rend()), rv(v.rbegin(), v.rend());
  main_lorentz_impl(u, sft), main_lorentz_impl(v, sft+nu);
  auto zalgo = [](const string &a) {
    int n = a.size(); vector<int> z(a.size()+1); // z[n]=0 at boundary
    for (int i = 1, l = 0, r = 0; i < n; ++i) {
      if (i <= r) z[i] = min(r-i+1, z[i-l]);
      while (i+z[i] < n && a[z[i]] == a[i+z[i]]) ++z[i];
      if (i+z[i]-1 > r) l = i, r = i+z[i]-1;
    } return z;
  };
  // No separator: u is no longer than v; cap ru matches at nu.
  auto z1 = zalgo(ru), z2 = zalgo(v+u),
       z3 = zalgo(ru+rv), z4 = zalgo(v);
  for (int c = 0; c < n; ++c) {
    bool left = c < nu; int l, k1, k2;
    if (left) l = nu-c, k1 = z1[l], k2 = z2[nv+c];
    else l = c-nu+1, k1 = min(nu, z3[n-l]), k2 = z4[l];
    int L = max(1, l-k2), R = min(l-left, k1), d = left ? 0 : l-1;
    if (L <= R) rep[l].emplace_back(sft+c-R-d, sft+c-L-d);
  }
}
void main_lorentz(const string &s, int sft = 0) { // fresh report
  for (auto &v : rep) v.clear();
  main_lorentz_impl(s, sft); }
// O(n log n + kN); p in rep[l] => s[p,p+l)==s[p+l,p+2l)
