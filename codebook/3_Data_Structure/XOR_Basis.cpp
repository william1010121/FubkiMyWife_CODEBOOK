unsigned long long basis[64]; // basis[i]: highest bit i; initially zero
// 64-bit unsigned long long; clear basis for each independent instance.
void ins(unsigned long long x) { // O(64), discard dependent values
  while (x) { int i = __lg(x);
    if (!basis[i]) return void(basis[i] = x);
    x ^= basis[i];
  }
}
