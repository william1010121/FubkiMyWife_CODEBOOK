#include "string_brute_common.hpp"

vector<int> template_matches(const string &text, const string &pattern) {
  ostringstream output;
  string s = text, t = pattern;
#define cout output
#include "../../codebook/5_String/KMP.cpp"
#undef cout
  vector<int> ans;
  istringstream parsed(output.str());
  for (int x; parsed >> x;) ans.push_back(x);
  return ans;
}

vector<int> oracle_matches(const string &s, const string &t) {
  vector<int> ans;
  if (t.empty()) {
    for (int i = 0; i <= (int)s.size(); ++i) ans.push_back(i + 1);
    return ans;
  }
  for (int i = 0; i + (int)t.size() <= (int)s.size(); ++i)
    if (s.compare(i, t.size(), t) == 0) ans.push_back(i + 1);
  return ans;
}

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    string s, t;
    if (!(cin >> s >> t)) return 2;
    for (int x : oracle_matches(s, t)) cout << x << ' ';
    cout << '\n';
    return 0;
  }
  for (int tc = 0; tc < 500; ++tc) {
    string s = random_string(1 + rng() % 45, 1 + rng() % 4);
    string t = random_string(1 + rng() % 14, 1 + rng() % 4);
    expect(template_matches(s, t) == oracle_matches(s, t), "KMP");
  }
  expect(template_matches("aaaaa", "aaa") == vector<int>({1, 2, 3}), "KMP overlap");
  expect(template_matches("abc", "") == vector<int>({1, 2, 3, 4}), "KMP empty");
  cout << "PASS KMP 501 cases\n";
}
