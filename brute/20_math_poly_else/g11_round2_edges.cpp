#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define ALL(v) v.begin(), v.end()
#include "../../codebook/9_Else/cyc_tsearch.cpp"
#include "../../codebook/9_Else/DynamicConvexTrick.cpp"
#include "../../codebook/9_Else/BinarySearchOnFraction.cpp"
#include "../../codebook/9_Else/min_plus_convolution.cpp"
#include "../../codebook/9_Else/BitsetLCS.cpp"

void require(bool ok, const char *msg) {
  if (!ok) { cerr << msg << '\n'; exit(1); }
}
void cyclic() {
  // All strict cyclic U-shapes: exactly two changes of edge direction.
  for (int n = 2; n <= 8; ++n) {
    vector<int> a(n);
    iota(ALL(a), 0);
    do {
      int changes = 0;
      for (int i = 0; i < n; ++i)
        changes += (a[i] < a[(i+1)%n]) !=
                   (a[(i+1)%n] < a[(i+2)%n]);
      if (changes != 2) continue;
      auto pred = [&](int i, int j) { return a[i] < a[j]; };
      require(a[cyc_tsearch(n, pred)] == 0, "cyclic U-shape");
    } while (next_permutation(ALL(a)));
  }
  int n = INT_MAX;
  for (int shift : {0, 1, n/2, n-2, n-1}) {
    auto pred = [&](int i, int j) {
      require(0 <= i && i < n && 0 <= j && j < n,
              "cyclic search index overflow");
      return (ll(i)-shift+n)%n < (ll(j)-shift+n)%n;
    };
    require(cyc_tsearch(n, pred) == shift, "cyclic INT_MAX domain");
  }
}
void hull() {
  DynamicHull h;
  // Equal slopes retain the inferior line with the -kInf sentinel.
  h.addline(0, 0), h.addline(0, 1);
  for (ll x : {-DynamicHull::kInf+1, 0LL, DynamicHull::kInf})
    require(h.query(x) == 1, "hull valid sentinel endpoints");
  h.addline(1, 0), h.addline(-1, 0);
  for (ll x : {-DynamicHull::kInf+1, -1LL, 0LL, 1LL,
               DynamicHull::kInf})
    require(h.query(x) == max(1LL, abs(x)), "hull extreme x");
}
void fractions() {
  for (ll N = 1; N <= 25; ++N)
    for (ll A = 0; A <= N+1; ++A)
      for (ll B = 1; B <= N; ++B) for (bool strict : {false, true}) {
        auto pred = [=](Q x) {
          return !x.q || (strict ? x.p*B > A*x.q : x.p*B >= A*x.q);
        };
        Q want{1, 0}, got = frac_bs(N, pred);
        for (ll q = 1; q <= N; ++q) for (ll p = 0; p <= N; ++p)
          if (pred({p, q}) && (!want.q || p*want.q < want.p*q))
            want = {p, q};
        require(got.p*want.q == want.p*got.q,
                "fraction strict/closed/zero/infinity");
        require(got.p <= N && got.q <= N && pred(got),
                "fraction feasible bounds");
      }
}
struct Counted {
  ll value = 0;
  static inline int copies = 0;
  Counted() = default;
  Counted(ll x) : value(x) {}
  Counted(const Counted &x) : value(x.value) { ++copies; }
  Counted &operator=(const Counted &) = default;
  friend Counted operator+(const Counted &a, const Counted &b) {
    return Counted(a.value + b.value);
  }
  bool operator<(const Counted &b) const { return value < b.value; }
};
void convolution() {
  vector<Counted> a(1), b(1000);
  for (int i = 0; i < 1000; ++i) b[i].value = i;
  Counted::copies = 0;
  auto c = min_plus_convolution(a, b);
  require(c.size() == b.size(), "counted min-plus size");
  for (int i = 0; i < 1000; ++i)
    require(c[i].value == i, "counted min-plus value");
  require(Counted::copies == 0, "min-plus return copies output");
}
void empty_bitset() {
  vector<int> empty, distinct(20000);
  iota(ALL(distinct), 0);
  require(bitset_lcs(empty, distinct) == 0, "empty first LCS input");
  require(bitset_lcs(distinct, empty) == 0, "empty second LCS input");
}
int main(int argc, char **argv) {
  string test = argc > 1 ? argv[1] : "all";
  if (test == "all" || test == "cyclic") cyclic();
  if (test == "all" || test == "hull") hull();
  if (test == "all" || test == "fractions") fractions();
  if (test == "all" || test == "convolution") convolution();
  if (test == "all" || test == "bitset") empty_bitset();
  cout << "g11 round2 edges PASS: " << test << '\n';
}
