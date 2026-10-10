#include "codebook/1_Basic/Default_code.cpp"
constexpr int N = 200000;
constexpr ll INF = 1LL << 62;
#include "codebook/3_Data_Structure/LiChaoST.cpp"
#include "codebook/4_Flow_Matching/Bipartite_Matching.cpp"

int main() {
  auto bm = make_unique<Bipartite_Matching>();
  // N free left vertices plus the free-right sentinel fill the BFS queue.
  bm->init(N, 1);
  for (int u = 0; u < N; ++u) bm->add_edge(u, 0);
  assert(bm->matching() == 1 && bm->matching() == 1);
  // 20 long alternating paths, within the documented DFS stack bound.
  bm->init(N, N);
  for (int u = 0; u < N; ++u) {
    if (u % 10000 != 9999) bm->add_edge(u, u + 1);
    bm->add_edge(u, u);
  }
  assert(bm->matching() == N && bm->matching() == N);
  for (int u = 0; u < N; ++u) assert(bm->mq[bm->mp[u]] == u);
  // Every line wins somewhere; non-power-of-two domain, shuffled queries.
  const int n = N + 3;
  vector<int> order(n); iota(ALL(order), 0);
  mt19937 rng(1207); shuffle(ALL(order), rng);
  LiChao hull(n);
  for (int i : order) hull.insert(L(2LL * i, -1LL * i * i, i));
  for (int x : order) assert(hull.query(x) == 1LL * x * x);
  for (int empty : {0, -1, INT_MIN}) {
    LiChao h(empty); h.insert(L(1, 2, 0));
    assert(h.query(LLONG_MIN) == -INF);
    assert(h.query(LLONG_MAX) == -INF);
  }
  cout << "round12 stress: PASS\n";
}
