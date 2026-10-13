#include "pattern_brute_common.hpp"

using namespace std;
using namespace string_pattern_brute;

static vector<int> template_matches(const string &text,
                                    const string &pattern) {
  ostringstream output;
  string s = text, t = pattern;
#define cout output
#include "../../codebook/5_String/KMP.cpp"
#undef cout
  // nxt is public output too, including when no text match exists.
  vector<int> prefix(t.size());
  for (int i = 0; i < (int)t.size(); ++i)
    for (int len = 1; len <= i; ++len)
      if (t.compare(0, len, t, i + 1 - len, len) == 0)
        prefix[i] = len;
  expect_equal("KMP prefix table", shown(pattern), nxt, prefix);
  vector<int> answer;
  istringstream parsed(output.str());
  for (int x; parsed >> x;) answer.push_back(x);
  return answer;
}

static vector<int> direct_matches(const string &text,
                                  const string &pattern) {
  vector<int> answer;
  if (pattern.empty()) {
    for (int i = 0; i <= (int)text.size(); ++i) answer.push_back(i + 1);
    return answer;
  }
  for (int i = 0; i + (int)pattern.size() <= (int)text.size(); ++i)
    if (text.compare(i, pattern.size(), pattern) == 0)
      answer.push_back(i + 1);  // Matches are 1-based.
  return answer;
}

static void check(const string &text, const string &pattern) {
  expect_equal("KMP", shown(text) + "," + shown(pattern),
               template_matches(text, pattern),
               direct_matches(text, pattern));
}

int main() {
  long long cases = 0;

  // Every binary text of length at most eight against every binary
  // pattern of length at most six, including the empty text.
  const vector<string> binary_texts = words("ab", 8);
  const vector<string> binary_patterns = words("ab", 6);
  for (const string &text : binary_texts)
    for (const string &pattern : binary_patterns)
      check(text, pattern), ++cases;

  // A second alphabet catches accidental assumptions about one repeated
  // character and exercises pattern-longer-than-text cases systematically.
  const vector<string> ternary_texts = words("abc", 5);
  const vector<string> ternary_patterns = words("abc", 4);
  for (const string &text : ternary_texts)
    for (const string &pattern : ternary_patterns)
      check(text, pattern), ++cases;

  check("", "a");
  check("", "");
  check("abc", "");
  check("a", "a");
  check("aaaaaa", "aaa");
  check("abababab", "abab");
  check("abc", "abcd");
  check("ababa", "aba");
  check(string(512, 'a'), string(257, 'a'));
  // KMP compares bytes, not locale characters.  Keep embedded NUL and high
  // bytes in both text and pattern so a C-string assumption cannot pass.
  const vector<string> byte_words = {
      string({char(0), 'A', char(0), 'B'}),
      string({'A', char(255), 'A'}),
      string({char(128), char(255), char(128), char(0)}),
  };
  for (const string &text : byte_words)
    for (const string &pattern : byte_words) check(text, pattern), ++cases;
  cases += 9;

  for (int c = 0; c < 256; ++c) {
    string one(1, char(c)), repeated(7, char(c));
    for (const string &pattern : {string(), one, one + one}) {
      check(one, pattern); check(repeated, pattern); cases += 2;
    }
  }

  // Frequent matches/fallbacks with separator, NUL and signed-char bytes.
  string alphabet({'%', char(0), char(255)});
  for (const string &text : words(alphabet, 5))
    for (const string &pattern : words(alphabet, 4))
      check(text, pattern), ++cases;

  // A defect at each boundary of a long periodic prefix/fallback chain.
  for (int n : {31, 127, 511}) for (string unit : {"a", "ab", "abacaba"}) {
    string text;
    while ((int)text.size() < n) text += unit;
    text.resize(n);
    string pattern = text.substr(0, n / 2);
    check(text, pattern); ++cases;
    for (int at : {0, 1, n / 2 - 1, n / 2, n - 2, n - 1}) {
      string changed = text; changed[at] = '!';
      check(changed, pattern); check(text, changed.substr(0, n / 2));
      cases += 2;
    }
  }

  mt19937 rng(0x4B4D5032u);
  for (int tc = 0; tc < 4000; ++tc) {
    string text = random_word(rng, "abc", rng() % 160);
    string pattern = random_word(rng, "abc", rng() % 33);
    check(text, pattern);
    ++cases;
  }

  for (int tc = 0; tc < 1000; ++tc) {
    string text(rng() % 160, '\0');
    for (char &c : text) c = char(rng() % 256);
    int start = text.empty() ? 0 : rng() % text.size();
    string pattern = text.substr(start, rng() % 33);
    check(text, pattern); ++cases;
  }

  cout << "PASS KMP exhaustive_cases=" << cases
       << " oracle=direct_overlapping_substrings output=1-based\n";
}
