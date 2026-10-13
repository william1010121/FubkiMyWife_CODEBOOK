#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 800;
const ll INF = 1LL << 60;
#include "codebook/4_Flow_Matching/Kuhn_Munkres.cpp"
static KM km;
static void require(bool ok) { if (!ok) abort(); }
int main() {
  auto start = chrono::steady_clock::now();
  for (int mode = 0; mode < 3; ++mode) {
    km.init(N);
    for (int i = 0; i < N; ++i) for (int j = 0; j < N; ++j)
      km.add_edge(i, j, mode == 0 ? j :
                  mode == 1 ? -ll(i) : -ll(i) * j);
    ll want = mode == 0 ? N * (N - 1LL) / 2 :
              mode == 1 ? -N * (N - 1LL) / 2 :
              -N * (N - 1LL) * (N - 2LL) / 6;
    // Column ties, row ties, and reverse-order rearrangement optimum.
    for (int repeat = 0; repeat < 2; ++repeat) {
      require(km.solve() == want);
      // Exercise bfs's new left-root contract with feasible duals.
      int left = N / 2, right = km.fl[left];
      km.fl[left] = km.fr[right] = -1; km.bfs(left);
      vector<int> seen(N);
      ll sum = 0;
      for (int i = 0; i < N; ++i) {
        int j = km.fl[i];
        require(j >= 0 && j < N && !seen[j]++ && km.fr[j] == i);
        sum += km.w[i][j];
      }
      require(sum == want);
    }
  }
  cout << "PASS KM round9 stress: six 800x800 solves; "
       << chrono::duration<double>(
            chrono::steady_clock::now() - start).count() << " s\n";
}
