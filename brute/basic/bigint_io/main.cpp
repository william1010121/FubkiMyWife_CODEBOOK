#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
#include "../../../codebook/1_Basic/bigIntIO.cpp"

int main() {
  FILE *input = tmpfile();
  assert(input);
  fputs("-1234567890123456789", input);
  fflush(input);
  rewind(input);
  assert(dup2(fileno(input), fileno(stdin)) >= 0);
  __int128 x = read();
  assert(x == -1234567890123456789LL);
  print(x);
  putchar('\n');
}
