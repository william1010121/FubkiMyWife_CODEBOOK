#include "pattern_brute_common.hpp"

using namespace std;
using namespace string_pattern_brute;

constexpr int MAXN = 4096;
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "../../codebook/5_String/Manacher.cpp"
#undef pb
#undef SZ

static vector<int> template_radii(const string &input) {
  fill(z, z + 2 * input.size() + 1, 0);
  Manacher(input);
  return vector<int>(z, z + 2 * input.size() + 1);
}

static vector<int> direct_radii(const string &input) {
  // Use an integer separator outside the byte alphabet.  This keeps the
  // oracle valid even when the input itself contains '%', NUL, or 0xff.
  vector<int> transformed = {256};
  for (unsigned char c : input) {
    transformed.push_back(c);
    transformed.push_back(256);
  }
  vector<int> answer(transformed.size(), 1);
  for (int center = 0; center < (int)transformed.size(); ++center)
    while (center - answer[center] >= 0 &&
           center + answer[center] < (int)transformed.size() &&
           transformed[center - answer[center]] ==
               transformed[center + answer[center]])
      ++answer[center];
  return answer;
}

static void check(const string &input) {
  expect_equal("Manacher", shown(input), template_radii(input),
               direct_radii(input));
}

int main() {
  long long cases = 0;

  for (const string &input : words("ab", 10)) check(input), ++cases;
  for (const string &input : words("abc", 7)) check(input), ++cases;

  check("");
  check("a");
  check("aa");
  check("abba");
  check("aaaaaa");
  check("ababababab");
  check("abcddcba");
  check(string(2047, 'a'));  // 2*n+1 == MAXN-1, the largest valid input.
  const vector<string> byte_words = {
      string({char(0), 'A', char(0), 'B'}),
      string({'A', char(255), 'A'}),
      string({char(128), char(255), char(128), char(0)}),
  };
  for (const string &input : byte_words) check(input), ++cases;
  cases += 8;

  mt19937 rng(0x4D414E32u);
  for (int tc = 0; tc < 3000; ++tc) {
    check(random_word(rng, "abc", rng() % 420));
    ++cases;
  }

  cout << "PASS Manacher exhaustive_cases=" << cases
       << " oracle=direct_palindrome_radii\n";
}
