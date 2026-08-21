#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/6_Math/floor_ceil.cpp"

int main() {
  long long cases = 0;
  auto check = [&](int a, int b) {
    if (a == numeric_limits<int>::min() && b == -1) return;
    long double x = static_cast<long double>(a) / static_cast<long double>(b);
    long long want_floor = static_cast<long long>(std::floor(x));
    long long want_ceil = static_cast<long long>(std::ceil(x));
    if (::floor(a, b) != want_floor || ::ceil(a, b) != want_ceil) {
      cerr << "floor/ceil mismatch a=" << a << " b=" << b
           << " got=" << ::floor(a, b) << "," << ::ceil(a, b)
           << " want=" << want_floor << "," << want_ceil << '\n';
      exit(1);
    }
    ++cases;
  };

  for (int a = -500; a <= 500; ++a)
    for (int b = -500; b <= 500; ++b)
      if (b != 0) check(a, b);

  const vector<int> edge = {
      numeric_limits<int>::min(), numeric_limits<int>::min() + 1,
      -2000000001, -1000000000, -999999937, -2, -1, 0, 1, 2,
      999999937, 1000000000, 2000000001,
      numeric_limits<int>::max() - 1, numeric_limits<int>::max()};
  for (int a : edge) for (int b : edge) if (b != 0) check(a, b);

  cout << "PASS floor_ceil signed exhaustive and int-boundary cases: "
       << cases << '\n';
}
