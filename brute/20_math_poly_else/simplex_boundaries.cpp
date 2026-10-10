#include <bits/stdc++.h>
using namespace std;

namespace tested {
#include "../../codebook/6_Math/Simplex_Algorithm.cpp"
}

struct Constraint { vector<double> a; double b; };

static bool solve_square(vector<Constraint> eq, vector<double> &x) {
  const int m = (int)x.size();
  for (int col = 0; col < m; ++col) {
    int row = col;
    while (row < m && abs(eq[row].a[col]) < 1e-10) ++row;
    if (row == m) return false;
    swap(eq[row], eq[col]);
    double z = eq[col].a[col];
    for (int j = col; j < m; ++j) eq[col].a[j] /= z;
    eq[col].b /= z;
    for (int i = 0; i < m; ++i) if (i != col) {
      z = eq[i].a[col];
      for (int j = col; j < m; ++j) eq[i].a[j] -= z * eq[col].a[j];
      eq[i].b -= z * eq[col].b;
    }
  }
  for (int i = 0; i < m; ++i) x[i] = eq[i].b;
  return true;
}

static optional<double> oracle(const vector<vector<double>> &A,
                               const vector<double> &b,
                               const vector<double> &c) {
  const int n = (int)A.size(), m = (int)c.size();
  vector<Constraint> all;
  for (int i = 0; i < n; ++i) all.push_back({A[i], b[i]});
  for (int j = 0; j < m; ++j) {
    vector<double> row(m);
    row[j] = 1;
    all.push_back({row, 0}); // x_j = 0 is a vertex boundary.
  }
  double best = -1e100;
  bool found = false;
  vector<double> zero(m);
  bool zero_ok = true;
  for (int i = 0; i < n; ++i)
    if (b[i] < -1e-9) zero_ok = false;
  if (zero_ok) found = true, best = 0;
  function<void(int, vector<Constraint> &)> go =
      [&](int at, vector<Constraint> &pick) {
    if ((int)pick.size() == m) {
      vector<double> x(m);
      if (!solve_square(pick, x)) return;
      for (double v : x) if (v < -1e-8) return;
      for (int i = 0; i < n; ++i) {
        double lhs = inner_product(A[i].begin(), A[i].end(),
                                   x.begin(), 0.0);
        if (lhs > b[i] + 1e-7) return;
      }
      double value = inner_product(c.begin(), c.end(), x.begin(), 0.0);
      best = max(best, value);
      found = true;
      return;
    }
    for (int i = at; i <= (int)all.size() - (m - (int)pick.size()); ++i) {
      pick.push_back(all[i]);
      go(i + 1, pick);
      pick.pop_back();
    }
  };
  vector<Constraint> pick;
  go(0, pick);
  return found ? optional<double>(best) : nullopt;
}

static void load(const vector<vector<double>> &A, const vector<double> &b,
                 const vector<double> &c) {
  for (int i = 0; i < (int)A.size(); ++i) {
    for (int j = 0; j < (int)c.size(); ++j) tested::a[i][j] = A[i][j];
    tested::b[i] = b[i];
  }
  copy(c.begin(), c.end(), tested::c);
}

static void check_bounded(const vector<vector<double>> &A,
                          const vector<double> &b,
                          const vector<double> &c, const string &where) {
  load(A, b, c);
  auto want = oracle(A, b, c);
  double got = tested::simplex(A.size(), c.size());
  if (!want || !isfinite(got) || abs(got - *want) > 1e-7) {
    cerr << "Simplex mismatch at " << where << " want="
         << want.value_or(numeric_limits<double>::quiet_NaN())
         << " got=" << got << '\n';
    exit(1);
  }
  for (int j = 0; j < int(c.size()); ++j)
    if (!isfinite(tested::x[j]) || tested::x[j] < -1e-7) abort();
  for (int i = 0; i < int(A.size()); ++i)
    if (inner_product(A[i].begin(), A[i].end(), tested::x, 0.0)
        > b[i] + 1e-7) abort();
  if (abs(inner_product(c.begin(), c.end(), tested::x, 0.0) - got)
      > 1e-7) abort();
}

