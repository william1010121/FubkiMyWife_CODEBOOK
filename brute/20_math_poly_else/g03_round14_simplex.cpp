// Tests codebook/6_Math/Simplex_Algorithm.cpp, including unbounded LPs.
#include "../../codebook/1_Basic/Default_code.cpp"
#define main prior_main
#include "simplex_boundaries.cpp"
#undef main
int main() {
  mt19937 rng(140314);
  for (int tc = 0; tc < 6000; ++tc) {
    int m = tc % 4, n = rng() % 9;
    vector<vector<double>> A(n, vector<double>(m));
    vector<double> rhs(n), c(m);
    for (int i = 0; i < n; ++i) {
      for (auto &v : A[i]) v = int(rng() % 7) - 3;
      rhs[i] = int(rng() % 9) - 4;
      if (i && tc % 4 == 0) A[i] = A[0], rhs[i] = rhs[0];
    }
    for (auto &v : c) v = int(rng() % 7) - 3;
    auto best_vertex = oracle(A, rhs, c);
    // A nonnegative polyhedron contains no lines, hence a feasible one
    // has a vertex. Positive objective on a bounded slice of its recession
    // cone independently certifies unboundedness.
    auto directions = A;
    directions.push_back(vector<double>(m, 1));
    vector<double> limits(n + 1); limits.back() = 1;
    auto ray_value = oracle(directions, limits, c);
    assert(ray_value);
    load(A, rhs, c); fill_n(tested::x, tested::MAXM, -123);
    double got = tested::simplex(n, m);
    if (!best_vertex || *ray_value > 1e-8) {
      assert(got == -1); continue;
    }
    assert(isfinite(got) && abs(got - *best_vertex) < 1e-7);
    for (int j = 0; j < m; ++j)
      assert(isfinite(tested::x[j]) && tested::x[j] >= -1e-7);
    for (int i = 0; i < n; ++i)
      assert(inner_product(ALL(A[i]), tested::x, 0.) <= rhs[i] + 1e-7);
    assert(abs(inner_product(ALL(c), tested::x, 0.) - got) < 1e-7);
    for (int j = m + 1; j < tested::MAXM; ++j)
      assert(tested::x[j] == -123);
  }
  cout << "PASS simplex: 6000 vertex/recession-oracle LPs, reuse\n";
}
