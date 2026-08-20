#include "string_brute_common.hpp"

#include "../../codebook/5_String/De_Bruijn_sequence.cpp"

vector<int> template_sequence(int c, int n) {
  long long k = 1;
  for (int i = 0; i < n; ++i) k *= c;
  vector<int> out(n + k - 1);
  dbs.solve(c, n, (int)k, out.data());
  return out;
}

int main(int argc, char **argv) {
  using namespace string_brute;
  if (argc > 1 && string(argv[1]) == "--oracle") {
    int c, n;
    if (!(cin >> c >> n)) return 2;
    vector<int> out;
    int x;
    while (cin >> x) out.push_back(x);
    cout << (valid_de_bruijn(out, c, n) ? "VALID" : "INVALID") << '\n';
    return 0;
  }
  int cases = 0;
  for (int c = 2; c <= 3; ++c)
    for (int n = 1; n <= 4; ++n) {
      if (c == 3 && n == 4) break;
      expect(valid_de_bruijn(template_sequence(c, n), c, n), "De Bruijn sequence");
      ++cases;
    }
  cout << "PASS De Bruijn sequence " << cases << " cases\n";
}
