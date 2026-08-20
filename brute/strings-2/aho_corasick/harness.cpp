#include <bits/stdc++.h>
using namespace std;

const int len = 512;
const int sigma = 3;
#include "../../../codebook/5_String/Aho-Corasick_Automatan.cpp"

static int brute_at(const vector<string> &patterns, const string &text, int pos) {
  int ans = 0;
  for (const string &p : patterns)
    if ((int)p.size() <= pos + 1 && text.compare(pos + 1 - p.size(), p.size(), p) == 0)
      ++ans;
  return ans;
}

static void check(const vector<string> &patterns, const string &text) {
  ac.init();
  vector<int> endpoints;
  for (string p : patterns) endpoints.push_back(ac.input(p));
  ac.make_fl();
  int state = 1;
  fill(ac.cnt, ac.cnt + len, 0);
  for (char c : text) {
    state = ac.rnx[state][c - 'A'];
    ++ac.cnt[state];
  }
  ac.solve();
  for (int i = 0; i < (int)patterns.size(); ++i) {
    int want = 0;
    for (int pos = 0; pos < (int)text.size(); ++pos)
      want += brute_at({patterns[i]}, text, pos);
    if (ac.cnt[endpoints[i]] != want)
      throw runtime_error("Aho-Corasick occurrence mismatch");
  }
}

static vector<string> words(const string &alphabet, int max_len) {
  vector<string> out;
  function<void(string &)> dfs = [&](string &s) {
    if (!s.empty()) out.push_back(s);
    if ((int)s.size() == max_len) return;
    for (char c : alphabet) s.push_back(c), dfs(s), s.pop_back();
  };
  string s;
  dfs(s);
  return out;
}

int main() {
  try {
    check({"A", "AA", "AAA", "A", "BA", "ABA"}, "AAAAABAAB");
    check({"AB", "B", "AB", "BAB", "CAB", "A"}, "CBABABACAB");
    const vector<string> pool = words("ABC", 4);
    mt19937 rng(0x5A17u);
    for (int tc = 0; tc < 1200; ++tc) {
      vector<string> ps;
      const int m = 1 + (rng() % 12);
      for (int i = 0; i < m; ++i) ps.push_back(pool[rng() % pool.size()]);
      string text;
      const int n = rng() % 25;
      for (int i = 0; i < n; ++i) text.push_back("ABC"[rng() % 3]);
      check(ps, text);
    }
    // Explicitly exercise reuse of the same global object after larger tries.
    check({"C", "CC", "CCC"}, "CCCCCC");
    cout << "PASS aho_corasick cases=1204 oracle=occurrences-with-duplicates\n";
  } catch (const exception &e) {
    cerr << "FAIL aho_corasick: " << e.what() << '\n';
    return 1;
  }
}
