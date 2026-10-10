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
  vector<int> storage((size_t)length + 2, -9);
  dbs.solve(alphabet, order, windows, storage.data() + 1);
  if (storage.front() != -9 || storage.back() != -9)
    fail("output buffer boundary overwritten");
  vector<int> out(storage.begin() + 1, storage.end() - 1);
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
  // solve() must reset the seed even after the scratch buffer was reused.
  dbs.buf[0] = 7;
  check(2, 3, 8), ++cases;
  // Unary output should not require a hundred thousand recursive frames.
  check(1, 100000, 1), ++cases;
  // One window is all zeros for any alphabet, even at very large order.
  vector<int> single(500000, -9);
  dbs.solve(2, single.size(), 1, single.data());
  if (any_of(single.begin(), single.end(), [](int x) { return x != 0; }))
    fail("single-window output mismatch");
  ++cases;
  check(INT_MAX, 1, 8), ++cases;
  check(46341, 2, 33), ++cases;
  check(257, 2, 257 * 257), ++cases;
  check(2, 16, 1 << 16), ++cases;
  // Contest-scale outputs with short and long FKM copy blocks.
  check(200000, 1, 200000), ++cases;
  check(447, 2, 447 * 447), ++cases;
  check(2, 18, 1 << 18), ++cases;
  // Maximum buffer order still takes the single-window fast path.
  single.assign(MAXC * MAXN - 1, -9);
  dbs.solve(INT_MAX, single.size(), 1, single.data());
  if (any_of(single.begin(), single.end(), [](int x) { return x != 0; }))
    fail("maximum-order single window");
  ++cases;
  // K=2 takes DFS; the first two words differ only at their last symbol.
  single.assign(2049, -9);
  dbs.solve(INT_MAX, 2048, 2, single.data());
  if (single.back() != 1 ||
      any_of(single.begin(), single.end() - 1,
             [](int x) { return x != 0; }))
    fail("large-order two-window prefix");
  ++cases;
  dbs.solve(INT_MAX, 0, 1, nullptr), ++cases;
  cout << "PASS de-bruijn-edge cases=" << cases
       << " oracle=all-linear-window-uniqueness\n";
}
