#include <bits/stdc++.h>
using namespace std;

#include "../../../codebook/5_String/SAM.cpp"

static map<string, int> brute_occ(const string &s) {
  map<string, int> occ;
  for (int l = 0; l < (int)s.size(); ++l)
    for (int r = l + 1; r <= (int)s.size(); ++r)
      ++occ[s.substr(l, r - l)];
  return occ;
}

static void check(const string &s) {
  sam.init();
  for (char c : s) sam.push(c - 'a' + 1);
  sam.count();
  const map<string, int> want = brute_occ(s);
  long long distinct = 0;
  for (int v = 1; v <= sam.tot; ++v)
    distinct += sam.mx[v] - sam.mx[sam.mom[v]];
  if (distinct != (long long)want.size())
    throw runtime_error("SAM distinct-substring mismatch");
  long long fast_answer = 0;
  for (int v = 1; v <= sam.tot; ++v)
    if (sam.cnt[v] > 1) fast_answer = max(fast_answer, 1LL * sam.cnt[v] * (long long)sam.mx[v]);
  long long brute_answer = 0;
  for (const auto &[sub, count] : want)
    if (count > 1) brute_answer = max(brute_answer, 1LL * count * (long long)sub.size());
  if (fast_answer != brute_answer)
    throw runtime_error("SAM P3804 answer mismatch");
  for (const auto &[sub, expected] : want) {
    int v = sam.root;
    for (char c : sub) {
      v = sam.nxt[v][c - 'a' + 1];
      if (!v) throw runtime_error("SAM missing transition");
    }
    if (sam.cnt[v] != expected)
      throw runtime_error("SAM occurrence mismatch");
  }
}

int main() {
  try {
    check("aaaaaa");
    check("abababa");
    check("abacaba");
    for (int n = 1; n <= 9; ++n)
      for (int mask = 0; mask < (1 << n); ++mask) {
        string s(n, 'a');
        for (int i = 0; i < n; ++i)
          if (mask >> i & 1) s[i] = 'b';
        check(s);
      }
    cout << "PASS sam cases=1533 oracle=all-substring-occurrences\n";
  } catch (const exception &e) {
    cerr << "FAIL sam: " << e.what() << '\n';
    return 1;
  }
}
