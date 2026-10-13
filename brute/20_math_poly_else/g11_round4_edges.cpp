#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define pb push_back
#include "../../codebook/9_Else/NQueens.cpp"

void require(bool ok, const char *msg) {
  if (!ok) { cerr << msg << '\n'; exit(1); }
}
void check(int n) {
  vector<int> prefix = {-7, INT_MIN, 5}, q = prefix;
  q.shrink_to_fit(); // Appending must tolerate reallocation.
  solve(q, n);
  require(equal(prefix.begin(), prefix.end(), q.begin()),
          "NQueens changed existing prefix");
  int added = n == 2 || n == 3 ? 0 : n;
  require(q.size() == prefix.size() + added, "NQueens append size");
  vector<bool> col(n + 1), down(2 * n + 1), up(2 * n + 1);
  for (int i = 0; i < added; ++i) {
    int x = q[prefix.size() + i];
    require(1 <= x && x <= n && !col[x], "NQueens column conflict");
    require(!down[i - x + n] && !up[i + x],
            "NQueens diagonal conflict");
    col[x] = down[i - x + n] = up[i + x] = true;
  }
  auto saved = q;
  solve(q, 0), solve(q, 2), solve(q, 3);
  require(q == saved, "NQueens empty / impossible append changed state");
}
int main() {
  for (int n = 0; n <= 2000; ++n) check(n);
  for (int n = 1000000; n < 1000003; ++n) check(n);
  vector<int> q;
  for (int n : {8, 9, 10, 1}) {
    auto saved = q;
    vector<int> one;
    solve(one, n), solve(q, n);
    saved.insert(saved.end(), one.begin(), one.end());
    require(q == saved, "NQueens repeated append changes earlier segment");
  }
  cout << "g11 round4 edges PASS: NQueens\n";
}
