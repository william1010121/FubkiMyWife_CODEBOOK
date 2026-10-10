#include <bits/stdc++.h>
#include <unistd.h>

#include "../../codebook/1_Basic/bigIntIO.cpp"

int main() {
  using i128 = __int128_t;
  const i128 hi = (i128)((__uint128_t(1) << 127) - 1);
  const i128 lo = -hi - 1;
  const std::string digits[] = {
      "0", "7", "170141183460469231731687303715884105727",
      "170141183460469231731687303715884105728"};
  struct Case { bool ok; i128 value; };
  std::vector<Case> cases;
  std::string input;
  auto append = [&](const std::string &prefix) {
    bool negative = !prefix.empty() && prefix.back() == '-';
    for (int i = 0; i < 4; ++i) {
      input += prefix + digits[i] + '\n';
      if (i == 3) cases.push_back({negative, lo});
      else {
        i128 magnitude = i == 0 ? 0 : i == 1 ? 7 : hi;
        cases.push_back({true, negative ? -magnitude : magnitude});
      }
    }
  };

  // Every prefix through length five over signs, junk, whitespace,
  // NUL and high bytes; a sign binds only to an immediately following digit.
  const std::string alphabet = std::string("-+x ") + '\0' + char(0xff);
  std::function<void(std::string, int)> enumerate =
      [&](std::string prefix, int left) {
        append(prefix);
        if (left) for (char c : alphabet) enumerate(prefix + c, left - 1);
      };
  enumerate("", 5);
  // Every nondigit byte immediately before a number, including signed char.
  for (int c = 0; c < 256; ++c)
    if (c < '0' || c > '9') append(std::string(1, char(c)));
  input += "-+x --\xff"; // junk and signs ending at EOF

  FILE *file = tmpfile();
  if (!file || fwrite(input.data(), 1, input.size(), file) != input.size())
    return 1;
  fflush(file), rewind(file);
  if (dup2(fileno(file), STDIN_FILENO) < 0) return 1;
  clearerr(stdin);
  for (size_t i = 0; i < cases.size(); ++i) {
    i128 value = 123;
    bool ok = read(value);
    if (ok != cases[i].ok || value != (ok ? cases[i].value : 123)) {
      std::cerr << "FAIL bigint scanner case " << i << '\n';
      return 1;
    }
  }
  for (int i = 0; i < 5; ++i) {
    i128 value = 123;
    if (read(value) || value != 123 || read() != 0) return 1;
  }
  std::cerr << "PASS bigint scanner: " << cases.size()
            << " prefixes/limits and stable EOF\n";
}
