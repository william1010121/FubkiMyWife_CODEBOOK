// Integer-domain dynamic Li Chao tree. less<T> keeps maximum, greater<T> minimum.
template<class val = ll, class compare = less<val>>
struct DynamicConvexTrick {
  static constexpr val minx = 0, maxx = val(1e9) + 5;
  inline static compare comp{};
  struct Line {
    val a = 0, b = 0;
    val operator()(val x) const { return a * x + b; }
  };
  struct Node {
    Line line;
    int ch[2] = {-1, -1};
    bool has_line = false;
  };
  vector<Node> tree = vector<Node>(1);

  int new_node() {
    tree.emplace_back();
    return (int)tree.size() - 1;
  }
  void insert(int node, val l, val r, Line line) {
    if (!tree[node].has_line) {
      tree[node].line = line, tree[node].has_line = true;
      return;
    }
    val mid = l + (r - l) / 2;
    bool left = comp(tree[node].line(l), line(l));
    bool middle = comp(tree[node].line(mid), line(mid));
    if (middle) swap(tree[node].line, line);
    if (l == r) return;
    int side = left == middle;
    val nl = side ? mid + 1 : l, nr = side ? r : mid;
    if (tree[node].ch[side] == -1) tree[node].ch[side] = new_node();
    insert(tree[node].ch[side], nl, nr, line);
  }
  void ins(val a, val b) { insert(0, minx, maxx, {a, b}); }
  val operator()(val x) const {
    assert(minx <= x && x <= maxx && tree[0].has_line);
    int node = 0; val l = minx, r = maxx, answer = tree[0].line(x);
    while (true) {
      if (tree[node].has_line && comp(answer, tree[node].line(x)))
        answer = tree[node].line(x);
      if (l == r) break;
      val mid = l + (r - l) / 2;
      int side = x > mid;
      if (tree[node].ch[side] == -1) break;
      node = tree[node].ch[side];
      if (side) l = mid + 1; else r = mid;
    }
    return answer;
  }
};

DynamicConvexTrick<> DCT;
