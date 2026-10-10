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
  // Check every byte as int, so 0xff must remain distinct from EOF.
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
    int got = readchar();
    if (got != expected[i])
      fail("byte " + to_string(i) + ": got " + to_string(got) +
           ", expected " + to_string(expected[i]));
  }
  for (int i = 0; i < 3; ++i)
    if (readchar() != EOF) fail("EOF must be stable across repeated reads");

  // Redirect only after the entire old stream and private buffer are drained.
  for (size_t length : {0, 1, 65535, 65536, 65537, 131072, 131073}) {
    FILE *edge = tmpfile();
    if (!edge) fail("boundary tmpfile");
    vector<unsigned char> bytes(length);
    for (size_t i = 0; i < length; ++i) bytes[i] = (i * 131 + 255) & 255;
    if (fwrite(bytes.data(), 1, length, edge) != length) fail("boundary write");
    fflush(edge), rewind(edge);
    if (dup2(fileno(edge), STDIN_FILENO) < 0) fail("boundary redirect");
    clearerr(stdin);
    for (size_t i = 0; i < length; ++i)
      if (readchar() != bytes[i]) fail("boundary byte " + to_string(i));
    for (int i = 0; i < 10; ++i)
      if (readchar() != EOF) fail("boundary repeated EOF");
    fclose(edge);
  }

  cerr << "PASS readchar: " << expected.size()
       << " bytes plus 7 boundary streams, NUL, 0xff, repeated EOF\n";
}
