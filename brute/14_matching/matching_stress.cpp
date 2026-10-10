#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/4_Flow_Matching/Maximum_Simple_Graph_Matching.cpp"
#include "../../codebook/4_Flow_Matching/Maximum_Weight_Matching.cpp"

static void require(bool ok, const char *what) {
  if (!ok) { cerr << "FAIL " << what << '\n'; exit(1); }
}

static void check_weighted(WeightGraph &g, ll want, int count) {
  // Repeat with the same object; odd graphs leave contracted blossoms.
  for (int repeat = 0; repeat < 2; ++repeat) {
    require(g.solve() == make_pair(want, count), "weighted optimum");
    ll sum = 0; int pairs = 0;
    for (int u = 1; u <= g.n; ++u) if (int v = g.match[u]) {
      require(v > 0 && v <= g.n && v != u && g.match[v] == u,
              "weighted matching validity");
      if (u < v) sum += g.g[u][v].w, ++pairs;
    }
    require(sum == want && pairs == count, "weighted sum/count");
  }
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

  Matching path(20000);
  for (int u = 0; u + 1 < path.n; ++u) path.add_edge(u, u + 1);
  require(path.solve() == path.n / 2, "long simple path");
  for (int u = 0; u < path.n; ++u)
    require(path.match[u] == (u ^ 1), "path matching validity");
  require(path.solve() == 0, "path repeated solve");

  Matching shifted(20000);
  for (int u = 1; u + 1 < shifted.n; ++u) shifted.add_edge(u, u + 1);
  require(shifted.solve() == shifted.n / 2 - 1, "shifted path seeds");
  shifted.add_edge(0, 1);
  require(shifted.solve() == 1, "long alternating augmentation");
  for (int u = 0; u < shifted.n; ++u)
    require(shifted.match[u] == (u ^ 1), "long rerouting validity");
  require(shifted.solve() == 0, "long rerouting repeated solve");

  Matching barbell(402);
  for (int base : {0, 201})
    for (int u = base; u < base + 201; ++u)
      for (int v = u + 1; v < base + 201; ++v) barbell.add_edge(u, v);
  require(barbell.solve() == 200, "two odd simple cliques");
  int a = 0, b = 201;
  while (barbell.match[a] != barbell.n) ++a;
  while (barbell.match[b] != barbell.n) ++b;
  barbell.add_edge(a, b);
  require(barbell.solve() == 1, "simple blossom reuse after bridge");
  for (int u = 0; u < barbell.n; ++u) {
    int v = barbell.match[u];
    require(v < barbell.n && barbell.match[v] == u &&
            find(ALL(barbell.G[u]), v) != barbell.G[u].end(),
            "simple bridged matching validity");
  }
  require(barbell.solve() == 0, "simple bridge repeated solve");

  Matching reroute(4);
  reroute.add_edge(0, 1); // The first greedy edge must be undone.
  reroute.add_edge(0, 2); reroute.add_edge(1, 3);
  require(reroute.solve() == 2, "simple greedy rerouting");
  require(reroute.match == vector<int>({2, 3, 0, 1, 4}),
          "simple rerouting validity");

  WeightGraph weighted_reroute(4);
  weighted_reroute.add_edge(1, 2, 10); // A tight greedy edge.
  weighted_reroute.add_edge(1, 3, 9);
  weighted_reroute.add_edge(2, 4, 9);
  check_weighted(weighted_reroute, 18, 2);

  WeightGraph weighted_barbell(102);
  for (int base : {1, 52})
    for (int u = base; u < base + 51; ++u)
      for (int v = u + 1; v < base + 51; ++v)
        weighted_barbell.add_edge(u, v, 7);
  check_weighted(weighted_barbell, 350, 50);
  // A low-weight bridge must reroute maximum-weight seeds in both cliques.
  weighted_barbell.add_edge(1, 52, 1);
  check_weighted(weighted_barbell, 351, 51);
  weighted_barbell.add_edge(1, 52, -1);
  check_weighted(weighted_barbell, 350, 50);
  weighted_barbell.add_edge(2, 53, 1);
  check_weighted(weighted_barbell, 351, 51);

  WeightGraph cycle(601);
  for (int u = 1; u <= cycle.n; ++u)
    cycle.add_edge(u, u % cycle.n + 1, 7);
  check_weighted(cycle, 7LL * (cycle.n / 2), cycle.n / 2);
  WeightGraph clique(301);
  for (int u = 1; u <= clique.n; ++u)
    for (int v = u + 1; v <= clique.n; ++v) clique.add_edge(u, v, 3);
  check_weighted(clique, 3LL * (clique.n / 2), clique.n / 2);
  for (int u = 1; u <= clique.n; ++u)
    for (int v = u + 1; v <= clique.n; ++v) clique.add_edge(u, v, -1);
  check_weighted(clique, 0, 0); // Overwrites after a blossom search.
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
