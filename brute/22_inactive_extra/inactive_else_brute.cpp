#include <bits/stdc++.h>
using namespace std;
using ll = long long;

static void require(bool condition, const string &message) {
  if (!condition) {
    cerr << "FAIL " << message << '\n';
    exit(1);
  }
}

namespace dlx_template {
constexpr int NN = 2048;
constexpr int RR = 32;
#include "../../codebook/9_Else/DLX.cpp"
}  // namespace dlx_template

namespace cht_template {
#include "../../codebook/9_Else/DynamicConvexTrick_bb.cpp"
}  // namespace cht_template

namespace subset_template {
#include "../../codebook/9_Else/SubsetSum.cpp"
}  // namespace subset_template

namespace cyclic_template {
constexpr int MAXL = 24;
#include "../../codebook/9_Else/cyclicLCS.cpp"
}  // namespace cyclic_template

namespace manhattan_template {
struct Point {
  long long x = 0, y = 0;
  int u = -1;
  bool operator<(const Point &other) const {
    if (x != other.x) return x > other.x;
    if (y != other.y) return y < other.y;
    return u < other.u;
  }
};
struct Edge { int u, v; long long w; };
vector<Edge> es;
long long dist(const Point &a, const Point &b) {
  return llabs(a.x - b.x) + llabs(a.y - b.y);
}
#include "../../codebook/9_Else/ManhattanMST.cpp"
}  // namespace manhattan_template

static int brute_cover(const vector<vector<int>> &rows, int columns, bool exact) {
  int answer = INT_MAX;
  for (int mask = 0; mask < (1 << rows.size()); ++mask) {
    if (__builtin_popcount((unsigned)mask) >= answer) continue;
    vector<int> count(columns);
    for (int row = 0; row < (int)rows.size(); ++row) if (mask >> row & 1)
      for (int column : rows[row]) ++count[column];
    bool ok = true;
    for (int value : count) ok &= exact ? value == 1 : value >= 1;
    if (ok) answer = __builtin_popcount((unsigned)mask);
  }
  return answer;
}

static void test_dlx() {
  mt19937 rng(40291);
  for (bool exact : {false, true}) {
    for (int test = 0; test < 3000; ++test) {
      int columns = 1 + rng() % 7, row_count = rng() % 13;
      vector<vector<int>> rows(row_count);
      for (auto &row : rows) for (int c = 0; c < columns; ++c)
        if (rng() % 3 == 0) row.push_back(c);
      int want = brute_cover(rows, columns, exact);
      int got;
      if (exact) {
        dlx_template::DLX<true> solver;
        solver.init(columns);
        for (const auto &row : rows) solver.insert(row);
        got = solver.solve();
      } else {
        dlx_template::DLX<false> solver;
        solver.init(columns);
        for (const auto &row : rows) solver.insert(row);
        got = solver.solve();
      }
      require((got == (int)1e9) == (want == INT_MAX), "DLX feasibility");
      if (want != INT_MAX) require(got == want, exact ? "DLX exact" : "DLX cover");
    }
  }
}

static void test_dynamic_cht() {
  mt19937_64 rng(78123);
  for (int test = 0; test < 1000; ++test) {
    cht_template::DynamicConvexTrick<long long> hull;
    vector<pair<long long, long long>> lines;
    int count = 1 + rng() % 80;
    for (int i = 0; i < count; ++i) {
      long long a = (long long)(rng() % 81) - 40;
      long long b = (long long)(rng() % 201) - 100;
      hull.ins(a, b);
      lines.emplace_back(a, b);
      for (int q = 0; q < 10; ++q) {
        long long x = rng() % 101;
        long long want = LLONG_MIN;
        for (auto [slope, intercept] : lines) want = max(want, slope * x + intercept);
        long long got = hull(x);
        if (got != want) {
          cerr << "CHT mismatch test=" << test << " inserted=" << i + 1
               << " x=" << x << " got=" << got << " want=" << want << '\n';
          for (auto [slope, intercept] : lines) cerr << slope << ' ' << intercept << '\n';
          require(false, "dynamic CHT maximum");
        }
      }
    }
  }
}

