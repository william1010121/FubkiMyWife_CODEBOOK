// Unlocked stdio, one thread; skips junk; adjacent sign.
bool read(__int128 &value) { // EOF/overflow: unchanged
  int ch = getchar_unlocked(), prev = 0;
  bool overflow = false;
  while (ch != EOF && !isdigit(ch))
    prev = ch, ch = getchar_unlocked();
  if (ch == EOF) return false; bool neg = prev == '-';
  __uint128_t x = 0, lim = ((__uint128_t)1 << 127) - !neg;
  do { unsigned d = ch - '0';
    if (x > (lim - d) / 10) overflow = true;
    else x = x * 10 + d;
  } while (isdigit(ch = getchar_unlocked())); // eats delim
  if (overflow) return false;
  value = !neg ? (__int128)x : x >> 127 ?
    -(__int128)(x - 1) - 1 : -(__int128)x;
  return true;
}
__int128 read() { __int128 x = 0; read(x); return x; }
void print_unsigned(__uint128_t x) { // 64-bit if possible
  auto q = x >> 64 ? x / 10 : (uint64_t)x / 10;
  if (q) print_unsigned(q);
  putchar_unlocked(x - q * 10 + '0');
}
void print(__int128 x) {
  if (x < 0) putchar_unlocked('-');
  print_unsigned(x < 0 ? (__uint128_t)(-(x + 1)) + 1 : x);
}
bool cmp(__int128 x, __int128 y) { return x > y; }
