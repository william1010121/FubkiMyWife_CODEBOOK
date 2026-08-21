#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/5_String/De_Bruijn_sequence.cpp"

static void fail(const string &message) {
  cerr << "FAIL de-bruijn-edge: " << message << '\n';
  exit(1);
}

static long long power(int base, int exponent) {
  long long result = 1;
  for (int i = 0; i < exponent; ++i) result *= base;
  return result;
}

static void check(int alphabet, int order, int windows) {
  const long long total = power(alphabet, order);
  const long long length = (long long)order + windows - 1;
  vector<int> out((size_t)max(0LL, length), -9);
  dbs.solve(alphabet, order, windows, out.data());
  if ((long long)out.size() != length)
    fail("output length mismatch");

  if (order == 0) {
    if (windows != 1 || !out.empty()) fail("order-zero boundary mismatch");
    return;
  }
  if (windows < 1 || windows > total) fail("invalid test case");

  unordered_set<long long> seen;
  seen.reserve((size_t)windows * 2 + 1);
  for (int x : out)
    if (x < 0 || x >= alphabet) fail("symbol outside alphabet");
  for (int start = 0; start < windows; ++start) {
    long long code = 0;
    for (int j = 0; j < order; ++j)
      code = code * alphabet + out[start + j];
    if (!seen.insert(code).second) fail("duplicate de Bruijn window");
  }
  if ((int)seen.size() != windows) fail("wrong number of windows");
  if (windows == total) {
    for (long long code = 0; code < total; ++code)
      if (!seen.count(code)) fail("full de Bruijn sequence misses a word");
  }
}

int main() {
  int cases = 0;
  for (int alphabet : {1, 2, 3, 4})
    for (int order = 0; order <= 5; ++order) {
      const int total = (int)power(alphabet, order);
      for (int windows = 1; windows <= total; ++windows)
        check(alphabet, order, windows), ++cases;
    }
  for (int order = 0; order <= 3; ++order) {
    const int total = (int)power(10, order);
    for (int windows = 1; windows <= total; ++windows)
      check(10, order, windows), ++cases;
  }
  cout << "PASS de-bruijn-edge cases=" << cases
       << " oracle=all-linear-window-uniqueness\n";
}
