bool read(__int128 &value) {
    int ch = getchar();
    bool neg = false;
    while (ch != EOF) {
        while (ch != EOF && ch != '-' && ch != '+' && (ch < '0' || ch > '9')) ch = getchar();
        if (ch == EOF) return false;
        if (ch != '-' && ch != '+') { neg = false; break; }
        neg = ch == '-';
        ch = getchar();
        if (ch >= '0' && ch <= '9') break;
    }
    if (ch == EOF) return false;
    __uint128_t x = 0, lim = ((__uint128_t)1 << 127) - !neg;
    bool overflow = false;
    while (ch >= '0' && ch <= '9') {
        unsigned digit = ch - '0';
        if (x > (lim - digit) / 10) overflow = true;
        else x = x * 10 + digit;
        ch = getchar();
    }
    if (overflow) return false;
    if (!neg) value = (__int128)x;
    else if (x == ((__uint128_t)1 << 127))
        value = -(__int128)(x - 1) - 1;
    else value = -(__int128)x;
    return true;
}
__int128 read() { __int128 value = 0; read(value); return value; }
void print_unsigned(__uint128_t x) {
    if (x > 9) print_unsigned(x / 10);
    putchar(x % 10 + '0');
}
void print(__int128 x) {
    __uint128_t magnitude;
    if (x < 0) putchar('-'), magnitude = (__uint128_t)(-(x + 1)) + 1;
    else magnitude = x;
    print_unsigned(magnitude);
}
bool cmp(__int128 x, __int128 y) { return x > y; }
