#include "string_brute_common.hpp"

#define SZ(x) ((int)(x).size())
#define ALL(x) begin(x), end(x)
#define pb push_back
#include "../../codebook/5_String/SAIS-C++20.cpp"
#undef pb
#undef ALL
#undef SZ

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    string s;
    if (!(cin >> s)) return 2;
    auto sa = brute_sa(s), h = brute_lcp(s, sa);
    for (int x : sa) cout << x << ' ';
    cout << "\n";
    for (int x : h) cout << x << ' ';
    cout << '\n';
    return 0;
  }
  for (int tc = 0; tc < 500; ++tc) {
    string s = random_string(1 + rng() % 100, 1 + rng() % 5);
    vector<int> a(s.size());
    for (int i = 0; i < (int)s.size(); ++i) a[i] = s[i] - 'a' + 1;
    Suffix got(a.data(), a.size());
    auto sa = brute_sa(s), h = brute_lcp(s, sa);
    expect(got.sa == sa, "SAIS-C++20 SA");
    expect(got.hi == h, "SAIS-C++20 LCP");
  }
  cout << "PASS SAIS-C++20 500 cases\n";
}
