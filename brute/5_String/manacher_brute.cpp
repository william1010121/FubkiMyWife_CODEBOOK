#include "string_brute_common.hpp"

constexpr int MAXN = 4096;
#define SZ(x) ((int)(x).size())
#define pb push_back
#include "../../codebook/5_String/Manacher.cpp"
#undef pb
#undef SZ

vector<int> template_radii(const string &s) {
  fill(begin(z), begin(z) + 2 * s.size() + 1, 0);
  Manacher(s);
  return vector<int>(z, z + 2 * s.size() + 1);
}

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    string s;
    if (!(cin >> s)) return 2;
    for (int x : brute_manacher_radii(s)) cout << x << ' ';
    cout << '\n';
    return 0;
  }
  for (int tc = 0; tc < 500; ++tc) {
    string s = random_string(1 + rng() % 80, 1 + rng() % 4);
    expect(template_radii(s) == brute_manacher_radii(s), "Manacher");
  }
  expect(template_radii("abba") == brute_manacher_radii("abba"), "Manacher even");
  cout << "PASS Manacher 501 cases\n";
}
