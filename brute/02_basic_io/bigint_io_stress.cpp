#include <bits/stdc++.h>
#include <unistd.h>

#include "../../codebook/1_Basic/bigIntIO.cpp"

int main() {
  using i128 = __int128_t;
  const i128 max = static_cast<i128>((__uint128_t(1) << 127) - 1);
  const i128 min = -max - 1;
  // Both cutoff equality and strict inequality, with both signs.
  const std::vector<std::pair<std::string, i128>> cases = {
      {"170141183460469231731687303715884105727", max},
      {"-170141183460469231731687303715884105728", min},
      {"170141183460469231731687303715884105720", max - 7},
      {"-170141183460469231731687303715884105720", min + 8},
      {"170141183460469231731687303715884105719", max - 8},
      {"-170141183460469231731687303715884105719", min + 9},
  };
  const int repetitions = 50000;
  FILE *input = tmpfile();
  assert(input);
  for (int i = 0; i < repetitions; ++i)
    for (const auto &[token, value] : cases) {
      assert(fwrite(token.data(), 1, token.size(), input) == token.size());
      assert(fputc(' ', input) != EOF);
    }
  rewind(input);
  assert(dup2(fileno(input), STDIN_FILENO) >= 0);
  clearerr(stdin);
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < repetitions; ++i)
    for (const auto &[token, expected] : cases) {
      i128 value = 0;
      if (!read(value) || value != expected) {
        std::cerr << "FAIL bigint_io stress repetition=" << i
                  << " token=" << token << '\n';
        return 1;
      }
    }
  i128 value = 123;
  assert(!read(value) && value == 123);
  double seconds = std::chrono::duration<double>(
      std::chrono::steady_clock::now() - start).count();
  std::cout << "PASS bigint_io stress: " << repetitions * cases.size()
            << " near-limit tokens in " << seconds << "s\n";
  fclose(input);
}
