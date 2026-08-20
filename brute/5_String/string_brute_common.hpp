#pragma once

#include <bits/stdc++.h>
using namespace std;

namespace string_brute {

inline mt19937 rng(0x5EED1234u);

inline string random_string(int n, int alphabet = 4) {
  string s(n, 'a');
  for (char &c : s) c = char('a' + int(rng() % alphabet));
  return s;
}

inline void expect(bool ok, const string &message) {
  if (!ok) {
    cerr << "FAIL: " << message << '\n';
    exit(1);
  }
}

inline vector<int> brute_sa(const string &s) {
  vector<int> sa(s.size());
  iota(sa.begin(), sa.end(), 0);
  sort(sa.begin(), sa.end(), [&](int a, int b) {
    return s.substr(a) < s.substr(b);
  });
  return sa;
}

inline vector<int> brute_lcp(const string &s, const vector<int> &sa) {
  vector<int> h(sa.size());
  for (int i = 1; i < (int)sa.size(); ++i) {
    while (sa[i] + h[i] < (int)s.size() &&
           sa[i - 1] + h[i] < (int)s.size() &&
           s[sa[i] + h[i]] == s[sa[i - 1] + h[i]]) ++h[i];
  }
  return h;
}

inline vector<int> brute_z(const string &s) {
  vector<int> z(s.size());
  for (int i = 1; i < (int)s.size(); ++i)
    while (i + z[i] < (int)s.size() && s[z[i]] == s[i + z[i]]) ++z[i];
  return z;
}

inline string brute_min_rotation(const string &s) {
  string ans = s;
  for (int i = 1; i < (int)s.size(); ++i)
    ans = min(ans, s.substr(i) + s.substr(0, i));
  return ans;
}

inline string transformed(const string &s) {
  string t = "%";
  for (char c : s) {
    t.push_back(c);
    t.push_back('%');
  }
  return t;
}

inline vector<int> brute_manacher_radii(const string &s) {
  string t = transformed(s);
  vector<int> z(t.size());
  for (int i = 0; i < (int)t.size(); ++i) {
    z[i] = 1;
    while (i - z[i] >= 0 && i + z[i] < (int)t.size() &&
           t[i - z[i]] == t[i + z[i]]) ++z[i];
  }
  return z;
}

inline bool valid_de_bruijn(const vector<int> &out, int c, int n) {
  if (c <= 0 || n <= 0 || c > 10 || n > 8) return false;
  long long k = 1;
  for (int i = 0; i < n; ++i) k *= c;
  if ((long long)out.size() != n + k - 1) return false;
  vector<int> seen((size_t)k);
  for (int i = 0; i < (int)out.size(); ++i)
    if (out[i] < 0 || out[i] >= c) return false;
  for (int i = 0; i < k; ++i) {
    int code = 0;
    for (int j = 0; j < n; ++j) code = code * c + out[i + j];
    if (seen[code]++) return false;
  }
  // This is the brute-force oracle condition: all c^n possible words
  // occur exactly once among the c^n linear windows.
  return all_of(seen.begin(), seen.end(), [](int x) { return x == 1; });
}

}  // namespace string_brute
