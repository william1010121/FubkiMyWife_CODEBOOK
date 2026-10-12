#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
set<string> subsequences(const string &s) {
  set<string> ret;
  for (int mask = 0; mask < (1 << SZ(s)); ++mask) {
    string x;
    for (int i = 0; i < SZ(s); ++i) if (mask >> i & 1) x += s[i];
    ret.insert(x);
  }
  return ret;
}
int main(int argc, char **) {
  mt19937 rng(1509);
  for (int tc = 0; tc < 1500; ++tc) {
    string s(rng() % 11, '\0'), t(rng() % 11, '\0');
    for (char &c : s) c = char(rng() % 5 * 63);
    for (char &c : t) c = char(rng() % 5 * 63);
    if (tc % 3 == 0) t = s;
    auto a = subsequences(s), b = subsequences(t);
    vector<string> both, want;
    set_intersection(ALL(a), ALL(b), back_inserter(both));
    size_t best = 0;
    for (auto &x : both) best = max(best, x.size());
    for (auto &x : both) if (x.size() == best) want.pb(x);
    assert(all_lcs(s, t) == want);
  }
  if (argc > 1) { // This case would allocate 40 billion DP cells.
    string s(200000, '\0');
    for (int i = 0; i < SZ(s); ++i) s[i] = char(i % 256);
    assert(all_lcs(s, s) == vector<string>{s});
    assert(all_lcs(string(200000, 'a'), string(200000, 'a')) ==
           vector<string>{string(200000, 'a')});
  }
  cout << "PASS equality/near-equality byte oracle and optional 200k\n";
}
