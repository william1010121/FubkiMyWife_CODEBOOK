#include <bits/stdc++.h>
#include <unistd.h>

using i128 = __int128_t;
using u128 = __uint128_t;

#include "../../codebook/1_Basic/bigIntIO.cpp"

namespace {

[[noreturn]] void fail(const std::string &message) {
  std::cerr << "FAIL bigint_io: " << message << '\n';
  std::exit(1);
}

void check(bool condition, const std::string &message) {
  if (!condition) fail(message);
}

std::string decimal(i128 value) {
  if (value == 0) return "0";
  bool negative = value < 0;
  u128 magnitude = negative ? static_cast<u128>(-(value + 1)) + 1
                            : static_cast<u128>(value);
  std::string result;
  while (magnitude) {
    result.push_back(static_cast<char>('0' + magnitude % 10));
    magnitude /= 10;
  }
  if (negative) result.push_back('-');
  std::reverse(result.begin(), result.end());
  return result;
}

struct InputCase {
  std::string token;
  i128 value;
};

}  // namespace

int main() {
  const i128 max_value = static_cast<i128>((static_cast<u128>(1) << 127) - 1);
  const i128 min_value = -max_value - 1;

  std::vector<InputCase> cases = {
      {"0", 0},
      {"-0", 0},
      {"+42", 42},
      {"0000000000000000000000000000000000000000007", 7},
      {decimal(max_value), max_value},
      {decimal(min_value), min_value},
      {"-9223372036854775808", static_cast<i128>(-9223372036854775807LL) - 1},
      {"18446744073709551616", static_cast<i128>(1) << 64},
  };

  std::mt19937_64 rng(0x51a7e5eedULL);
  for (int tc = 0; tc < 240; ++tc) {
    u128 raw = (static_cast<u128>(rng()) << 64) | rng();
    raw &= (static_cast<u128>(1) << 126) - 1;
    i128 value = static_cast<i128>(raw);
    if (rng() & 1) value = -value;
    cases.push_back({decimal(value), value});
  }

  std::string input = "  leading-text\n\t";
  for (size_t i = 0; i < cases.size(); ++i) {
    input += cases[i].token;
    if (i + 1 != cases.size()) input += (i & 1) ? "\n" : " \t";
  }

  FILE *input_file = tmpfile();
  FILE *output_file = tmpfile();
  check(input_file && output_file, "tmpfile");
  check(fwrite(input.data(), 1, input.size(), input_file) == input.size(),
        "write input");
  fflush(input_file);
  rewind(input_file);
  fflush(stdout);
  check(dup2(fileno(input_file), STDIN_FILENO) >= 0, "redirect stdin");
  clearerr(stdin);

  std::vector<i128> parsed;
  parsed.reserve(cases.size());
  for (size_t i = 0; i < cases.size(); ++i) parsed.push_back(read());
  for (size_t i = 0; i < cases.size(); ++i)
    check(parsed[i] == cases[i].value, "read case " + std::to_string(i));
  i128 eof_value = 123;
  bool eof_result = read(eof_value);
  check(!eof_result, "EOF must terminate without hanging");
  check(eof_value == 123, "EOF must not overwrite the destination");

  check(dup2(fileno(output_file), STDOUT_FILENO) >= 0, "redirect stdout");
  clearerr(stdout);
  for (i128 value : parsed) {
    print(value);
    putchar('|');
  }
  fflush(stdout);
  rewind(output_file);

  std::string expected;
  for (i128 value : parsed) expected += decimal(value) + '|';
  std::string printed;
  char buffer[256];
  size_t got;
  while ((got = fread(buffer, 1, sizeof buffer, output_file)) != 0)
    printed.append(buffer, got);
  check(printed == expected, "print round-trip");

  for (i128 x : parsed)
    for (i128 y : parsed)
      check(cmp(x, y) == (x > y), "descending comparator");

  std::cerr << "PASS bigint_io: " << cases.size()
            << " signed/zero/large tokens, EOF token, print, cmp\n";
}
