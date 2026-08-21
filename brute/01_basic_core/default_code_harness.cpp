#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/1_Basic/Default_code.cpp"

static_assert(is_same_v<ll, long long>);
static_assert(is_same_v<pii, pair<int, int>>);
static_assert(is_same_v<pll, pair<long long, long long>>);

int main() {
  pii p{7, 11};
  assert(p.X == 7 && p.Y == 11);

  pll q{-(1LL << 40), 1LL << 40};
  assert(q.X == -(1LL << 40) && q.Y == (1LL << 40));

  vector<int> values{9, -2, 4, 4};
  values.pb(0);
  sort(ALL(values));
  assert(SZ(values) == 5);
  assert((values == vector<int>{-2, 0, 4, 4, 9}));

  array<int, 4> raw{3, 1, 2, 0};
  sort(ALL(raw));
  const array<int, 4> expected{0, 1, 2, 3};
  assert(raw == expected);

  cout << "PASS Default_code aliases/macros and array/vector boundaries\n";
}
