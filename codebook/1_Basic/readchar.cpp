inline int readchar() { // byte or EOF; amortized O(1)
  static char buf[65536], *p = buf, *e = buf;
  if (p == e) p = buf, e = buf + fread_unlocked(buf, 1, sizeof buf, stdin);
  return p == e ? EOF : (unsigned char)*p++;
}
