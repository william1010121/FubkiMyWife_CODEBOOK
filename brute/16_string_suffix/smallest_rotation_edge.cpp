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

  // Rotation ordering is over raw string bytes; embedded NUL/high bytes must
  // not be treated as a terminator or signed characters.
  const vector<string> byte_words = {
      string({char(0), 'A', char(0), 'B'}),
      string({'A', char(255), 'A'}),
      string({char(128), char(255), char(128), char(0)}),
  };
  for (const string &s : byte_words) check(s, cases++);

  cout << "PASS Smallest_Rotation non-empty exhaustive/periodic/random cases="
       << cases << '\n';
  check({}, cases++);
  cout << "PASS Smallest_Rotation exhaustive/periodic/random cases=" << cases
       << " including empty\n";
}
