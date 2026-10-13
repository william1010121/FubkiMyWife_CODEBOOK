#include <bits/stdc++.h>
using namespace std;
#include "../../codebook/9_Else/AdaptiveSimpson.cpp"

int main() {
  // Cubics need no refinement: four new samples per split plus one
  // first endpoint. Large k detects redundant endpoint evaluations.
  for (int k : {1, 997, 200000})
    for (auto [l, r] : {pair{0., 1.}, pair{1., 0.}, pair{1., 1.}}) {
      long long calls = 0;
      auto s = make_simpson([&](double x) { ++calls; return x*x*x; });
      double got = s.eval2(l, r, 1e-8, k);
      double want = (pow(r, 4) - pow(l, 4)) / 4;
      assert(abs(got - want) < 1e-11);
      assert(calls == 4LL*k + 1);
    }
  cout << "PASS Simpson shared endpoints, k=1/997/200000\n";
}
