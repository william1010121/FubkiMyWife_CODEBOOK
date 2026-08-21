#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

#define fread_unlocked fread
#include "../../codebook/1_Basic/readchar.cpp"

[[noreturn]] void fail(const string &message) {
  cerr << "FAIL readchar: " << message << '\n';
  exit(1);
}

int main() {
  // The template has no EOF sentinel.  Therefore the contract is checked up
  // to (and including) the final byte, without making an undefined extra read.
  const size_t size = 65536 + 37;
  vector<unsigned char> expected(size);
  for (size_t i = 0; i < size; ++i)
    expected[i] = static_cast<unsigned char>((i * 37 + 11) & 255);
  expected[0] = 0;
  expected[1] = '\n';
  expected[65535] = 0xff;
  expected[65536] = 0;
  expected.back() = 'Z';

  FILE *input = tmpfile();
  if (!input) fail("tmpfile");
  if (fwrite(expected.data(), 1, expected.size(), input) != expected.size())
    fail("write input");
  fflush(input);
  rewind(input);
  if (dup2(fileno(input), STDIN_FILENO) < 0) fail("redirect stdin");
  clearerr(stdin);

  for (size_t i = 0; i < expected.size(); ++i) {
    unsigned char got = static_cast<unsigned char>(readchar());
    if (got != expected[i])
      fail("byte " + to_string(i) + ": got " + to_string(got) +
           ", expected " + to_string(expected[i]));
  }

  cerr << "PASS readchar: " << expected.size()
       << " bytes, embedded NUL, 0xff, full-buffer refill, final byte\n";
}
