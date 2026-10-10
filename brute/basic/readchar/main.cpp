#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
#define fread_unlocked fread
#include "../../../codebook/1_Basic/readchar.cpp"

int main() {
  const string want = "readchar\n";
  FILE *input = tmpfile();
  assert(input);
  assert(dup2(fileno(input), fileno(stdin)) >= 0);
  clearerr(stdin);
  assert(readchar() == EOF); // initially empty input
  fputs(want.c_str(), input);
  fflush(input), rewind(input);
  assert(dup2(fileno(input), fileno(stdin)) >= 0);
  clearerr(stdin);
  for (char c : want) assert(readchar() == c);
  assert(readchar() == EOF);
}
