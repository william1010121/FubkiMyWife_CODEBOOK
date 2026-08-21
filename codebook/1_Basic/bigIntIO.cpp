bool read(__int128 &value) {
  int ch = getchar(); bool neg;
  while (true) {
    while (ch != EOF && ch != '-' && ch != '+' && !isdigit(ch)) ch = getchar();
    if (ch == EOF) return false;
    neg = ch == '-'; if (isdigit(ch) || isdigit(ch = getchar())) break;
  }
  __uint128_t x = 0, lim = ((__uint128_t)1 << 127) - !neg;
    bool overflow = false;
  do {
    unsigned d = ch - '0';
    if (x > (lim - d) / 10) overflow = true;
    else x = x * 10 + d;
  } while (isdigit(ch = getchar()));
  if (overflow) return false;
  value = !neg ? (__int128)x : x >> 127 ? -(__int128)(x - 1) - 1 : -(__int128)x;
  return true;
}
__int128 read() { __int128 value = 0; read(value); return value; }
void print_unsigned(__uint128_t x) {
  if (x > 9) print_unsigned(x / 10); putchar(x % 10 + '0');
}
void print(__int128 x) {
  if (x < 0) putchar('-');
  print_unsigned(x < 0 ? (__uint128_t)(-(x + 1)) + 1 : x);
}
bool cmp(__int128 x, __int128 y) { return x > y; }
