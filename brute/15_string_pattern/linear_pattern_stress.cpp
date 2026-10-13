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

  // Long byte-periodic strings exercise reused boxes and both parities.
  for (int i = 0; i < n; ++i) s[i] = i % 2 ? char(255) : char(0);
  t = s.substr(0, m);
  istringstream periodic(matches(s, t));
  for (int pos = 1, got; pos <= n - m + 1; pos += 2)
    require(bool(periodic >> got) && got == pos, "KMP byte-periodic");
  require(!(periodic >> extra), "KMP periodic extra output");
  z_test::make_z(s);
  for (int i = 1; i < n; ++i)
    require(z_test::z[i] == (i % 2 ? 0 : n - i), "Z-value periodic");
  manacher_test::Manacher(s);
  for (int i = 0; i <= 2 * n; ++i)
    require(manacher_test::z[i] == (i % 2 ?
            2 * min(i / 2, n - 1 - i / 2) + 2 : 1),
            "Manacher periodic odd/even radii");

  // Shrinking inputs must not read stale output or overwrite its suffix.
  vector<int> old_z(z_test::z, z_test::z + z_test::MAXn);
  z_test::make_z(""); z_test::make_z("%");
  require(equal(ALL(old_z), z_test::z), "Z empty/singleton reuse");
  z_test::make_z("aaa");
  require(z_test::z[1] == 2 && z_test::z[2] == 1 &&
          equal(old_z.begin() + 3, old_z.end(), z_test::z + 3),
          "Z short reuse and untouched suffix");
  vector<int> old_r(manacher_test::z,
                    manacher_test::z + manacher_test::MAXN);
  manacher_test::Manacher("");
  require(manacher_test::z[0] == 1 &&
          equal(old_r.begin() + 1, old_r.end(), manacher_test::z + 1),
          "Manacher empty reuse and untouched suffix");
  manacher_test::Manacher("%");
  require(manacher_test::z[0] == 1 && manacher_test::z[1] == 2 &&
          manacher_test::z[2] == 1 &&
          equal(old_r.begin() + 3, old_r.end(), manacher_test::z + 3),
          "Manacher singleton reuse and untouched suffix");
  istringstream boundaries(matches(s, ""));
  for (int pos = 1, got; pos <= n + 1; ++pos)
    require(bool(boundaries >> got) && got == pos, "KMP empty boundaries");
  require(!(boundaries >> extra) && matches("", "") == "1\n",
          "KMP empty output count");
  double seconds = chrono::duration<double>(
      chrono::steady_clock::now() - start).count();
  cout << "PASS linear string stress n=" << n
       << " seconds=" << seconds << '\n';
}
