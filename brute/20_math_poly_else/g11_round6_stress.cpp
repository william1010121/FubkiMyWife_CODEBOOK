#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
int main() {
  const int n = 200000;
  vector<vector<int>> p(n, vector<int>(3));
  for (int i = 0; i < n; ++i) fill(ALL(p[i]), i / 3);
  reverse(ALL(p));
  auto ans = dominance(p);
  for (int i = 0; i < n; ++i) {
    int group = (n - i - 1) / 3;
    require(ans[i] == min(n, 3 * (group + 1)) - 1,
            "CDQ duplicate weights at contest scale");
  }
  for (int i = 0; i < n; ++i) p[i] = {i, n-i, i%997, i%31};
  ans = dominance(p);
  require(ans == vector<ll>(n, 0), "4D contest-scale antichain");
  require(all_lcs(string(2000, 'a'), string(2000, 'b'))
            == vector<string>{""}, "large zero-length LCS");
  require(all_lcs(string(2000, 'a'), string(2000, 'a'))
            == vector<string>{string(2000, 'a')}, "large single LCS");
  cout << "g11 round6 stress PASS\n";
}
