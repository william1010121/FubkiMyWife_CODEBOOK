#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
#include "../../codebook/9_Else/digit_dp.cpp"
void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
int main() {
  const int n = 200000;
  require(dominance(vector<vector<int>>(n)) == vector<ll>(n, n-1),
          "zero-dimensional contest-scale dominance");
  mt19937 rng(7011);
  for (bool duplicates : {false, true}) {
    vector<int> values(n);
    for (int i = 0; i < n; ++i) values[i] = duplicates ? i%997 : i;
    values[0] = INT_MIN; values.back() = INT_MAX;
    shuffle(ALL(values), rng);
    vector<vector<int>> points;
    for (int value : values) points.pb({value});
    auto got = dominance(points);
    sort(ALL(values));
    for (int i = 0; i < n; ++i)
      require(got[i] == upper_bound(ALL(values), points[i][0])
                         - values.begin() - 1,
              "one-dimensional ties and signed extremes");
  }
  require(calc(INT_MIN) == 0 && calc(INT_MAX) == 3524578,
          "digit signed boundaries");
  for (int i = 0; i <= 30; ++i) {
    int x = 1 << i;
    require(calc(x) == calc(x-1) + 1, "isolated high bit");
    if (i) require(calc(x + x/2) == calc(x + x/2 - 1),
                   "adjacent high bits");
  }
  cout << "g11 round7 stress PASS\n";
}
