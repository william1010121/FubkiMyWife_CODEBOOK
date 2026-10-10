#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
#include "../../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"

static void require(bool ok, const char *what) {
  if (!ok) { cerr << "FAIL " << what << '\n'; exit(1); }
}

static void check_weighted(WeightGraph &g, ll want, int count) {
  require(g.solve() == make_pair(want, count), "weighted optimum");
  ll sum = 0; int pairs = 0;
  for (int u = 1; u <= g.n; ++u) if (int v = g.match[u]) {
    require(v > 0 && v <= g.n && v != u && g.match[v] == u,
            "weighted matching validity");
    if (u < v) sum += g.g[u][v].w, ++pairs;
  }
  require(sum == want && pairs == count, "weighted sum/count");
}

int main() {
  auto start = chrono::steady_clock::now();
  Matching simple(801);
  for (int u = 0; u < simple.n; ++u)
    for (int v = u + 1; v < simple.n; ++v) simple.add_edge(u, v);
  require(simple.solve() == simple.n / 2, "odd simple clique");
  int pairs = 0;
  for (int u = 0; u < simple.n; ++u) if (int v = simple.match[u];
      v != simple.n) {
    require(v >= 0 && v < simple.n && v != u && simple.match[v] == u,
            "simple matching validity");
    pairs += u < v;
  }
  require(pairs == simple.n / 2, "simple count");

  WeightGraph cycle(601);
  for (int u = 1; u <= cycle.n; ++u)
    cycle.add_edge(u, u % cycle.n + 1, 7);
  check_weighted(cycle, 7LL * (cycle.n / 2), cycle.n / 2);
  WeightGraph clique(301);
  for (int u = 1; u <= clique.n; ++u)
    for (int v = u + 1; v <= clique.n; ++v) clique.add_edge(u, v, 3);
  check_weighted(clique, 3LL * (clique.n / 2), clique.n / 2);
  WeightGraph planted(200);
  for (int u = 1; u <= planted.n; ++u)
    for (int v = u + 1; v <= planted.n; ++v)
      planted.add_edge(u, v, u % 2 && v == u + 1 ? 100000000 : 1);
  check_weighted(planted, 100000000LL * (planted.n / 2),
                 planted.n / 2);
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "PASS matching stress: odd clique/cycle, planted weights"
       << " seconds=" << seconds << '\n';
}
