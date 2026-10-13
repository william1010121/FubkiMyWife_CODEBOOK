#include <bits/stdc++.h>
using namespace std;
#include "../../codebook/6_Math/Simplex_Algorithm.cpp"

// Near-limit dense tableau with a known, independently checked optimum.
// Each dense row is redundant; the coordinate bounds fix the optimum.
void stress(int n, int m, bool phase_one) {
  for (int i = 0; i < n; ++i) {
    double sum = 0;
    for (int j = 0; j < m; ++j) {
      double v = i < m ? double(i == j) :
        (phase_one && i < 2 * m ? -double(i - m == j) :
         .5 + ((i * 31 + j * 17) % 100) / 200.);
      a[i][j] = v; sum += v;
    }
    b[i] = i < m ? (phase_one ? 2 : 1) :
      (phase_one && i < 2 * m ? -1 :
       sum * (phase_one ? 2 : 1) + 1);
  }
  fill_n(c, m, 1.);
  auto t = chrono::steady_clock::now();
  double ans = simplex(n, m), expected = m * (phase_one ? 2. : 1.);
  assert(abs(ans - expected) < 1e-6);
  for (int j = 0; j < m; ++j)
    assert(abs(x[j] - (phase_one ? 2 : 1)) < 1e-8);
  for (int i = 0; i < n; ++i) {
    double lhs = inner_product(a[i], a[i] + m, x, 0.);
    assert(lhs <= b[i] + 1e-7);
  }
  cout << n << 'x' << m << " phase_I=" << phase_one << ": PASS "
       << chrono::duration<double, milli>(
            chrono::steady_clock::now() - t).count() << " ms\n";
}
int main() {
  stress(MAXN - 2, MAXM - 2, false);
  stress(10000, 80, true);
}