static void test_subset_sum() {
  mt19937 rng(99881);
  constexpr int S = 256;
  for (int test = 0; test < 5000; ++test) {
    vector<int> values;
    int remaining = S - 1, n = rng() % 35;
    for (int i = 0; i < n; ++i) {
      int value = remaining ? rng() % (min(20, remaining) + 1) : 0;
      values.push_back(value), remaining -= value;
    }
    auto got = subset_template::SubsetSum<S>(values.data(), values.size());
    bitset<S> want; want[0] = 1;
    for (int value : values) want |= want << value;
    require(got == want, "subset sum grouped multiplicities");
  }
}

static int lcs(string a, const string &b) {
  vector<int> previous(b.size() + 1), current(b.size() + 1);
  for (char x : a) {
    for (int j = 1; j <= (int)b.size(); ++j)
      current[j] = x == b[j - 1] ? previous[j - 1] + 1
                                  : max(previous[j], current[j - 1]);
    swap(previous, current);
    fill(current.begin(), current.end(), 0);
  }
  return previous.back();
}

static int brute_cyclic_lcs(const string &a, const string &b) {
  int answer = 0;
  if (a.empty() || b.empty()) return 0;
  for (int shift = 0; shift < (int)a.size(); ++shift)
    answer = max(answer, lcs(a.substr(shift) + a.substr(0, shift), b));
  for (int shift = 0; shift < (int)b.size(); ++shift)
    answer = max(answer, lcs(a, b.substr(shift) + b.substr(0, shift)));
  return answer;
}

static void test_cyclic_lcs() {
  for (int na = 1; na <= 8; ++na) for (int nb = 1; nb <= 8; ++nb) {
    for (int ma = 0; ma < (1 << na); ++ma) for (int mb = 0; mb < (1 << nb); ++mb) {
      string a(na, 'a'), b(nb, 'a');
      for (int i = 0; i < na; ++i) if (ma >> i & 1) a[i] = 'b';
      for (int i = 0; i < nb; ++i) if (mb >> i & 1) b[i] = 'b';
      cyclic_template::al = na, cyclic_template::bl = nb;
      strcpy(cyclic_template::a, a.c_str());
      strcpy(cyclic_template::b, b.c_str());
      require(cyclic_template::cyclic_lcs() == brute_cyclic_lcs(a, b), "cyclic LCS");
    }
  }
}

struct DSU {
  vector<int> p;
  explicit DSU(int n) : p(n, -1) {}
  int find(int x) { return p[x] < 0 ? x : p[x] = find(p[x]); }
  bool merge(int a, int b) {
    a = find(a), b = find(b); if (a == b) return false;
    if (p[a] > p[b]) swap(a, b);
    p[a] += p[b], p[b] = a; return true;
  }
};

static long long kruskal(int n, vector<manhattan_template::Edge> edges) {
  sort(edges.begin(), edges.end(), [](auto a, auto b) { return a.w < b.w; });
  DSU dsu(n); long long answer = 0; int used = 0;
  for (auto edge : edges) if (dsu.merge(edge.u, edge.v)) answer += edge.w, ++used;
  return used == n - 1 ? answer : LLONG_MAX;
}

static void test_manhattan_mst() {
  mt19937 rng(556677);
  for (int test = 0; test < 3000; ++test) {
    int n = 1 + rng() % 30;
    vector<manhattan_template::Point> points(n), transformed;
    for (int i = 0; i < n; ++i)
      points[i] = {(int)(rng() % 61) - 30, (int)(rng() % 61) - 30, i};
    transformed = points;
    manhattan_template::es.clear();
    manhattan_template::MST(transformed.data(), n);
    vector<manhattan_template::Edge> complete;
    for (int i = 0; i < n; ++i) for (int j = i + 1; j < n; ++j)
      complete.push_back({i, j, manhattan_template::dist(points[i], points[j])});
    long long got = kruskal(n, manhattan_template::es), want = kruskal(n, complete);
    if (got != want) {
      cerr << "Manhattan mismatch test=" << test << " n=" << n
           << " candidates=" << manhattan_template::es.size()
           << " got=" << got << " want=" << want << '\n';
      for (auto p : points) cerr << p.x << ' ' << p.y << '\n';
      require(false, "Manhattan MST");
    }
  }
}

int main() {
  test_dlx();
  test_dynamic_cht();
  test_subset_sum();
  test_cyclic_lcs();
  test_manhattan_mst();
  cout << "inactive else PASS\n";
}
