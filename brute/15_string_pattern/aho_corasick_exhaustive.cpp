#include "pattern_brute_common.hpp"

using namespace std;
using namespace string_pattern_brute;

constexpr int len = 2048;
constexpr int sigma = 3;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"

static int occurrences(const string &text, const string &pattern) {
  int answer = 0;
  for (int i = 0; i + (int)pattern.size() <= (int)text.size(); ++i)
    answer += text.compare(i, pattern.size(), pattern) == 0;
  return answer;
}

static void check(const vector<string> &patterns, const string &text) {
  for (const string &pattern : patterns)
    expect(!pattern.empty(), "AC test setup requires nonempty patterns",
           shown(text) + "," + shown(pattern));

  ac.init();
  vector<int> endpoints;
  map<string, int> multiplicity;
  for (const string &pattern : patterns) {
    string copy = pattern;  // input() deliberately takes a nonconst reference.
    endpoints.push_back(ac.input(copy));
    ++multiplicity[pattern];
  }
  ac.make_fl();

  int state = 1;
  for (char c : text) {
    expect('A' <= c && c < 'A' + sigma, "AC alphabet", shown(text));
    state = ac.rnx[state][c - 'A'];
    ++ac.cnt[state];
  }
  ac.solve();

  for (int i = 0; i < (int)patterns.size(); ++i) {
    const string &pattern = patterns[i];
    // Duplicate dictionary entries share one trie endpoint. The template's
    // scan count is per endpoint, so each copy is checked against that same
    // occurrence count rather than multiplying the scan itself.
    const int want = occurrences(text, pattern);
    const int got = ac.cnt[endpoints[i]];
    if (got != want) {
      cerr << "FAIL Aho-Corasick text=" << shown(text)
           << " pattern=" << shown(pattern) << " got=" << got
           << " want=" << want << " multiplicity=" << multiplicity[pattern]
           << '\n';
      exit(1);
    }
  }
}

int main() {
  long long cases = 0;

  // All texts over ABC up to length five, with every nonempty ABC pattern up
  // to length three. This covers every failure-link shape in the tiny domain.
  const vector<string> texts = words("ABC", 5);
  vector<string> all_patterns = words("ABC", 3);
  all_patterns.erase(all_patterns.begin());  // Do not insert the empty word.
  for (const string &text : texts) check(all_patterns, text), ++cases;

  check({}, "");
  check({}, "ABCABC");
  check({"A"}, "");
  check({"A", "A", "AA", "AAA", "AAAA"}, "AAAAAA");
  check({"AB", "B", "AB", "BAB", "ABA", "A"}, "ABABABABA");
  check({"ABC", "BC", "C", "ABC", "A"}, "ABCABC");
  vector<string> near_capacity = words("ABC", 6);
  near_capacity.erase(near_capacity.begin());
  check(near_capacity, "ABCABCABCABCABCABC");
  cases += 7;

  mt19937 rng(0xAC0F2026u);
  for (int tc = 0; tc < 2500; ++tc) {
    vector<string> patterns;
    const int m = rng() % 25;
    for (int i = 0; i < m; ++i)
      patterns.push_back(random_word(rng, "ABC", 1 + rng() % 8));
    string text = random_word(rng, "ABC", rng() % 70);
    check(patterns, text);
    ++cases;
  }

  cout << "PASS Aho-Corasick cases=" << cases
       << " oracle=direct_overlapping_occurrences_with_duplicate_patterns\n";
}
