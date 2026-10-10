#include <bits/extc++.h>
#include <ext/rope>
using namespace __gnu_pbds; using namespace __gnu_cxx;
typedef tree<int, null_type, std::less<int>, rb_tree_tag,
  tree_order_statistics_node_update> tree_set;
typedef cc_hash_table<int, int> umap;
typedef __gnu_pbds::priority_queue<int> heap;
#ifndef BLACK_MAGIC_NO_MAIN
int main() {
  mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
  auto get_rand = [&](int l, int r) { // l<=r
    return uniform_int_distribution<int>(l, r)(rng);
  };
  vector<int> v = {1, 2, 3}; get_rand(0, 10);
  shuffle(v.begin(), v.end(), rng);
  tree_set s; s.insert(71), s.insert(22); // O(log n), 0-based rank
  assert(*s.find_by_order(0) == 22 && *s.find_by_order(1) == 71);
  assert(s.order_of_key(22) == 0 && s.order_of_key(71) == 1);
  s.erase(22);
  assert(*s.find_by_order(0) == 71 && s.order_of_key(71) == 0);
  // Meldable heap: distinct heaps; join empties b.
  heap a, b; a.push(1), b.push(2), a.join(b);
  rope<char> r[2]; r[1] = r[0]; // persistent copy
  r[1].insert(0, "abc"), r[1].erase(1, 1);
  std::cout << r[1].substr(0, 2) << std::endl;
}
#endif
