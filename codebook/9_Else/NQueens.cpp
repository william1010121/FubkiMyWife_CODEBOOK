// n>=0; appends 1-based columns; O(n).
void solve(vector<int> &ret, int n) {
  if (n == 2 || n == 3) return; // No solution.
  auto offset = ret.size(); if (!offset) ret.reserve(n);
  for (ll i = 2; i <= n; i += 2) ret.pb(i); // Evens.
  for (ll i = 1; i <= n; i += 2) ret.pb(i);
  auto first = ret.begin() + offset, odd = first + n / 2;
  if (n % 6 == 2) {
    swap(odd[0], odd[1]);
    rotate(odd + 2, odd + 3, ret.end());
  } else if (n % 6 == 3) {
    rotate(first, first + 1, odd);
    rotate(odd, odd + 2, ret.end());
  }
}
