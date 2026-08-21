#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/3_Data_Structure/2D_Segment_Tree.cpp"

int main() {
  N = 1, M = 2;
  num[1][1] = -7;
  num[1][2] = -3;
  seg_1D tree;
  tree.init();
  // y=[2,2] is a partial query of the root y=[1,2], so it exercises the
  // branch whose local maximum accumulator is initialized to zero.
  int got = tree.query(1, 1, 2, 2);
  int want = -3;
  if (got != want) {
    cerr << "2D segment tree negative-value contract mismatch: got=" << got
         << " want=" << want
         << " (query accumulator currently starts at zero)\n";
    return 1;
  }
  return 0;
}
