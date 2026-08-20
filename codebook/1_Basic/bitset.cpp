#include<bits/stdc++.h>
using namespace std;
#ifndef BITSET_NO_MAIN
int main () {
    bitset<4> bit;
    bit.all(); // all bit is true, ret tru;
    bit.any();  // any bit is true, ret true
    bit.none(); // all bit is false, ret true
    bit.count();
    bit.to_string('0', '1');//with parmeter
    bit.reset(); // set all to false
    bit.set(); // set all to true
    std::bitset<8> b3{0}, b4{42};
    std::hash<std::bitset<8>> hash_fn8;
    hash_fn8(b3); hash_fn8(b4);
}
#endif
