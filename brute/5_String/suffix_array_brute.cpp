#include "string_brute_common.hpp"

constexpr int MAXN = 512;
#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Suffix_Array.cpp"

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
    suffix_array got;
    got.build(s);
    auto sa = brute_sa(s), h = brute_lcp(s, sa);
    expect(vector<int>(got.sa, got.sa + s.size()) == sa, "Suffix Array SA");
    expect(vector<int>(got.he, got.he + s.size()) == h, "Suffix Array LCP");
  }
  cout << "PASS Suffix Array 500 cases\n";
}
