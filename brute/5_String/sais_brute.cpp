#include "string_brute_common.hpp"

#define N 512
#include "../../codebook/5_String/SAIS.cpp"
#undef N

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
    sfx::build(a.data(), a.size());
    auto sa = brute_sa(s), h = brute_lcp(s, sa);
    expect(vector<int>(sfx::SA, sfx::SA + s.size()) == sa, "SAIS SA");
    expect(vector<int>(sfx::H, sfx::H + s.size()) == h, "SAIS LCP");
  }
  cout << "PASS SAIS 500 cases\n";
}
