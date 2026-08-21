#include <bits/stdc++.h>
using namespace std;

#define N 1
#include "../../codebook/3_Data_Structure/BIT_kth.cpp"

int main() {
  for (int frequency = 0; frequency <= 1000; ++frequency) {
    fill(bit, bit + N + 1, 0);
    bit[1] = frequency;
    if (frequency == 0) continue;  // empty multiset has no valid kth query
    if (query_kth(1) != 1 || query_kth(frequency) != 1) {
      cerr << "BIT_kth singleton mismatch: frequency=" << frequency << '\n';
      return 1;
    }
  }
  return 0;
}
