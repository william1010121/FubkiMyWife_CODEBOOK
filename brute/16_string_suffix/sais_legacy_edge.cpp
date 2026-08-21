#include "suffix_test_common.hpp"

#define N 1024
#include "../../codebook/5_String/SAIS.cpp"
#include "../../codebook/5_String/SAIS-old.cpp"
#undef N

using namespace suffix_edge;

static void check(const vector<int> &s, long long id) {
  const auto want_sa = oracle_sa(s);
  const auto want_h = oracle_lcp(s, want_sa);
  const auto want_ra = inverse_sa(want_sa);

  vector<int> input = s;
  sfx::build(input.data(), (int)input.size());
  expect_vector(vector<int>(sfx::SA, sfx::SA + s.size()), want_sa,
                "SAIS SA case " + to_string(id), s);
  expect_vector(vector<int>(sfx::H, sfx::H + s.size()), want_h,
                "SAIS LCP case " + to_string(id), s);
  expect_vector(vector<int>(sfx::RA, sfx::RA + s.size()), want_ra,
                "SAIS rank case " + to_string(id), s);

  SAIS old;
  old.build(input.data(), (int)input.size(), 256);
  expect_vector(vector<int>(old.SA, old.SA + s.size()), want_sa,
                "SAIS-old SA case " + to_string(id), s);
  expect_vector(vector<int>(old.H, old.H + s.size()), want_h,
                "SAIS-old LCP case " + to_string(id), s);
}

int main() {
  long long cases = 0;
  enumerate_strings(7, 3, [&](const vector<int> &s) { check(s, cases++); });

  vector<vector<int>> periodic;
  add_periodic_cases(periodic);
  for (const auto &s : periodic) check(s, cases++);

  mt19937 rng(0x16A15EEDu);
  for (int tc = 0; tc < 1600; ++tc) {
    int n = rng() % 300;
    int alphabet = 1 + rng() % 8;
    vector<int> s(n);
    for (int &x : s) x = 1 + rng() % alphabet;
    check(s, cases++);
  }

  // Exercise the full byte alphabet, including the largest legal bucket.
  for (int n : {1, 2, 3, 17, 64, 127, 255}) {
    vector<int> s(n);
    for (int i = 0; i < n; ++i)
      s[i] = (i * 73 + (i / 7) * 41) % 255 + 1;
    check(s, cases++);
  }
  cout << "PASS SAIS and SAIS-old exhaustive/periodic/random cases="
       << cases << '\n';
}
