node *root = nullptr; insert(root, k); erase(root, k);
node *t = kth(root, i); // i-th, 1-base
int r = Rank(root, key); // # less than key
interval(root, l, r); // split2 by size
