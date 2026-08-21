#define VORONOI_LIBRARY_ONLY
#include "../../codebook/8_Geometry/Triangulation_Vonoroi.cpp"
#include "../../codebook/8_Geometry/Triangulation_Vonoroi_short.cpp"

static void require(bool condition, const std::string &message) {
  if (!condition) {
    std::cerr << "FAIL " << message << '\n';
    std::exit(1);
  }
}

static std::vector<VoronoiPoint> clip(std::vector<VoronoiPoint> polygon,
                                      long double a, long double b, long double c) {
  std::vector<VoronoiPoint> result;
  auto value = [&](VoronoiPoint p) { return a * p.x + b * p.y - c; };
  for (int i = 0; i < (int)polygon.size(); ++i) {
    VoronoiPoint p = polygon[i], q = polygon[(i + 1) % polygon.size()];
    long double vp = value(p), vq = value(q);
    bool pin = vp <= 1e-10L, qin = vq <= 1e-10L;
    if (pin) result.push_back(p);
    if (pin != qin) {
      long double ratio = vp / (vp - vq);
      result.push_back({p.x + (q.x - p.x) * ratio,
                        p.y + (q.y - p.y) * ratio});
    }
  }
  return result;
}

static long double clipped_oracle(const std::vector<VoronoiPoint> &points, int site) {
  constexpr long double bound = 100000;
  std::vector<VoronoiPoint> polygon = {
      {-bound, -bound}, {bound, -bound}, {bound, bound}, {-bound, bound}};
  for (int j = 0; j < (int)points.size(); ++j) if (j != site) {
    long double a = 2 * (points[j].x - points[site].x);
    long double b = 2 * (points[j].y - points[site].y);
    long double c = points[j].x * points[j].x + points[j].y * points[j].y -
                    points[site].x * points[site].x - points[site].y * points[site].y;
    polygon = clip(std::move(polygon), a, b, c);
  }
  for (auto p : polygon)
    if (fabsl(fabsl(p.x) - bound) < 1e-6L || fabsl(fabsl(p.y) - bound) < 1e-6L)
      return -1;
  long double twice_area = 0;
  for (int i = 0; i < (int)polygon.size(); ++i) {
    auto a = polygon[i], b = polygon[(i + 1) % polygon.size()];
    twice_area += a.x * b.y - a.y * b.x;
  }
  return fabsl(twice_area) / 2;
}

int main() {
  auto triangle = solve_voronoi({{0, 0}, {4, 0}, {0, 3}});
  require(triangle == std::vector<long double>({-1, -1, -1}), "triangle cells unbounded");
  auto centered = solve_voronoi({{0, 0}, {2, 0}, {2, 2}, {0, 2}, {1, 1}});
  require(centered.size() == 5, "centered size");
  for (int i = 0; i < 4; ++i) require(centered[i] == -1, "hull cell unbounded");
  require(fabsl(centered[4] - 2) < 1e-10L, "center diamond area");

  std::mt19937 rng(11235813);
  for (int test = 0; test < 1500; ++test) {
    int n = 3 + rng() % 9;
    std::set<std::pair<int, int>> selected;
    while ((int)selected.size() < n)
      selected.emplace((int)(rng() % 41) - 20, (int)(rng() % 41) - 20);
    std::vector<VoronoiPoint> points;
    for (auto [x, y] : selected) points.push_back({(long double)x, (long double)y});
    auto got = solve_voronoi(points);
    for (int i = 0; i < n; ++i) {
      long double want = clipped_oracle(points, i);
      require((got[i] < 0) == (want < 0), "Voronoi bounded classification");
      if (want >= 0) {
        long double scale = 1 + want;
        require(fabsl(got[i] - want) < 1e-8L * scale, "Voronoi finite area");
      }
    }
  }
  std::cout << "Voronoi PASS\n";
}
