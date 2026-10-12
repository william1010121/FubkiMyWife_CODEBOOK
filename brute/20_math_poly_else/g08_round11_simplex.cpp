// Tests codebook/6_Math/Simplex_Algorithm.cpp via the vertex harness.
// Reuse the independent vertex oracle and primal checks.
#define main prior_main
#include "simplex_boundaries.cpp"
#undef main

int main() {
  // Exhaustive one-variable LPs, including empty feasible sets and rays.
  const int coefficients[] = {-2, -1, 0, 1, 2};
  const int bounds[] = {-3, -1, 0, 1, 3};
  int cases = 0;
  for (int code = 0; code < 15625; ++code) {
    vector<vector<double>> A(3, vector<double>(1));
    vector<double> b(3);
    int z = code;
    double lower = 0, upper = numeric_limits<double>::infinity();
    bool feasible = true;
    for (int i = 0; i < 3; ++i) {
      int a = coefficients[z % 5]; z /= 5;
      int rhs = bounds[z % 5]; z /= 5;
      A[i][0] = a; b[i] = rhs;
      if (a > 0) upper = min(upper, double(rhs) / a);
      if (a < 0) lower = max(lower, double(rhs) / a);
      if (!a && rhs < 0) feasible = false;
    }
    feasible &= lower <= upper;
    for (double c : {-2., 0., 2.}) {
      load(A, b, {c});
      double got = tested::simplex(3, 1);
      bool unbounded = c > 0 && !isfinite(upper);
      if (!feasible || unbounded) {
        if (got != -1) abort();
      } else {
        double want = c * (c > 0 ? upper : lower);
        if (!isfinite(got) || abs(got - want) > 1e-8 ||
            tested::x[0] < lower - 1e-8 ||
            tested::x[0] > upper + 1e-8 ||
            abs(c * tested::x[0] - got) > 1e-8) abort();
      }
      ++cases;
    }
  }
  // Beale's cycling LP translated away from the origin, with all row and
  // variable permutations. Tight lower bounds force negative-RHS phase I.
  const vector<vector<double>> base{{.5, -5.5, -2.5, 9},
      {.5, -1.5, -.5, 1}, {1, 0, 0, 0}};
  const vector<double> rhs{0, 0, 1}, objective{10, -57, -9, -24};
  const vector<double> offset{1, 2, 3, 4};
  vector<int> cols{0, 1, 2, 3};
  do {
    vector<int> rows{0, 1, 2};
    do {
      vector<vector<double>> A;
      vector<double> b, c(4);
      for (int i : rows) {
        vector<double> row(4);
        for (int j = 0; j < 4; ++j) row[j] = base[i][cols[j]];
        A.push_back(row);
        b.push_back(rhs[i] + inner_product(base[i].begin(),
                                          base[i].end(),
                                          offset.begin(), 0.0));
      }
      for (int j = 0; j < 4; ++j) {
        vector<double> row(4); row[j] = -1;
        A.push_back(row); b.push_back(-offset[cols[j]]);
        c[j] = objective[cols[j]];
      }
      check_bounded(A, b, c, "translated cycling LP permutation");
      double want = 1 + inner_product(objective.begin(), objective.end(),
                                      offset.begin(), 0.0);
      if (abs(tested::simplex(A.size(), 4) - want) > 1e-7) abort();
      ++cases;
    } while (next_permutation(rows.begin(), rows.end()));
  } while (next_permutation(cols.begin(), cols.end()));
  cout << "round11 simplex PASS: " << cases
       << " interval-oracle LPs and translated cycling permutations\n";
}
