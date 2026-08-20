#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define SZ(x) static_cast<int>((x).size())
#include "../../../codebook/5_String/PalTree.cpp"

static bool palindrome(const string &s, int l, int r) {
  while (l < r) if (s[l++] != s[r--]) return false;
  return true;
}

static map<string, int> brute_occ(const string &s, vector<int> &ending) {
  map<string, int> occ;
  ending.assign(s.size(), 0);
  for (int l = 0; l < (int)s.size(); ++l)
    for (int r = l; r < (int)s.size(); ++r)
      if (palindrome(s, l, r)) ++occ[s.substr(l, r - l + 1)], ++ending[r];
  return occ;
}

static void check(palindromic_tree &pt, const string &s) {
  vector<int> ending;
  const map<string, int> want = brute_occ(s, ending);
  pt.clear();
  for (int i = 0; i < (int)s.size(); ++i) {
    pt.add(s[i]);
    if (pt.St[pt.last].num != ending[i])
      throw runtime_error("PalTree suffix-palindrome mismatch");
  }
  if (pt.size() != (int)want.size())
    throw runtime_error("PalTree distinct-palindrome mismatch");
  pt.count();
  vector<int> got, expected;
  for (int v = 2; v < SZ(pt.St); ++v) got.push_back(pt.St[v].cnt);
  for (const auto &[pal, count] : want) (void)pal, expected.push_back(count);
  sort(got.begin(), got.end());
  sort(expected.begin(), expected.end());
  if (got != expected) throw runtime_error("PalTree occurrence mismatch");
}

static void check_encrypted_problem(palindromic_tree &pt, const string &original) {
  vector<int> want;
  (void)brute_occ(original, want);
  string encrypted = original;
  for (int i = 1; i < (int)original.size(); ++i)
    encrypted[i] = char('a' + (original[i] - 'a' - want[i - 1] + 52) % 26);
  pt.clear();
  int previous = 0;
  for (int i = 0; i < (int)encrypted.size(); ++i) {
    const char decoded = char('a' + (encrypted[i] - 'a' + previous) % 26);
    pt.add(decoded);
    if (pt.St[pt.last].num != want[i])
      throw runtime_error("PalTree encrypted-input decode mismatch");
    previous = pt.St[pt.last].num;
  }
}

int main() {
  try {
    palindromic_tree pt;
    check(pt, "aaaaaa");
    check(pt, "abababa");
    check(pt, "abacaba");
    check_encrypted_problem(pt, "aabaabbaaa");
    for (int n = 0; n <= 10; ++n)
      for (int mask = 0; mask < (1 << n); ++mask) {
        string s(n, 'a');
        for (int i = 0; i < n; ++i)
          if (mask >> i & 1) s[i] = 'b';
        check(pt, s);
      }
    cout << "PASS paltree cases=2050 oracle=palindromic-suffixes-and-frequencies\n";
  } catch (const exception &e) {
    cerr << "FAIL paltree: " << e.what() << '\n';
    return 1;
  }
}
