#include "common.hpp"
namespace tested {
#include "../../codebook/6_Math/Simplex_Algorithm.cpp"
}
static void load(const vector<vector<double>> &A, const vector<double> &B, const vector<double> &C) {
  for (int i = 0; i < (int)A.size(); ++i) for (int j = 0; j < (int)C.size(); ++j) tested::a[i][j] = A[i][j];
  copy(B.begin(), B.end(), tested::b); copy(C.begin(), C.end(), tested::c);
}
int main() {
  load({{1,1},{1,0},{0,1}}, {4,2,3}, {3,2});
  require_ok(abs(tested::simplex(3,2) - 10.0) < 1e-8, "bounded LP");
  load({{-1}}, {0}, {1});
  require_ok(tested::simplex(1,1) == -1, "unbounded LP");
  load({{1}}, {-1}, {1});
  require_ok(tested::simplex(1,1) == -1, "infeasible LP");
  load({{1,1},{1,0},{0,1}}, {1,1,1}, {1,1});
  require_ok(abs(tested::simplex(3,2) - 1.0) < 1e-8, "degenerate LP");
  cout << "simplex: OK\n";
}
