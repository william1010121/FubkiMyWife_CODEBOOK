// Expected O(log n); int sizes fit, total <= RAND_MAX; stack for height.
// Merge disjoint trees with all keys in a <= all keys in b.
// Split outputs distinct, may alias root; interval needs
// 1 <= l <= size+1, 0 <= r <= size-l+1 (1-base start l, length r).
// split: <= key | > key; split2: first k nodes, 0 <= k <= size.
struct node {
  int data, sz; node *l, *r;
  node(int k) : data(k), sz(1), l(0), r(0) {}
  void up() { sz = 1 + (l ? l->sz : 0) + (r ? r->sz : 0); }
  void down() {} // lazy also needs down() before kth/Rank/erase reads
};
int sz(node *a) { return a ? a->sz : 0; }
node *merge(node *a, node *b) {
  if (!a || !b) return a ? a : b;
  if (rand() % (sz(a) + sz(b)) < sz(a))
    return a->down(), a->r = merge(a->r, b), a->up(), a;
  return b->down(), b->l = merge(a, b->l), b->up(), b;
}
void split(node *o, node *&a, node *&b, int k) {
  if (!o) return a = b = 0, void();
  o->down();
  if (o->data <= k) a = o, split(o->r, a->r, b, k), a->up();
  else b = o, split(o->l, a, b->l, k), b->up();
}
void split2(node *o, node *&a, node *&b, int k) {
  if (sz(o) <= k) return a = o, b = 0, void();
  o->down();
  if (sz(o->l) < k)
    a = o, split2(o->r, a->r, b, k - sz(o->l) - 1);
  else b = o, split2(o->l, a, b->l, k);
  o->up();
}
node *kth(node *o, int k) { // 1 <= k <= size
  int s = sz(o->l) + 1;
  if (k == s) return o;
  return k < s ? kth(o->l, k) : kth(o->r, k - s);
}
int Rank(node *o, int key) { // count keys < key
  if (!o) return 0;
  return o->data < key ? sz(o->l) + 1 + Rank(o->r, key)
                      : Rank(o->l, key);
}
bool erase(node *&o, int k) { // erase one copy
  if (!o) return 0;
  if (o->data == k) {
    node *t = o;
    o->down(), o = merge(o->l, o->r);
    delete t; return 1;
  }
  node *&t = k < o->data ? o->l : o->r;
  return erase(t, k) ? o->up(), 1 : 0;
}
void insert(node *&o, int k) {
  node *a, *b;
  split(o, a, b, k), o = merge(a, merge(new node(k), b));
}
void interval(node *&o, int l, int r) { // 1-base start l, length r
  node *a, *b, *c;
  split2(o, a, b, l - 1), split2(b, b, c, r);
  o = merge(a, merge(b, c)); // modify b before merging
}
