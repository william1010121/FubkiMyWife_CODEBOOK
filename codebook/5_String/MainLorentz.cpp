vector<pair<int, int>> rep[kN]; // 0-base [l, r]
void main_lorentz_impl(const string &s, int sft) {
	const int n = s.size(); if (n <= 1) return;
  const int nu = n / 2, nv = n - nu;
	const string u = s.substr(0, nu), v = s.substr(nu),
	  ru(u.rbegin(), u.rend()), rv(v.rbegin(), v.rend());
	main_lorentz_impl(u, sft), main_lorentz_impl(v, sft + nu);
  auto join = [](const string &a, const string &b) {
    vector<int> x; x.reserve(a.size() + b.size() + 1);
    for (unsigned char c : a) x.push_back(c);
    x.push_back(256);
    for (unsigned char c : b) x.push_back(c); return x;
  };
  auto zalgo = [](const vector<int> &a) {
    vector<int> z(a.size());
    for (int i = 1, l = 0,
      r = 0; i < (int)a.size(); ++i) {
      if (i <= r) z[i] = min(r - i + 1, z[i - l]);
      while (i + z[i] < (int)a.size() &&
        a[z[i]] == a[i + z[i]]) ++z[i];
      if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    } return z;
  };
	const auto z1 = zalgo(join(ru, "")),
	  z2 = zalgo(join(v, u)), z3 = zalgo(join(ru,
	  rv)), z4 = zalgo(join(v, ""));
	auto get_z = [](const vector<int> &z, int i) {
	  return (0 <= i and i < (int)z.size()) ? z[i] : 0;
	  };
  auto add_rep = [&](bool left,
    int c, int l, int k1, int k2) {
		const int L = max(1, l - k2),
		  R = min(l - left, k1);
		if (L > R) return; int d = !left * (l - 1);
		rep[l].emplace_back(sft + c - R - d,
		  sft + c - L - d);
  };
  for (int cntr = 0; cntr < n; cntr++) {
    int l, k1, k2;
    if (cntr < nu) {
		l = nu - cntr, k1 = get_z(z1, nu - cntr),
		  k2 = get_z(z2, nv + 1 + cntr);
    } else {
        l = cntr - nu + 1, k1 = get_z(z3, n + nu -
          cntr), k2 = get_z(z4, cntr - nu + 1);
    } if (k1 + k2 >= l) add_rep(cntr < nu,
      cntr, l, k1, k2);
  }
}

// Each public call starts a fresh report; recursive work uses the helper so
// that ranges found in earlier subproblems are retained.
void main_lorentz(const string &s, int sft = 0) {
  for (auto &v : rep) v.clear();
  main_lorentz_impl(s, sft);
}
// p \in [l, r] => s[p, p + i) = s[p + i, p + 2i)
