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
  friend bool operator<=(int l, const observed_n &n) {
    return l <= n.value;
  }
  friend int operator/(observed_n &n, int divisor) {
    int result = n.value / divisor;
    if (!n.pending) n.left = divisor, n.quotient = result;
    else n.blocks.emplace_back(n.left, result, n.quotient);
    n.pending = !n.pending;
    return result;
  }
};
vector<tuple<int,int,int>> enumerate_oracle_input(int value) {
  // Observe the actual snippet's n/l and n/x divisions, using exactly
  // int arithmetic. No copied algorithm supplies the recorded blocks.
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

  mt19937 rng(190019);
  for (int tc = 0; tc < 250; ++tc)
    check(1 + static_cast<int>(rng() % 1000000000u));

  cout << "PASS floor_enumeration actual snippet, zero, maximality, "
       << "and large n\n";
}
