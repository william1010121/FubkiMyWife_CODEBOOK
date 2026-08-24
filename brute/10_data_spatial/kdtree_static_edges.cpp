#include <bits/stdc++.h>
using namespace std;

const int maxn = 1024;
struct point {
  int x, y;
};

#include "../../codebook/3_Data_Structure/KDTree.cpp"

static long long expected_distance(
    const vector<point> &points, point q) {
  long long ans = LLONG_MAX;
  for (point p : points) {
    __int128 dx = (__int128)p.x - q.x;
    __int128 dy = (__int128)p.y - q.y;
    __int128 wide = dx * dx + dy * dy;
    long long d = wide > LLONG_MAX ? LLONG_MAX : (long long)wide;
    // KDTree.cpp intentionally excludes the query point itself.
    if (d) ans = min(ans, d);
  }
  return ans;
}

static void check_case(
    const vector<point> &points,
    const vector<point> &queries,
    const string &name) {
  if (points.empty() || (int)points.size() > maxn)
    throw runtime_error("invalid static KD-tree case");
  kdt::init(points);
  for (size_t i = 0; i < queries.size(); ++i) {
    long long got = kdt::nearest(queries[i]);
    long long want = expected_distance(points, queries[i]);
    if (got != want) {
      cerr << "KDTree mismatch case=" << name
           << " query=" << i << " got=" << got
           << " want=" << want << " points=" << points.size()
           << " q=(" << queries[i].x << ',' << queries[i].y << ")\n";
      exit(1);
    }
  }
}

int main() {
  kdt::init({});
  if (kdt::nearest({0, 0}) != LLONG_MAX) {
    cerr << "KDTree empty tree did not return sentinel\n";
    return 1;
  }

  vector<point> queries;
  for (int x = -4; x <= 4; ++x)
    for (int y = -4; y <= 4; ++y)
      queries.push_back({x, y});
  queries.insert(queries.end(), {
      {-1000000, -1000000}, {-1000000, 1000000},
      {1000000, -1000000}, {1000000, 1000000},
      {0, 0}, {1, -1}, {-1, 1}});

  check_case({{0, 0}}, queries, "one-point");

  vector<point> duplicate(64, {-17, 23});
  check_case(duplicate, queries, "all-duplicates");
  check_case(duplicate, {{-17, 23}}, "all-duplicates-sentinel");

  vector<point> grid;
  for (int x = -3; x <= 3; ++x)
    for (int y = -3; y <= 3; ++y) {
      grid.push_back({x, y});
      if ((x + y) % 3 == 0) grid.push_back({x, y});
    }
  check_case(grid, queries, "duplicate-grid");

  check_case({
      {-1000000, -1000000}, {-1000000, 1000000},
      {1000000, -1000000}, {1000000, 1000000},
      {0, 0}, {0, 0}, {-1, 1}, {1, -1}},
      queries, "coordinate-boundaries");

  check_case({{-1000000000, -1000000000}, {1000000000, 1000000000}},
             {{-1000000000, -1000000000}}, "8e18-distance");
  check_case({{-2000000000, -2000000000}, {2000000000, 2000000000}},
             {{-2000000000, -2000000000}}, "saturated-distance");

  mt19937 rng(0x51a7c0de);
  const int values[] = {
      -1000000, -999999, -1000, -17, -2, -1,
      0, 1, 2, 17, 1000, 999999, 1000000};
  for (int tc = 0; tc < 220; ++tc) {
    int n = 1 + (int)(rng() % 220);
    vector<point> points(n);
    for (point &p : points) {
      if (rng() % 3) {
        p.x = values[rng() % (sizeof(values) / sizeof(*values))];
        p.y = values[rng() % (sizeof(values) / sizeof(*values))];
      } else {
        p.x = (int)(rng() % 2000001) - 1000000;
        p.y = (int)(rng() % 2000001) - 1000000;
      }
    }
    vector<point> qs = queries;
    for (int i = 0; i < 120; ++i) {
      if (i % 4 == 0) {
        qs.push_back(points[rng() % points.size()]);
      } else {
        qs.push_back({(int)(rng() % 2000001) - 1000000,
                      (int)(rng() % 2000001) - 1000000});
      }
    }
    check_case(points, qs, "seeded-random-" + to_string(tc));
  }
  cout << "kdtree_static_edges: PASS\n";
}
