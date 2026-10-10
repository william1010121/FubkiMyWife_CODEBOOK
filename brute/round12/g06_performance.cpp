#include "../../codebook/1_Basic/Default_code.cpp"
#include "../../codebook/2_Graph/BCC_Vertex.cpp"
namespace circles {
constexpr double eps = 1e-8, pi = acos(-1.);
double sq(double x) { return x * x; }
struct P {
  double x, y;
  P operator-(P b) const { return {x - b.x, y - b.y}; }
  double abs() const { return hypot(x, y); }
  double angle() const { return atan2(y, x); }
};
struct C { P c; double r; };
bool same(double a, double b) { return abs(a - b) < eps; }
#include "../../codebook/8_Geometry/circleOrArea.cpp"
}
static void require(bool ok, const char *message) {
  if (!ok) throw runtime_error(message);
}
int main() {
  // 200k vertices but shallow DFS: no special stack limit required.
  const int n = 200000;
  BCC g(n);
  for (int i = 1; i < n; ++i) g.add_edge(0, i);
  g.solve();
  require(g.nbcc == n - 1 && g.dft == n, "star block count");
  require(g.is_ap[0] == 1, "star root articulation");
  vector<bool> seen(n);
  for (auto block : g.bcc) {
    require(block.size() == 2, "star edge block");
    sort(ALL(block));
    require(block[0] == 0 && !seen[block[1]], "star exact blocks");
    seen[block[1]] = true;
  }
  for (int i = 1; i < n; ++i)
    require(seen[i] && !g.is_ap[i], "star leaves");
  g.block_cut_tree();
  require(g.nbcc == n && g.nG[g.bln[0]].size() == n - 1,
          "star block-cut tree");
  size_t arcs = 0;
  for (auto &adj : g.nG) arcs += adj.size();
  require(arcs == 2 * (n - 1), "star block-cut arcs");

  using namespace circles;
  const int k = 2000;
  for (int orientation = 0; orientation < 4; ++orientation) {
    vector<C> cs;
    for (int i = 0; i < k; ++i) {
      double x = orientation & 1 ? 3. * i : 0.;
      double y = orientation & 2 ? -3. * i : 3. * i;
      cs.pb({{x + 1e7, y - 1e7}, 1.});
    }
    double got = CircleUnionArea(cs);
    require(abs(got - k * pi) < 1e-5, "disjoint circle stress area");
  }
  // The fast rejection must preserve the caller's interval buffer.
  vector<pair<double, double>> segments{{0.1, 0.2}};
  C a{{0., 0.}, 1.}, b{{0., 3.}, 1.};
  AppendCoverSegment(a, b, segments);
  require(segments == vector<pair<double, double>>{{0.1, 0.2}},
          "disjoint circle append preserves intervals");
  cout << "PASS round12 BCC star200k, circle union2000\n";
}
