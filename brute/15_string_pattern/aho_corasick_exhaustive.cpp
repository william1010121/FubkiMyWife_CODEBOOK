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
    expect(all_of(pattern.begin(), pattern.end(), [](char c) {
      return 'A' <= c && c < 'A' + sigma;
    }), "AC pattern alphabet", shown(text) + "," + shown(pattern));

  ac.init();
  vector<int> endpoints;
  map<string, int> multiplicity;
  for (const string &pattern : patterns) {
    string copy = pattern; // input() takes a nonconst reference.
    endpoints.push_back(ac.input(copy));
    ++multiplicity[pattern];
  }
  ac.make_fl();

  vector<int> rank(ac.top, -1);
  for (int i = 0; i < ac.top - 1; ++i) {
    int v = ac.ord[i];
    expect(1 <= v && v < ac.top && rank[v] == -1, "AC BFS order", text);
    rank[v] = i;
    if (v != 1)
      expect(1 <= ac.fl[v] && rank[ac.fl[v]] >= 0 &&
                 rank[ac.fl[v]] < i, "AC failure is already built", text);
    for (int c = 0; c < sigma; ++c)
      expect(1 <= ac.rnx[v][c] && ac.rnx[v][c] < ac.top,
             "AC complete transition", text);
  }
  // The empty pattern matches the boundary preceding the first character.
  if (multiplicity.count("")) ac.cnt[1] = 1;

  int state = 1;
  for (char c : text) {
    expect('A' <= c && c < 'A' + sigma, "AC alphabet", shown(text));
    state = ac.rnx[state][c - 'A'];
    ++ac.cnt[state];
  }
  ac.solve();

  for (int i = 0; i < (int)patterns.size(); ++i) {
    const string &pattern = patterns[i];
    // Duplicates share a trie endpoint. Each copy gets the same count,
    // without multiplying the scan itself.
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

  // All texts up to length five, all nonempty ABC patterns up to three.
  // This covers every failure-link shape in the tiny domain.
  const vector<string> texts = words("ABC", 5);
  vector<string> all_patterns = words("ABC", 3);
  all_patterns.erase(all_patterns.begin()); // Skip the empty word here.
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

  check({"", "", "A", "AA", "BA"}, "AABAA");
  check({""}, "");
  check({"", "A", "A"}, "CCCC");
  check({string(len - 2, 'A')}, string(len + 7, 'A'));
  expect(ac.top == len, "AC exact node capacity", "");
  cases += 4;
  ac.init();
  string a = "A", b = "B";
  int va = ac.input(a), vb = ac.input(b);
  ac.make_fl();
  ac.cnt[va] = INT_MAX - 1, ac.cnt[vb] = 1;
  ac.solve();
  expect(ac.cnt[1] == INT_MAX, "AC maximum representable visits", "");
  ++cases;

  // Every prefix/suffix is a pattern; the absent C forces root fallback.
  string periodic;
  for (int i = 0; i < 80; ++i) periodic += "ABBA"[i % 4];
  vector<string> nested = {"", ""};
  for (int i = 1; i <= (int)periodic.size(); ++i) {
    nested.push_back(periodic.substr(0, i));
    nested.push_back(periodic.substr(periodic.size() - i));
  }
  for (int i = 0; i < 8; ++i) {
    string text = periodic + 'C' + periodic;
    text[79 + i] = 'C';
    check(nested, text), ++cases;
    reverse(nested.begin(), nested.end());
  }

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
       << " oracle=direct_overlapping_occurrences_with_duplicate_patterns"
       << '\n';
}
