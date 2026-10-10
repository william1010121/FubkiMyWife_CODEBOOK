#include<bits/stdc++.h>
using namespace std;
#ifndef BITSET_NO_MAIN
int main() {
  bitset<4> bit;
  bit.all(), bit.any(), bit.none(); // all / any / no set bits
  bit.count(), bit.to_string('0', '1'); // popcount / custom digits
  bit.reset(), bit.set(); // clear all to 0 / fill all with 1
  bitset<8> b3{0}, b4{42}; hash<bitset<8>> hash_fn8;
  hash_fn8(b3), hash_fn8(b4);
}
#endif
