#include "pattern_brute_common.hpp"

using namespace std;
using namespace string_pattern_brute;

constexpr int MAXn = 4096;
#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Z-value.cpp"
#undef SZ

static vector<int> template_z(const string &input) {
  fill(z, z + MAXn, INT_MIN);  // No zero-initialization may be assumed.
  make_z(input);
  expect(z[0] == INT_MIN, "Z-value z[0] unchanged", shown(input));
  expect(all_of(z + max(1, (int)input.size()), z + MAXn,
                [](int x) { return x == INT_MIN; }),
         "Z-value no suffix write", shown(input));
  vector<int> result(z, z + input.size());
  if (!result.empty()) result[0] = 0; // Oracle leaves z[0] unset too.
  return result;
}

static vector<int> direct_z(const string &input) {
  vector<int> answer(input.size(), 0);
  for (int i = 1; i < (int)input.size(); ++i)
    while (i + answer[i] < (int)input.size() &&
           input[answer[i]] == input[i + answer[i]])
      ++answer[i];
  return answer;
}

static void check(const string &input) {
  expect_equal("Z-value", shown(input),
               template_z(input), direct_z(input));
}

int main() {
  long long cases = 0;

  for (const string &input : words("ab", 13)) check(input), ++cases;
  for (const string &input : words("abc", 8)) check(input), ++cases;

  check("");
  check("a");
  check("aaaaaaa");
  check("abababab");
  check("abcababcab");
  check("abacaba");
  check(string(4096, 'a'));  // The array has valid indices [0, MAXn).
  // Z-values are byte-wise; these cases include embedded NUL and 0xff.
  const vector<string> byte_words = {
      string({char(0), 'A', char(0), 'B'}),
      string({'A', char(255), 'A'}),
      string({char(128), char(255), char(128), char(0)}),
  };
  for (const string &input : byte_words) check(input), ++cases;
  cases += 7;

  for (int c = 0; c < 256; ++c) {
    check(string(1, char(c))); check(string(7, char(c))); cases += 2;
  }

  mt19937 rng(0x5A56414Cu);
  for (int tc = 0; tc < 3000; ++tc) {
    check(random_word(rng, "abc", rng() % 520));
    ++cases;
  }

  for (int tc = 0; tc < 1000; ++tc) {
    string input(rng() % 520, '\0');
    for (char &c : input) c = char(rng() % 256);
    input += input; // Repeated arbitrary bytes require Z-box reuse.
    check(input); ++cases;
  }

  cout << "PASS Z-value exhaustive_cases=" << cases
       << " oracle=direct_lcp_from_position_zero z0=unset\n";
}
