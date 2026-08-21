#include <bits/stdc++.h>
using namespace std;

const int N = 512;
const int CNUM = 3;
#define ALL(x) (x).begin(), (x).end()
#include "../../codebook/5_String/exSAM.cpp"

static void fail(const string &message) {
  cerr << "FAIL exsam-edge: " << message << '\n';
  exit(1);
}

static map<string, int> brute_occ(const vector<string> &strings) {
  map<string, int> result;
  for (const string &s : strings)
    for (int l = 0; l < (int)s.size(); ++l)
      for (int r = l + 1; r <= (int)s.size(); ++r)
        ++result[s.substr(l, r - l)];
  return result;
}

// A generalized SAM has one state per distinct end-position set.  Keep the
// document id in each endpoint so occurrences in different input strings do
// not accidentally merge.
static int brute_endpos_states(const vector<string> &strings) {
  map<string, set<pair<int, int>>> endpos;
  for (int id = 0; id < (int)strings.size(); ++id) {
    const string &s = strings[id];
    for (int l = 0; l < (int)s.size(); ++l)
      for (int r = l + 1; r <= (int)s.size(); ++r) {
        const string sub = s.substr(l, r - l);
        endpos[sub].insert({id, r - 1});
      }
  }
  set<set<pair<int, int>>> classes;
  for (const auto &[sub, positions] : endpos) (void)sub, classes.insert(positions);
  return (int)classes.size() + 1;  // include the root state
}

static void check(const vector<string> &strings) {
  exSAM ex;
  ex.init();
  for (const string &s : strings) ex.insert(s);
  ex.build();
  ex.solve();

  const auto want = brute_occ(strings);
  long long distinct = 0;
  for (int v = 1; v < ex.tot; ++v) {
    if (ex.link[v] < 0 || ex.link[v] >= ex.tot ||
        ex.len[ex.link[v]] >= ex.len[v])
      fail("invalid suffix-link invariant");
    distinct += ex.len[v] - ex.len[ex.link[v]];
  }
  if (distinct != (long long)want.size())
    fail("distinct-substring count mismatch");
  const int expected_states = brute_endpos_states(strings);
  if (ex.tot != expected_states)
    fail("end-position state count mismatch: got " +
         to_string(ex.tot) + " expected " + to_string(expected_states) +
         " for list size " + to_string(strings.size()) +
         (strings.empty() ? string() : " first=" + strings.front()));

  for (const auto &[sub, expected] : want) {
    int v = 0;
    for (char c : sub) {
      v = ex.next[v][c - 'a'];
      if (!v) fail("missing transition for substring " + sub);
    }
    if (ex.cnt[v] != expected)
      fail("occurrence mismatch for substring " + sub);
  }
}

static void enumerate_words(int alphabet, int length,
                            vector<string> &out) {
  long long total = 1;
  for (int i = 0; i < length; ++i) total *= alphabet;
  for (long long code = 0; code < total; ++code) {
    long long x = code;
    string s(length, 'a');
    for (int i = 0; i < length; ++i) {
      s[i] = char('a' + x % alphabet);
      x /= alphabet;
    }
    out.push_back(s);
  }
}

int main() {
  int cases = 0;
  check({}), ++cases;
  check({""}), ++cases;
  check({"", "", "a", "a", "ab", "ba"}), ++cases;
  check({"aaaaaaa", "aaaa", "aaa"}), ++cases;
  check({"abcabc", "bcabc", "cabc", "abcabc"}), ++cases;

  vector<string> words;
  for (int n = 0; n <= 5; ++n) enumerate_words(3, n, words);
  for (const string &s : words) check({s}), ++cases;

  vector<string> short_words;
  for (int n = 0; n <= 3; ++n) enumerate_words(3, n, short_words);
  for (const string &a : short_words)
    for (const string &b : short_words) check({a, b}), ++cases;

  mt19937 rng(0xE5A17EEDu);
  for (int tc = 0; tc < 1200; ++tc) {
    vector<string> strings;
    const int m = rng() % 7;
    for (int i = 0; i < m; ++i) {
      const int n = rng() % 9;
      string s(n, 'a');
      for (char &c : s) c = char('a' + rng() % 3);
      strings.push_back(s);
    }
    check(strings), ++cases;
  }
  cout << "PASS exsam-edge cases=" << cases
       << " oracle=substring-occurrences-and-minimized-suffix-trie\n";
}
