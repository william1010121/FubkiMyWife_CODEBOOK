#include "string_brute_common.hpp"

constexpr int MAXn = 4096;
#define SZ(x) ((int)(x).size())
#include "../../codebook/5_String/Z-value.cpp"
#undef SZ

vector<int> template_z(const string &s) {
  fill(begin(z), begin(z) + s.size(), 0);
  make_z(s);
  return vector<int>(z, z + s.size());
}

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    string s;
    if (!(cin >> s)) return 2;
    for (int x : brute_z(s)) cout << x << ' ';
    cout << '\n';
    return 0;
  }
  for (int tc = 0; tc < 500; ++tc) {
    string s = random_string(1 + rng() % 120, 1 + rng() % 4);
    expect(template_z(s) == brute_z(s), "Z-value");
  }
  expect(template_z("aaaaa") == vector<int>({0, 4, 3, 2, 1}), "Z-value equal chars");
  cout << "PASS Z-value 501 cases\n";
}
