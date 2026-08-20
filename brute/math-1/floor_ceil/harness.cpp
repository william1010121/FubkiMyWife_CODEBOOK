#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/6_Math/floor_ceil.cpp"

long long flo(long long a, long long b) {
  long long q = a / b, r = a % b;
  return q - (r != 0 && ((a < 0) ^ (b < 0)));
}
long long cei(long long a, long long b) {
  long long q = a / b, r = a % b;
  return q + (r != 0 && ((a < 0) ^ (b > 0)));
}
int main() {
  for (int a = -200; a <= 200; ++a) for (int b = -30; b <= 30; ++b) {
    if (!b) continue;
    if (::floor(a, b) != flo(a, b) || ::ceil(a, b) != cei(a, b)) {
      cerr << "floor/ceil mismatch a=" << a << " b=" << b << '\n';
      return 1;
    }
  }
  cout << "PASS floor_ceil exhaustive signed range\n";
}
