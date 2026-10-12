#include <bits/stdc++.h>
#include <unistd.h>
#include "../../codebook/1_Basic/bigIntIO.cpp"

int main() {
  const __uint128_t umax = ~__uint128_t(0);
  const __int128 max = static_cast<__int128>(umax >> 1);
  const __int128 min = -max - 1;
  const std::string expected[] = {
    "170141183460469231731687303715884105727",
    "-170141183460469231731687303715884105728",
    "340282366920938463463374607431768211455"
  };
  FILE *output = tmpfile();
  assert(output && fflush(stdout) == 0);
  int saved = dup(STDOUT_FILENO);
  assert(saved >= 0 && dup2(fileno(output), STDOUT_FILENO) >= 0);
  const int n = 200000;
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < n; ++i) {
    if (i % 3 == 0) print(max);
    else if (i % 3 == 1) print(min);
    else print_unsigned(umax);
    putchar('\n'); // locked and unlocked calls may share the stream
  }
  assert(fflush(stdout) == 0 && !ferror(stdout));
  double seconds = std::chrono::duration<double>(
    std::chrono::steady_clock::now() - start).count();
  assert(dup2(saved, STDOUT_FILENO) >= 0);
  close(saved);
  rewind(output);
  char line[64];
  for (int i = 0; i < n; ++i) {
    assert(fgets(line, sizeof line, output));
    assert(std::string(line) == expected[i % 3] + '\n');
  }
  assert(fgetc(output) == EOF && !ferror(output));
  fclose(output);
  std::cout << "PASS bigint_io output stress: " << n
            << " full-range values in " << seconds << "s\n";
}
