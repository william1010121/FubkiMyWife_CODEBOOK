#include <bits/stdc++.h>
using namespace std;
#define BITSET_NO_MAIN
#include "../../../codebook/1_Basic/bitset.cpp"

int main() {
  bitset<8> b;
  assert(b.none());
  b.set(2), b.set(7);
  assert(b.count() == 2 && b.any() && !b.all());
  b.reset(2);
  assert(b.count() == 1 && b[7]);
  b.reset();
  assert(b.none());
  b.set();
  assert(b.all());
}
