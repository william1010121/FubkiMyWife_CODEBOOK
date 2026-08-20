#include <bits/stdc++.h>
#include <bits/extc++.h>
#include <ext/rope>
#define BLACK_MAGIC_NO_MAIN
#include "../../../codebook/1_Basic/black_magic.cpp"
using namespace std;

int main() {
  tree_set s;
  for (int x : {7, 2, 9, 4}) s.insert(x);
  assert(*s.find_by_order(2) == 7 && s.order_of_key(7) == 2);
  __gnu_pbds::priority_queue<int> a, b;
  a.push(3), b.push(5), a.join(b);
  assert(a.top() == 5 && b.empty());
  __gnu_cxx::rope<char> r("abc");
  r.insert(1, "X");
  assert(string(r.begin(), r.end()) == "aXbc");
}
