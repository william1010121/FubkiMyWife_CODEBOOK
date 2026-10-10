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
      {decimal(max_value - 1), max_value - 1},
      {decimal(min_value + 1), min_value + 1},
      {"+" + std::string(4096, '0') + decimal(max_value), max_value},
      {"-" + std::string(4096, '0') + decimal(max_value), -max_value},
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

  // Decimal recursion crossing the 64-bit boundary, on either side.
  for (i128 base = i128(1) << 64;; base *= 10) {
    for (int delta = -10; delta <= 10; ++delta)
      for (int sign : {-1, 1}) {
        i128 value = sign * (base + delta);
        cases.push_back({decimal(value), value});
      }
    if (base > (max_value - 10) / 10) break;
  }
  for (i128 base = 1;; base *= 10) {
    for (int delta : {-1, 0, 1})
      for (int sign : {-1, 1}) {
        i128 value = sign * (base + delta);
        cases.push_back({decimal(value), value});
      }
    if (base > (max_value - 1) / 10) break;
  }
  std::mt19937_64 low_rng(0x64dec1a1);
  for (int tc = 0; tc < 256; ++tc) {
    i128 value = low_rng();
    if (tc & 1) value = -value;
    cases.push_back({decimal(value), value});
  }
  // Every binary magnitude boundary exercises both printer division paths.
  for (int bit = 0; bit < 127; ++bit)
    for (int delta : {-1, 0, 1}) for (int sign : {-1, 1}) {
      i128 value = sign * ((i128(1) << bit) + delta);
      cases.push_back({decimal(value), value});
    }
  // getchar returns unsigned bytes: all nondigit bytes are valid junk,
  // including NUL and 0xff, and must not hide the adjacent minus sign.
  for (int byte = 0; byte < 256; ++byte)
    if (byte < '0' || byte > '9')
      cases.push_back({std::string(1, char(byte)) + "-42", -42});
  std::vector<u128> unsigned_values{0, ~u128(0)};
  for (int bit = 0; bit < 128; ++bit)
    for (int delta : {-1, 0, 1})
      unsigned_values.push_back((u128(1) << bit) + delta);
  for (int i = 0; i < 256; ++i)
    unsigned_values.push_back((u128(low_rng()) << 64) | low_rng());

  std::string input = "  leading-text\n\t";
  for (size_t i = 0; i < cases.size(); ++i) {
    input += cases[i].token;
    if (i + 1 != cases.size()) input += (i & 1) ? "\n" : " \t";
  }

  FILE *input_file = tmpfile();
  FILE *output_file = tmpfile();
  check(input_file && output_file, "tmpfile");
  check(dup2(fileno(input_file), STDIN_FILENO) >= 0, "redirect empty stdin");
  clearerr(stdin);
  i128 empty_value = 123;
  for (int i = 0; i < 3; ++i)
    check(!read(empty_value) && empty_value == 123, "empty input");
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

  // Failures must consume the entire token and preserve the destination.
  FILE *edge_input = tmpfile();
  check(edge_input != nullptr, "edge tmpfile");
  std::string edge_text =
      "170141183460469231731687303715884105728 "
      "-170141183460469231731687303715884105729 " +
      std::string(4096, '9') + " --12 ++34 +-56 -+78 9 +";
  check(fwrite(edge_text.data(), 1, edge_text.size(), edge_input) ==
            edge_text.size(), "write edge input");
  fflush(edge_input), rewind(edge_input);
  check(dup2(fileno(edge_input), STDIN_FILENO) >= 0,
        "redirect edge stdin");
  clearerr(stdin);
  for (int i = 0; i < 3; ++i) {
    i128 value = 123;
    check(!read(value) && value == 123, "overflow preserves value");
  }
  for (int expected : {-12, 34, -56, 78, 9}) {
    i128 value = 0;
    check(read(value) && value == expected, "sign recovery after overflow");
  }
  eof_value = 123;
  check(!read(eof_value) && eof_value == 123, "trailing sign EOF");
  check(read() == 0, "value overload returns zero at EOF");

  // Sticky overflow, high-byte delimiters, sign resynchronization and the
  // documented consumption of the first nondigit after a numeric sequence.
  FILE *more_input = tmpfile();
  check(more_input != nullptr, "more tmpfile");
  std::string more_text = std::string(1, char(0xff)) +
      "170141183460469231731687303715884105728000000000000 "
      "-170141183460469231731687303715884105729000000000000 "
      "12-34 56+78 +- -+ -- ++ 9";
  check(fwrite(more_text.data(), 1, more_text.size(), more_input) ==
            more_text.size(), "write more input");
  fflush(more_input), rewind(more_input);
  check(dup2(fileno(more_input), STDIN_FILENO) >= 0, "redirect more stdin");
  clearerr(stdin);
  for (int i = 0; i < 2; ++i) {
    i128 value = 123;
    check(!read(value) && value == 123, "sticky overflow with trailing zeros");
  }
  for (int value : {12, 34, 56, 78, 9}) {
    i128 got = 0;
    check(read(got) && got == value, "delimiter consumption/sign recovery");
  }
  check(read() == 0, "more EOF");

  check(dup2(fileno(output_file), STDOUT_FILENO) >= 0, "redirect stdout");
  clearerr(stdout);
  for (i128 value : parsed) {
    print(value);
    putchar('|');
  }
  print_unsigned(~u128(0));
  putchar('|');
  for (u128 value : unsigned_values) {
    print_unsigned(value); putchar('|');
  }
  fflush(stdout);
  rewind(output_file);

  std::string expected;
  for (i128 value : parsed) expected += decimal(value) + '|';
  expected += "340282366920938463463374607431768211455|";
  for (u128 value : unsigned_values) {
    std::string digits;
    do {
      digits.push_back(char('0' + value % 10)); value /= 10;
    } while (value);
    std::reverse(digits.begin(), digits.end());
    expected += digits + '|';
  }
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
            << " tokens, EOF, overflow/sign recovery, print, cmp\n";
}
