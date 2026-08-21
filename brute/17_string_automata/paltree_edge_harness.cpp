#include <bits/stdc++.h>
using namespace std;

#define pb push_back
#define SZ(x) static_cast<int>((x).size())
#include "../../codebook/5_String/PalTree.cpp"

static void fail(const string &message) {
  cerr << "FAIL paltree-edge: " << message << '\n';
  exit(1);
}

static bool is_palindrome(const string &s, int l, int r) {
  while (l < r)
    if (s[l++] != s[r--]) return false;
  return true;
}

static map<string, int> brute_occ(const string &s, vector<int> &ending,
                                  vector<int> &longest) {
  map<string, int> result;
  ending.assign(s.size(), 0);
  longest.assign(s.size(), 0);
  for (int l = 0; l < (int)s.size(); ++l)
    for (int r = l; r < (int)s.size(); ++r)
      if (is_palindrome(s, l, r)) {
        ++result[s.substr(l, r - l + 1)];
        ++ending[r];
        longest[r] = max(longest[r], r - l + 1);
      }
  return result;
}

static string node_string(const palindromic_tree &pt, int v,
                          vector<string> &memo, vector<char> &done) {
  if (done[v]) return memo[v];
  done[v] = true;
  if (v == 0 || v == 1) return memo[v];

  int parent = -1, edge = -1;
  for (int p = 0; p < SZ(pt.St); ++p)
    for (int c = 0; c < 26; ++c)
      if (pt.St[p].next[c] == v) {
        if (parent != -1) fail("palindrome node has multiple parents");
        parent = p;
        edge = c;
      }
  if (parent == -1) fail("palindrome node has no parent");
  if (pt.St[parent].len == -1)
    memo[v] = string(1, char('a' + edge));
  else {
    const string inner = node_string(pt, parent, memo, done);
    memo[v] = char('a' + edge) + inner + char('a' + edge);
  }
  return memo[v];
}

static void check(const string &s) {
  palindromic_tree pt;
  pt.clear();
  vector<int> ending, longest;
  const auto want = brute_occ(s, ending, longest);
  for (int i = 0; i < (int)s.size(); ++i) {
    pt.add(s[i]);
    if (pt.St[pt.last].num != ending[i])
      fail("palindromic suffix count mismatch");
    if (pt.St[pt.last].len != longest[i])
      fail("longest palindromic suffix mismatch");
  }
  if (pt.size() != (int)want.size())
    fail("distinct-palindrome count mismatch");

  vector<string> memo(pt.St.size());
  vector<char> done(pt.St.size(), false);
  for (int v = 2; v < SZ(pt.St); ++v) {
    const string p = node_string(pt, v, memo, done);
    if ((int)p.size() != pt.St[v].len || want.find(p) == want.end() ||
        !is_palindrome(p, 0, (int)p.size() - 1))
      fail("palindrome node reconstruction mismatch");
  }

  pt.count();
  for (int v = 2; v < SZ(pt.St); ++v) {
    const string p = node_string(pt, v, memo, done);
    if (pt.St[v].cnt != want.at(p))
      fail("palindrome occurrence count mismatch for " + p);
  }
}

static void enumerate_words(int alphabet, int length,
                            const function<void(const string &)> &visit) {
  long long total = 1;
  for (int i = 0; i < length; ++i) total *= alphabet;
  for (long long code = 0; code < total; ++code) {
    long long x = code;
    string s(length, 'a');
    for (int i = 0; i < length; ++i) {
      s[i] = char('a' + x % alphabet);
      x /= alphabet;
    }
    visit(s);
  }
}

int main() {
  int cases = 0;
  for (int n = 0; n <= 11; ++n)
    enumerate_words(2, n, [&](const string &s) { check(s), ++cases; });
  for (int n = 0; n <= 7; ++n)
    enumerate_words(3, n, [&](const string &s) { check(s), ++cases; });

  const vector<string> special = {
      "", "a", "aa", "aba", "abba", "abacaba", "abababa",
      "aabbaa", "abcddcba", string(70, 'a'), "abcabcabcabc"};
  for (const string &s : special) check(s), ++cases;

  mt19937 rng(0x0A17EEDu);
  for (int tc = 0; tc < 600; ++tc) {
    const int n = rng() % 51;
    const int alphabet = 1 + rng() % 4;
    string s(n, 'a');
    for (char &c : s) c = char('a' + rng() % alphabet);
    check(s), ++cases;
  }
  cout << "PASS paltree-edge cases=" << cases
       << " oracle=all-palindromic-substrings-and-suffixes\n";
}
