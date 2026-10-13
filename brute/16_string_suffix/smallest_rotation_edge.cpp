#include "suffix_test_common.hpp"

#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Smallest_Rotation.cpp"
#undef SZ

using namespace suffix_edge;

static void check(const string &s, long long id) {
  const string want = min_rotation_oracle(s);
  string got;
  try {
    got = mcp(s);
  } catch (const exception &e) {
    fail("Smallest_Rotation threw on case " + to_string(id) + " s=" +
         describe(s) + ": " + e.what());
  } catch (...) {
    fail("Smallest_Rotation threw an unknown exception on case " +
         to_string(id) + " s=" + describe(s));
  }
  if (got != want)
    fail("Smallest_Rotation case " + to_string(id) + " s=" + describe(s) +
         " got=" + describe(got) + " want=" + describe(want));
}

int main() {
  long long cases = 0;
  enumerate_strings(8, 3, [&](const vector<int> &values) {
    if (!values.empty()) check(as_string(values), cases++);
  });

  vector<vector<int>> periodic;
  add_periodic_cases(periodic);
  for (const auto &values : periodic) check(as_string(values), cases++);

  mt19937 rng(0xB07A710Du);
  for (int tc = 0; tc < 2200; ++tc) {
    int n = 1 + rng() % 500;
    int alphabet = 1 + rng() % 14;
    string s(n, 'a');
    for (char &c : s) c = char('a' + rng() % alphabet);
    check(s, cases++);
  }

  // Rotation order is over raw bytes; embedded NUL/high bytes must
  // not be treated as a terminator or signed characters.
  const vector<string> byte_words = {
      string({char(0), 'A', char(0), 'B'}),
      string({'A', char(255), 'A'}),
      string({char(128), char(255), char(128), char(0)}),
  };
  for (const string &s : byte_words) check(s, cases++);

  for (int a = 0; a < 256; ++a) {
    check(string(1, char(a)), cases++);
    for (int b = 0; b < 256; ++b)
      check(string({char(a), char(b)}), cases++);
  }
  const unsigned char alphabet[] = {0, 1, 127, 128, 255};
  enumerate_strings(6, 5, [&](const vector<int> &values) {
    string s;
    for (int x : values) s += char(alphabet[x - 1]);
    check(s, cases++);
  });
  for (int tc = 0; tc < 512; ++tc) {
    string s(rng() % 600, '\0');
    for (char &c : s) c = char(rng() & 255);
    check(s, cases++);
  }
  // A late unique minimum defeats quadratic candidate comparison.
  string long_word(200000, char(255));
  long_word[100000] = '\0';
  string want = string(1, '\0') + string(199999, char(255));
  expect(mcp(long_word) == want, "large high-byte rotation");
  ++cases;

  // Long tied rotations, then a mismatch at every possible byte position.
  for (int period : {1, 2, 3, 7, 16}) {
    string s(8 * period + 1, char(255));
    for (int i = 0; i < (int)s.size(); ++i)
      s[i] = char((i % period) ? 128 : 0);
    for (int p = 0; p < (int)s.size(); ++p) {
      string t = s;
      t[p] = char(1);
      check(t, cases++);
      reverse(t.begin(), t.end());
      check(t, cases++);
    }
  }

  cout << "PASS Smallest_Rotation exhaustive/periodic/byte/random cases="
       << cases << '\n';
  check({}, cases++);
  cout << "PASS Smallest_Rotation exhaustive/periodic/random cases="
       << cases
       << " including empty\n";
}
