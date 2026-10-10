// rep[l]: 0-based inclusive starts of period-l squares; n/2<kN
// n and sft+p (0<=p<n) fit int; empty/all bytes OK; C++17.
vector<pair<int, int>> rep[kN];
void main_lorentz_impl(const string &s, int sft) {
  auto rec = [](auto &&self, string_view s, int sft) -> void {
    int n = s.size(), nu = n/2, nv = n-nu; if (n <= 1) return;
    if (n == 2) { if (s[0] == s[1]) rep[1].push_back({sft,sft}); return; }
    string_view u = s.substr(0, nu), v = s.substr(nu);
    self(self, u, sft), self(self, v, sft+nu);
    auto zalgo = [](const string &a) {
      int n = a.size(); vector<int> z(n);
      for (int i = 1, l = 0, r = 0; i < n; ++i) {
        if (i <= r) z[i] = min(r-i+1, z[i-l]);
        while (i+z[i] < n && a[z[i]] == a[i+z[i]]) ++z[i];
        if (i+z[i]-1 > r) l = i, r = i+z[i]-1;
      } return z; };
    // |u|<=|v|; two Z arrays with matches capped at half boundaries.
    string a(v); a += u; auto z = zalgo(a); reverse(ALL(a));
    auto rz = zalgo(a); for (int c = 0; c < n; ++c) {
      bool left = c < nu; int l, k1, k2;
      if (left) l = nu-c, k1 = min(c, rz[l]), k2 = z[nv+c];
      else l = c-nu+1, k1 = min(nu, rz[n-l]), k2 = min(nv-l, z[l]);
      int L = max(1, l-k2), R = min(l-left, k1), d = left ? 0 : l-1;
      if (L <= R) rep[l].emplace_back(sft+c-R-d, sft+c-L-d);
    }
  }; rec(rec, s, sft);
}
void main_lorentz(const string &s, int sft = 0) { // fresh report
  for (auto &v : rep) v.clear();
  main_lorentz_impl(s, sft); }
// O(n log n + kN); p in rep[l] => s[p,p+l)==s[p+l,p+2l)
