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

static double oracle(const vector<vector<double>> &A, const vector<double> &b,
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
  function<void(int, vector<Constraint> &)> go = [&](int at, vector<Constraint> &pick) {
    if ((int)pick.size() == m) {
      vector<double> x(m);
      if (!solve_square(pick, x)) return;
      for (double v : x) if (v < -1e-8) return;
      for (int i = 0; i < n; ++i) {
        double lhs = inner_product(A[i].begin(), A[i].end(), x.begin(), 0.0);
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
  return found ? best : -1;
}

static void load(const vector<vector<double>> &A, const vector<double> &b,
                 const vector<double> &c) {
  for (int i = 0; i < (int)A.size(); ++i) {
    for (int j = 0; j < (int)c.size(); ++j) tested::a[i][j] = A[i][j];
    tested::b[i] = b[i];
  }
  copy(c.begin(), c.end(), tested::c);
}

static void check_bounded(const vector<vector<double>> &A, const vector<double> &b,
                          const vector<double> &c, const string &where) {
  load(A, b, c);
  double want = oracle(A, b, c), got = tested::simplex((int)A.size(), (int)c.size());
  if (want == -1 || abs(got - want) > 1e-7) {
    cerr << "Simplex mismatch at " << where << " want=" << want << " got=" << got << '\n';
    exit(1);
  }
}

int main() {
  check_bounded({{1, 1}, {1, 0}, {0, 1}}, {4, 2, 3}, {3, 2}, "triangle");
  check_bounded({{1}, {-1}}, {3, 0}, {2}, "one variable");
  check_bounded({{1}, {-1}}, {3, 0}, {-2}, "negative objective");
  check_bounded({{1}, {-1}}, {2, -2}, {1}, "equality boundary");
  check_bounded({{1, 0}, {0, 1}, {1, 1}, {1, 0}}, {4, 5, 6, 4}, {0, 0}, "zero objective");

  load({{-1}}, {0}, {1});
  if (tested::simplex(1, 1) != -1) { cerr << "Simplex missed unbounded LP\n"; return 1; }
  load({{0}}, {-1}, {1});
  if (tested::simplex(1, 1) != -1) { cerr << "Simplex missed infeasible LP\n"; return 1; }

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
  cout << "simplex_boundaries: PASS 180 brute-vertex LPs + edge cases\n";
}
