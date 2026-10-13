#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 1LL << 62;
#include "../../codebook/3_Data_Structure/LiChaoST.cpp"

int main() {
  const int n = 200000;
  LiChao hull(n);
  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  mt19937 rng(606);
  shuffle(order.begin(), order.end(), rng);
  // max_i(2*i*x-i*i) = x*x for each domain integer x.
  for (int i : order) hull.insert(L(2LL * i, -1LL * i * i, i));
  for (int x = 0; x < n; ++x) assert(hull.query(x) == 1LL * x * x);
  LiChao dominated(n);
  dominated.insert(L(0, 0, 0));
  for (int i = 1; i <= n; ++i) {
    ll m = 1 + rng() % 200000;
    dominated.insert(L(m, -m * (n - 1) - 1, i));
  }
  for (int x = 0; x < n; ++x) assert(dominated.query(x) == 0);
  cout << "lichao scale: PASS\n";
}
