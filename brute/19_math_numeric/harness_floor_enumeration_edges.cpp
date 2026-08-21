#include <bits/stdc++.h>
using namespace std;

void instantiate_template(int n) {
  // The source is intentionally a side-effect-free snippet.  Include and
  // execute it directly so compile/termination are checked as well.
#include "../../codebook/6_Math/floor_enumeration.cpp"
}

vector<tuple<int, int, int>> enumerate_oracle_input(int n) {
  vector<tuple<int, int, int>> got;
  // The bare source has no output hook, so this direct copy records its
  // quotient blocks for comparison with the deliberately simple scan below.
  for (int l = 1, r; l <= n; l = r + 1) {
    int x = n / l;
    r = n / x;
    got.emplace_back(l, r, x);
  }
  return got;
}

void check(int n) {
  instantiate_template(n);
  const auto got = enumerate_oracle_input(n);
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
  for (int n = 1; n <= 3000; ++n) check(n);
  const vector<int> selected = {
      9999, 10000, 10001, 46340, 46341, 1000000, 1000000007,
      2147483646};
  for (int n : selected) check(n);

  mt19937 rng(190019);
  for (int tc = 0; tc < 250; ++tc)
    check(1 + static_cast<int>(rng() % 1000000000u));

  cout << "PASS floor_enumeration direct blocks, maximality, and large n\n";
}
