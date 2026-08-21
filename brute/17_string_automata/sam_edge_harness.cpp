#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/5_String/SAM.cpp"

static void fail(const string &message) {
  cerr << "FAIL sam-edge: " << message << '\n';
  exit(1);
}

static map<string, int> brute_occ(const string &s) {
  map<string, int> result;
  for (int l = 0; l < (int)s.size(); ++l)
    for (int r = l + 1; r <= (int)s.size(); ++r)
      ++result[s.substr(l, r - l)];
  return result;
}

static void check(const string &s) {
  sam.init();
  for (char c : s) sam.push(c - 'a' + 1);
  sam.count();
  if (sam.cnt[sam.root] != (int)s.size())
    fail("root occurrence count mismatch for length " + to_string(s.size()));

  const auto want = brute_occ(s);
  long long distinct = 0;
  for (int v = 1; v <= sam.tot; ++v) {
    if (v == sam.root) continue;
    if (sam.mom[v] <= 0 || sam.mom[v] > sam.tot ||
        sam.mx[sam.mom[v]] >= sam.mx[v])
      fail("invalid suffix-link length invariant");
    distinct += sam.mx[v] - sam.mx[sam.mom[v]];
  }
  if (distinct != (long long)want.size())
    fail("distinct-substring count mismatch for length " + to_string(s.size()));

  for (const auto &[sub, expected] : want) {
    int v = sam.root;
    for (char c : sub) {
      v = sam.nxt[v][c - 'a' + 1];
      if (!v) fail("missing transition for substring " + sub);
    }
    if (sam.cnt[v] != expected)
      fail("occurrence mismatch for substring " + sub);
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
      "", "a", "b", "ab", "ba", "aa", "aaa", "abababab",
      "abcabcabc", "abacabadabacaba", string(80, 'a'),
      string("ab").append(39, 'a'), string("abc").append(25, 'c')};
  for (const string &s : special) check(s), ++cases;

  mt19937 rng(0x51A7EEDu);
  for (int tc = 0; tc < 700; ++tc) {
    const int n = rng() % 81;
    const int alphabet = 1 + rng() % 4;
    string s(n, 'a');
    for (char &c : s) c = char('a' + rng() % alphabet);
    check(s), ++cases;
  }
  cout << "PASS sam-edge cases=" << cases
       << " oracle=all-substring-occurrences\n";
}
