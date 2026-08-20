inline char readchar() {
  static char buf[65536], *p = buf, *end = buf;
  if (p == end) end = buf + fread_unlocked(buf, 1, sizeof buf, stdin), p = buf;
  return *p++;
}
