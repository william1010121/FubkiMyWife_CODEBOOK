// 64-bit unsigned long long; libstdc++ __lg.
unsigned long long basis[64]; // basis[i]: highest bit i.
void ins(unsigned long long x) { // O(64); zero each test
  while (x) { int i = __lg(x);
    if (!basis[i]) return void(basis[i] = x);
    x ^= basis[i];
  }
}
