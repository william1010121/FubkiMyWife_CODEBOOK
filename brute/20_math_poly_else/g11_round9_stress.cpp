#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define SZ(a) ((int)a.size())
#define ALL(v) v.begin(), v.end()
#define pb push_back
#include "../../codebook/9_Else/CDQ_Dominance.cpp"
#include "../../codebook/9_Else/All_LCS.cpp"
#include "../../codebook/9_Else/BitsetLCS.cpp"
namespace queens {
#include "../../codebook/9_Else/NQueens.cpp"
}
void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
int main() {
  const int n = 200000;
  vector<vector<int>> p(n, vector<int>(4));
  // Unique first coordinate, ties in every later coordinate.
  for (int i = 0; i < n; ++i) p[i] = {i, 0, 0, 0};
  mt19937 rng(9011); shuffle(ALL(p), rng);
  auto counts = dominance(p);
  for (int i = 0; i < n; ++i)
    require(counts[i] == p[i][0], "CDQ tied update/query ordering");
  require(all_lcs(string(2000, 'a'), string(1000, 'a'))
            == vector<string>{string(1000, 'a')}, "rectangular memo");
  require(all_lcs(string(1000, 'a'), string(2000, 'a'))
            == vector<string>{string(1000, 'a')}, "transposed memo");
  vector<int> a(n, INT_MIN), b(n + 1, INT_MIN);
  require(bitset_lcs(a, b) == n, "200k multiword carry/borrow");
  for (int i = 0; i < n; ++i) a[i] = i % 2;
  for (int i = 0; i <= n; ++i) b[i] = (i + 1) % 2;
  require(bitset_lcs(a, b) == n, "200k shifted alternating LCS");
  vector<int> ret;
  for (int size = 0; size <= 1000; ++size) {
    auto old = ret;
    queens::solve(ret, size);
    int offset = SZ(old), added = size == 2 || size == 3 ? 0 : size;
    require(SZ(ret) == offset + added, "queens append size");
    require(equal(ALL(old), ret.begin()), "queens prefix preserved");
    vector<char> columns(size + 1), down(2 * size), up(2 * size);
    for (int i = 0; i < added; ++i) {
      int col = ret[offset + i];
      require(col >= 1 && col <= size, "queens column bounds");
      require(!columns[col]++ && !down[i-col+size]++ && !up[i+col-1]++,
              "queens permutation and diagonals");
    }
  }
  cout << "g11 round9 stress PASS\n";
}
