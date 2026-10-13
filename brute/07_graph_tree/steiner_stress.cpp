#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int N = 100, T = 12, INF = 1000000000;
#define SZ(v) ((int)(v).size())
#define ALL(v) (v).begin(), (v).end()
#include "../../codebook/2_Graph/MinimumSteinerTree.cpp"

int main() {
  static SteinerTree st;
  st.init(N);
  vector<int> terminals(T);
  iota(terminals.begin(), terminals.end(), 0);
  int want = 7;
  st.vcst[T] = 7;
  for (int i = 0; i < T; ++i) {
    st.vcst[i] = i % 3;
    st.add_edge(i, T, i + 1);
    want += i + 1 + st.vcst[i];
  }
  // Extra vertices cannot reach a common sink more cheaply.
  for (int i = T + 1; i < N; ++i)
    for (int j = 0; j < T; ++j) st.add_edge(j, i, 1000);
  if (st.solve(terminals) != want)
    throw runtime_error("large Steiner star");
  st.init(N);
  if (st.solve(terminals) != INF)
    throw runtime_error("large disconnected Steiner graph");
  cout << "PASS Steiner n=100 t=12 star/disconnected stress\n";
}
