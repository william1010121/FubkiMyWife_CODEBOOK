ull basis[64];

void ins(ull x) {
    for (int i = 63; i >= 0; i--)
        if (x >> i & 1) {
            if (!basis[i]) return void(basis[i] = x);
            x ^= basis[i];
        }
}
