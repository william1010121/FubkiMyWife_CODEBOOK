#include <bits/stdc++.h>
using namespace std;

const int kN = 64;
static vector<int> Zalgo(const string &s) {
  vector<int> z(s.size());
  if (s.empty()) return z;
  z[0] = (int)s.size();
  for (int i = 1, l = 0, r = 0; i < (int)s.size(); ++i) {
    if (i <= r) z[i] = min(r - i + 1, z[i - l]);
    while (i + z[i] < (int)s.size() && s[z[i]] == s[i + z[i]]) ++z[i];
    if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
  }
  return z;
}
#include "../../../codebook/5_String/MainLorentz.cpp"

static set<pair<int, int>> brute(const string &s) {
  set<pair<int, int>> ans;
  for (int l = 1; 2 * l <= (int)s.size(); ++l)
    for (int p = 0; p + 2 * l <= (int)s.size(); ++p)
      if (s.compare(p, l, s, p + l, l) == 0) ans.emplace(l, p);
  return ans;
}

static void check(const string &s) {
  for (auto &v : rep) v.clear();
  main_lorentz(s);
  set<pair<int, int>> got;
  for (int l = 1; l <= (int)s.size() / 2; ++l)
    for (const auto &[left, right] : rep[l]) {
      if (left < 0 || right < left || right + 2 * l > (int)s.size())
        throw runtime_error("Main-Lorentz range out of bounds");
      for (int p = left; p <= right; ++p) {
        if (!got.emplace(l, p).second)
          throw runtime_error("Main-Lorentz duplicate report");
        if (s.compare(p, l, s, p + l, l) != 0)
          throw runtime_error("Main-Lorentz false tandem repeat");
      }
    }
  if (got != brute(s)) throw runtime_error("Main-Lorentz missing tandem repeat");
}

int main() {
  try {
    check("");
    check("aaaaaa");
    check("abababa");
    check("abacaba");
    check("##");
    check(string("\0#\0#", 4));
    for (int n = 1; n <= 14; ++n)
      for (int mask = 0; mask < (1 << n); ++mask) {
        string s(n, 'a');
        for (int i = 0; i < n; ++i)
          if (mask >> i & 1) s[i] = 'b';
        check(s);
      }
    cout << "PASS main_lorentz cases=32771 oracle=all-squares\n";
  } catch (const exception &e) {
    cerr << "FAIL main_lorentz: " << e.what() << '\n';
    return 1;
  }
}
