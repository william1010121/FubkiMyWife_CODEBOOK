#include "pattern_brute_common.hpp"

using namespace std;
using namespace string_pattern_brute;

constexpr int MAXn = 4096;
#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Z-value.cpp"
#undef SZ

static vector<int> template_z(const string &input) {
  fill(z, z + input.size(), 0);  // z[0] is intentionally left unset.
  make_z(input);
  return vector<int>(z, z + input.size());
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
  expect_equal("Z-value", shown(input), template_z(input), direct_z(input));
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
  cases += 7;

  mt19937 rng(0x5A56414Cu);
  for (int tc = 0; tc < 3000; ++tc) {
    check(random_word(rng, "abc", rng() % 520));
    ++cases;
  }

  cout << "PASS Z-value exhaustive_cases=" << cases
       << " oracle=direct_lcp_from_position_zero z0=unset\n";
}