int main() {
  check_bounded({}, {}, {}, "no variables or constraints");
  check_bounded({}, {}, {-1, 0, -2}, "unconstrained bounded objective");
  check_bounded({{}, {}}, {0, 2}, {}, "no variables");
  check_bounded({{1}, {-1}}, {1, -1}, {-1}, "valid optimum equals status");
  check_bounded({{1}, {1}, {-1}, {-1}, {0}}, {2, 2, -2, -2, 0},
                {3}, "duplicate equality and zero row");
  // Beale's cycling example: Bland's rule must terminate, optimum 1.
  check_bounded({{.5, -5.5, -2.5, 9}, {.5, -1.5, -.5, 1},
                 {1, 0, 0, 0}}, {0, 0, 1}, {10, -57, -9, -24},
                "cycling example");
  load({}, {}, {1});
  if (tested::simplex(0, 1) != -1) return 1;
  load({{}}, {-1}, {});
  if (tested::simplex(1, 0) != -1) return 1;
  // Both array limits and reuse after differently sized calls.
  vector<vector<double>> edge(tested::MAXN - 2, vector<double>(1, 1));
  load(edge, vector<double>(edge.size(), 1), {1});
  if (tested::simplex(edge.size(), 1) != 1 || tested::x[0] != 1)
    return 1;
  int width = tested::MAXM - 2;
  vector<double> objective(width, -1);
  load({}, {}, objective);
  if (tested::simplex(0, width) != 0) return 1;
  for (int j = 0; j < width; ++j)
    if (tested::x[j] != 0) return 1;
  check_bounded({{1, 1}, {1, 0}, {0, 1}}, {4, 2, 3}, {3, 2}, "triangle");
  check_bounded({{1}, {-1}}, {3, 0}, {2}, "one variable");
  check_bounded({{1}, {-1}}, {3, 0}, {-2}, "negative objective");
  check_bounded({{1}, {-1}}, {2, -2}, {1}, "equality boundary");
  check_bounded({{1, 0}, {0, 1}, {1, 1}, {1, 0}}, {4, 5, 6, 4},
                {0, 0}, "zero objective");

  load({{-1}}, {0}, {1});
  if (tested::simplex(1, 1) != -1) {
    cerr << "Simplex missed unbounded LP\n"; return 1;
  }
  load({{0}}, {-1}, {1});
  if (tested::simplex(1, 1) != -1) {
    cerr << "Simplex missed infeasible LP\n"; return 1;
  }

  mt19937 rng(0x51e3);
  for (int tc = 0; tc < 180; ++tc) {
    int m = 1 + rng() % 3, extra = rng() % 6;
    vector<vector<double>> A(m + extra, vector<double>(m));
    vector<double> b(m + extra), c(m);
    for (int j = 0; j < m; ++j) A[j][j] = 1, b[j] = 1 + rng() % 9;
    for (int i = m; i < m + extra; ++i) {
      for (int j = 0; j < m; ++j) A[i][j] = (int)(rng() % 7) - 2;
      b[i] = rng() % 12;
    }
    for (double &v : c) v = (int)(rng() % 13) - 6;
    check_bounded(A, b, c, "random bounded case");
  }
  // Negative RHS cases exercise phase I and check the primal witness.
  for (int tc = 0; tc < 400; ++tc) {
    int m = 1 + rng() % 3;
    vector<vector<double>> A;
    vector<double> b, c(m), witness(m);
    for (double &v : witness) v = rng() % 5;
    for (int j = 0; j < m; ++j) {
      vector<double> row(m); row[j] = 1;
      A.push_back(row); b.push_back(witness[j] + 1 + rng() % 5);
      row[j] = -1; A.push_back(row); b.push_back(-witness[j]);
      c[j] = int(rng() % 13) - 6;
    }
    for (int i = 0; i < 3; ++i) {
      vector<double> row(m);
      for (double &v : row) v = int(rng() % 7) - 3;
      double rhs = inner_product(row.begin(), row.end(),
                                 witness.begin(), 0.0) + rng() % 3;
      A.push_back(row); b.push_back(rhs);
    }
    check_bounded(A, b, c, "random phase-I case");
  }
  cout << "simplex_boundaries: PASS 580 brute-vertex LPs + "
          "zero dimensions/cycling/array limits\n";
}
