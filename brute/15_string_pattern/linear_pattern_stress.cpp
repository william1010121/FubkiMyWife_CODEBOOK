#include "../../codebook/1_Basic/Default_code.cpp"

namespace z_test {
constexpr int MAXn = 500001;
#include "../../codebook/5_String/Z-value.cpp"
}
namespace manacher_test {
constexpr int MAXN = 1000001;
#include "../../codebook/5_String/Manacher.cpp"
}

static string matches(string s, string t) {
  ostringstream output;
#define cout output
#include "../../codebook/5_String/KMP.cpp"
#undef cout
  return output.str();
}

static void require(bool ok, const char *what) {
  if (!ok) { cerr << "FAIL " << what << '\n'; exit(1); }
}

int main() {
  const int n = 500000, m = 250000;
  auto start = chrono::steady_clock::now();
  string s(n, 'a'), t(m, 'a');
  istringstream output(matches(s, t));
  for (int pos = 1, got; pos <= n - m + 1; ++pos)
    require(bool(output >> got) && got == pos, "KMP overlaps");
  int extra;
  require(!(output >> extra), "KMP extra output");
  t.back() = 'b';
  require(matches(s, t).empty(), "KMP long fallback");

  z_test::z[0] = -12345; // Neither read nor overwritten by make_z.
  z_test::make_z(s);
  require(z_test::z[0] == -12345, "Z-value z[0] contract");
  for (int i = 1; i < n; ++i)
    require(z_test::z[i] == n - i, "Z-value equal characters");
  s.back() = 'b';
  z_test::make_z(s);
  for (int i = 1; i < n; ++i)
    require(z_test::z[i] == n - i - 1, "Z-value reuse/fallback");

  for (char c : {'a', '%'}) {
    s.assign(n, c);
    manacher_test::Manacher(s);
    for (int i = 0; i <= 2 * n; ++i)
      require(manacher_test::z[i] == 1 + min(i, 2 * n - i),
              "Manacher equal characters/separators");
  }
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "PASS linear string stress n=" << n
       << " seconds=" << seconds << '\n';
}
