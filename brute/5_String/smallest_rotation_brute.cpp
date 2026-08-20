#include "string_brute_common.hpp"

#define SZ(x) ((int)(x).size())

#include "../../codebook/5_String/Smallest_Rotation.cpp"

string template_rotation(const string &s) {
  return mcp(s);
}

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    string s;
    if (!(cin >> s)) return 2;
    cout << brute_min_rotation(s) << '\n';
    return 0;
  }
  for (int tc = 0; tc < 500; ++tc) {
    string s = random_string(1 + rng() % 80, 1 + rng() % 4);
    expect(template_rotation(s) == brute_min_rotation(s), "Smallest Rotation");
  }
  expect(template_rotation("bbaabbabaa") == brute_min_rotation("bbaabbabaa"),
         "Smallest Rotation repeated candidates");
  cout << "PASS Smallest Rotation 501 cases\n";
}
