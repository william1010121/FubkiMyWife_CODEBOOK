#include <bits/stdc++.h>
using namespace std;

void instantiate_template(int n) {
  // The source is intentionally a side-effect-free snippet.  Include and
  // execute it directly so compile/termination are checked as well.
#include "../../codebook/6_Math/floor_enumeration.cpp"
}

struct observed_n {
  int value, left = 0, quotient = 0;
  bool pending = false;
  vector<tuple<int,int,int>> blocks;
  friend bool operator<=(int l, observed_n &n) {
    if (n.pending) n.blocks.emplace_back(n.left,l-1,n.quotient);
    n.left = l; n.pending = false;
    return l <= n.value;
  }
  friend int operator/(observed_n &n, int divisor) {
    int result = n.value / divisor;
    if (!n.pending) n.quotient = result, n.pending = true;
    return result;
  }
};
vector<tuple<int,int,int>> enumerate_oracle_input(int value) {
  // Observe n/l once per iteration; the next loop bound reveals r+1.
  // This records actual l,r,x even when the n/x division is skipped.
  observed_n n{value};
#include "../../codebook/6_Math/floor_enumeration.cpp"
  assert(!n.pending);
  return n.blocks;
}

void check(int n) {
  instantiate_template(n);
  const auto got = enumerate_oracle_input(n);
  if (n == 0) { assert(got.empty()); return; }
  if (got.empty()) {
    cerr << "empty enumeration n=" << n << '\n';
    exit(1);
  }

  int next_l = 1;
  for (auto [l, r, x] : got) {
    if (l != next_l || l > r || x != n / l || x != n / r) {
      cerr << "bad floor block n=" << n << " block=" << l << "," << r
           << "," << x << " expected_l=" << next_l << '\n';
      exit(1);
    }
    if (l > 1 && n / (l - 1) == x) {
      cerr << "left endpoint is not maximal n=" << n << " l=" << l
           << '\n';
      exit(1);
    }
    if (r < n && n / (r + 1) == x) {
      cerr << "right endpoint is not maximal n=" << n << " r=" << r
           << '\n';
      exit(1);
    }
    next_l = r + 1;
  }
  if (next_l != n + 1) {
    cerr << "blocks do not cover 1..n, n=" << n << " next=" << next_l
         << '\n';
    exit(1);
  }

  // For small n, use a deliberately direct scan rather than the quotient
  // identity used by the template.
  if (n <= 3000) {
    vector<tuple<int, int, int>> want;
    for (int i = 1; i <= n;) {
      const int x = n / i;
      int j = i;
      while (j < n && n / (j + 1) == x) ++j;
      want.emplace_back(i, j, x);
      i = j + 1;
    }
    if (got != want) {
      cerr << "direct floor-block mismatch n=" << n << '\n';
      exit(1);
    }
  }
}

int main() {
  check(0);
  for (int n = 1; n <= 3000; ++n) check(n);
  const vector<int> selected = {
      9999, 10000, 10001, 46340, 46341, 1000000, 1000000007,
      2147483646};
  for (int n : selected) check(n);

  // The division shortcut changes branch at perfect-square boundaries.
  for (int k : {1,2,3,31,32,1000,32767,46339,46340})
    for (int delta : {-1,0,1}) check(k*k+delta);

  mt19937 rng(190019);
  for (int tc = 0; tc < 250; ++tc)
    check(1 + static_cast<int>(rng() % 1000000000u));

  // Consecutive top-of-domain inputs exercise the final r+1 increment.
  for (int delta = 1; delta <= 64; ++delta) check(INT_MAX-delta);
  // k*(k+1) is the other transition of the square-root shortcut.
  for (int k : {1,2,31,32,1000,32767,46339,46340})
    for (int delta : {-1,0,1}) check(k*(k+1)+delta);

  cout << "PASS floor_enumeration actual snippet, zero, maximality, "
       << "and large n\n";
}
