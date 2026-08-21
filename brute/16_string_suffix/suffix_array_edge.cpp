#include "suffix_test_common.hpp"

constexpr int MAXN = 1024;
#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Suffix_Array.cpp"
#undef SZ

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace suffix_edge;

static void check(const vector<int> &values, long long id) {
  string s = as_string(values);
  auto want_sa = oracle_sa(s);
  auto want_h = oracle_lcp(s, want_sa);
  auto want_ra = inverse_sa(want_sa);

  suffix_array got;
  got.build(s);
  expect_vector(vector<int>(got.sa, got.sa + s.size()), want_sa,
                "Suffix_Array SA case " + to_string(id), values);
  expect_vector(vector<int>(got.he, got.he + s.size()), want_h,
                "Suffix_Array LCP case " + to_string(id), values);
  expect_vector(vector<int>(got.ra, got.ra + s.size()), want_ra,
                "Suffix_Array rank case " + to_string(id), values);
  for (int i = 0; i < (int)s.size(); ++i)
    expect(got.ra[got.sa[i]] == i,
           "Suffix_Array rank permutation case " + to_string(id));
}

static void check_empty_contract() {
  // build() has no return value, so isolate the zero-length probe.  A crash
  // is reported as a contract failure without taking down the rest of this
  // executable or hiding the ordinary exhaustive diagnostics.
  pid_t pid = fork();
  if (pid < 0) fail("cannot fork empty Suffix_Array probe");
  if (pid == 0) {
    alarm(2);
    suffix_array got;
    got.build("");
    _exit(0);
  }
  int status = 0;
  if (waitpid(pid, &status, 0) < 0) fail("waitpid failed for empty probe");
  if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
    fail("Suffix_Array empty input does not produce the oracle empty SA/LCP "
         "(the template may require a non-empty string)");
}

int main() {
  long long cases = 0;
  enumerate_strings(8, 3,
                   [&](const vector<int> &s) { if (!s.empty()) check(s, cases++); });

  vector<vector<int>> periodic;
  add_periodic_cases(periodic);
  for (const auto &s : periodic) check(s, cases++);

  mt19937 rng(0x5AFF1EEDu);
  for (int tc = 0; tc < 1800; ++tc) {
    int n = 1 + rng() % 420;
    int alphabet = 1 + rng() % 12;
    vector<int> s(n);
    for (int &x : s) x = 1 + rng() % alphabet;
    check(s, cases++);
  }
  cout << "PASS Suffix_Array non-empty exhaustive/periodic/random cases="
       << cases << '\n';
  check_empty_contract();
  cout << "PASS Suffix_Array exhaustive/periodic/random cases=" << cases
       << " plus empty-input probe\n";
}
