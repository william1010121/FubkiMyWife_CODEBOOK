#include <bits/stdc++.h>
using namespace std;
#define SZ(a) ((int)(a).size())
#define ALL(v) (v).begin(), (v).end()
#define pb push_back

namespace sa_example {
#include "../../codebook/5_String/SAIS-C++20.cpp"
void run() {
  string s({'A', char(255), 'A'}); int i = 1;
#include "../../codebook/examples/SAIS-C++20.cpp"
  if (sf.sa != vector<int>({2, 0, 1}) || sf.hi != vector<int>({0, 1, 0}))
    throw runtime_error("SAIS example unsigned bytes");
}
}
namespace ac_example {
const int len = 64, sigma = 3;
#include "../../codebook/5_String/Aho-Corasick_Automatan.cpp"
void run() {
  vector<string> pats = {"A", "BA", "A"}; string text = "BABA";
  int i = 0;
#include "../../codebook/examples/Aho-Corasick_Automatan.cpp"
  for (int v : ends)
    if (ac.cnt[v] != 2) throw runtime_error("AC example scan counts");
}
}
namespace db_example {
#include "../../codebook/5_String/De_Bruijn_sequence.cpp"
void run() {
  int C = 3, N = 3, K = 27;
  vector<int> storage(N + K - 1); int *out = storage.data();
#include "../../codebook/examples/De_Bruijn_sequence.cpp"
  set<vector<int>> windows;
  for (int i = 0; i < K; ++i)
    windows.emplace(out + i, out + i + N);
  if ((int)windows.size() != K) throw runtime_error("DB example windows");
}
}
int main() {
  try {
    sa_example::run(); ac_example::run(); db_example::run();
    cout << "PASS g06 examples SAIS/AC/DeBruijn\n";
  } catch (const exception &e) {
    cerr << "FAIL g06 examples: " << e.what() << '\n'; return 1;
  }
}
