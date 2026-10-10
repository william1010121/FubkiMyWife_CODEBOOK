#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
int main() {
  mt19937 rng(1505);
  // Independent 2D Fenwick sweep: distinct first coordinate orders points.
  vector<vector<int>> a(200000, vector<int>(3));
  vector<ll> want(a.size()); int bit[129][9]{};
  for (int i = 0; i < SZ(a); ++i) {
    int y = rng() % 128 + 1, z = rng() % 8 + 1;
    a[i] = {i, y, z};
    for (int j = y; j; j -= j & -j)
      for (int k = z; k; k -= k & -k) want[i] += bit[j][k];
    for (int j = y; j <= 128; j += j & -j)
      for (int k = z; k <= 8; k += k & -k) ++bit[j][k];
  }
  shuffle(ALL(a), rng);
  auto ans = dominance(a);
  for (int i = 0; i < SZ(a); ++i) assert(ans[i] == want[a[i][0]]);
  // Insert a redundant coordinate to exercise another CDQ layer.
  for (auto &p : a) p.insert(p.begin() + 1, p[0]);
  ans = dominance(a);
  for (int i = 0; i < SZ(a); ++i) assert(ans[i] == want[a[i][0]]);
  cout << "PASS 200k tied 3D/4D independent sweep oracle\n";
}
