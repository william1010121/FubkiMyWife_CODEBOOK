#include <bits/stdc++.h>
using namespace std;

const int N = 128;
const int CNUM = 2;
#define ALL(x) (x).begin(), (x).end()
#include "../../../codebook/5_String/exSAM.cpp"

static map<string, int> brute_occ(const vector<string> &ss) {
  map<string, int> occ;
  for (const string &s : ss)
    for (int l = 0; l < (int)s.size(); ++l)
      for (int r = l + 1; r <= (int)s.size(); ++r)
        ++occ[s.substr(l, r - l)];
  return occ;
}

static int brute_dfa_states(const vector<string> &ss) {
  struct TNode { array<int, 2> nx{0, 0}; bool terminal = false; };
  vector<TNode> trie(1);
  for (const string &s : ss)
    for (int start = 0; start < (int)s.size(); ++start) {
      int v = 0;
      for (int i = start; i < (int)s.size(); ++i) {
        int c = s[i] - 'a';
        if (!trie[v].nx[c]) trie[v].nx[c] = trie.size(), trie.emplace_back();
        v = trie[v].nx[c];
      }
      trie[v].terminal = true;
    }
  map<vector<int>, int> classes;
  vector<int> cls(trie.size());
  for (int v = (int)trie.size() - 1; v >= 0; --v) {
    vector<int> sig = {trie[v].terminal, cls[trie[v].nx[0]], cls[trie[v].nx[1]]};
    auto [it, inserted] = classes.emplace(sig, classes.size() + 1);
    cls[v] = it->second;
  }
  return classes.size();
}

static void check(const vector<string> &ss) {
  exSAM ex;
  ex.init();
  for (const string &s : ss) ex.insert(s);
  ex.build();
  ex.solve();
  const map<string, int> want = brute_occ(ss);

  long long distinct = 0;
  for (int v = 1; v < ex.tot; ++v) distinct += ex.len[v] - ex.len[ex.link[v]];
  if (distinct != (long long)want.size())
    throw runtime_error("exSAM distinct-substring mismatch");
  for (const auto &[sub, expected] : want) {
    int v = 0;
    for (char c : sub) {
      v = ex.next[v][c - 'a'];
      if (!v) throw runtime_error("exSAM missing transition");
    }
    if (ex.cnt[v] != expected)
      throw runtime_error("exSAM occurrence mismatch");
  }
}

static vector<string> pool_words() {
  vector<string> p;
  function<void(string &)> dfs = [&](string &s) {
    if (!s.empty()) p.push_back(s);
    if ((int)s.size() == 4) return;
    for (char c : string("ab")) s.push_back(c), dfs(s), s.pop_back();
  };
  string s;
  dfs(s);
  return p;
}

int main() {
  try {
    check({"ababa", "baba", "aa", "ababa"});
    check({"a", "b", "ab", "ba", "aba", "bab"});
    const vector<string> pool = pool_words();
    mt19937 rng(0xE5A17u);
    for (int tc = 0; tc < 900; ++tc) {
      vector<string> ss;
      const int m = 1 + rng() % 5;
      for (int i = 0; i < m; ++i) ss.push_back(pool[rng() % pool.size()]);
      check(ss);
    }
    cout << "PASS exsam cases=902 oracle=all-substring-set-and-occurrences\n";
  } catch (const exception &e) {
    cerr << "FAIL exsam: " << e.what() << '\n';
    return 1;
  }
}
