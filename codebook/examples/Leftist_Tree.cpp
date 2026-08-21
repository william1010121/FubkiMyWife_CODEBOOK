node *a = new node(x), *b = new node(y); // MAX-heap
a = merge(a, b); // O(log n)
ll top = a->data; pop(a);
// a->sz, a->sum kept per subtree
