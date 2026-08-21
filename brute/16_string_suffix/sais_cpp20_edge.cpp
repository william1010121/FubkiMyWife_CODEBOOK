#include "suffix_test_common.hpp"

#define SZ(x) ((int)(x).size())
#define ALL(x) begin(x), end(x)
#define pb push_back
#include "../../codebook/5_String/SAIS-C++20.cpp"
#undef pb
#undef ALL
#undef SZ

using namespace suffix_edge;

static void check(const vector<int> &s, long long id) {
  const auto want_sa = oracle_sa(s);
  const auto want_h = oracle_lcp(s, want_sa);
  const auto want_ra = inverse_sa(want_sa);

  Suffix got(s.data(), (int)s.size());
  expect_vector(got.sa, want_sa, "SAIS-C++20 wrapper SA case " +
                                    to_string(id),
                s);
  expect_vector(got.hi, want_h, "SAIS-C++20 wrapper LCP case " +
                                    to_string(id),
                s);
  expect_vector(got.ra, want_ra, "SAIS-C++20 wrapper rank case " +
                                    to_string(id),
                s);

  // Also test the underlying routine directly on the sentinel-terminated
  // sequence used by the wrapper.
  vector<int> with_sentinel = s;
  with_sentinel.push_back(0);
  auto direct = sais(with_sentinel);
  expect_vector(direct, oracle_sa(with_sentinel),
                "SAIS-C++20 direct SA case " + to_string(id),
                with_sentinel);
}

int main() {
  long long cases = 0;
  enumerate_strings(7, 3, [&](const vector<int> &s) { check(s, cases++); });

  vector<vector<int>> periodic;
  add_periodic_cases(periodic);
  for (const auto &s : periodic) check(s, cases++);

  mt19937 rng(0xC20A15EEDu);
  for (int tc = 0; tc < 1600; ++tc) {
    int n = rng() % 320;
    int alphabet = 1 + rng() % 10;
    vector<int> s(n);
    for (int &x : s) x = 1 + rng() % alphabet;
    check(s, cases++);
  }

  // The C++20 implementation sizes its buckets from max(s), so test sparse
  // and large positive symbols as well as the usual compact alphabet.
  for (const vector<int> &s : {vector<int>{255},
                               vector<int>{1000, 1, 1000, 2, 1},
                               vector<int>{4095, 4095, 7, 4095, 7, 1},
                               vector<int>{1, 255, 1, 255, 1, 0 + 2}})
    check(s, cases++);

  cout << "PASS SAIS-C++20 exhaustive/periodic/random cases=" << cases
       << '\n';
}
