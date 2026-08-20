#include <bits/stdc++.h>
using namespace std;
#include "../../../codebook/3_Data_Structure/Smart_Pointer.cpp"

int main() {
  auto a = new_reference(42);
  assert(*a == 42);
  auto b = a;
  *b = 17;
  assert(*a == 17);
  b = reference_pointer<int>();
  assert(*a == 17);
  a = new_reference(99);
  assert(*a == 99);
  auto p1 = new_reference(P(2, 3));
  auto p2 = p1;
  assert(p2->a == 2 && p2->b == 3);
  p1 = reference_pointer<P>();
  assert(p2->a == 2);
}
