string mcp(string s) { // O(n); unsigned bytes; 2*size fits int; empty OK
  int n = SZ(s), i = 0, j = 1; s += s;
  while (i < n && j < n) { int k = 0;
    while (k < n && s[i + k] == s[j + k]) ++k;
    if ((unsigned char)s[i+k] <= (unsigned char)s[j+k]) j += k+1;
    else i += k+1; if (i == j) ++j;
  } return s.substr(min(i, j), n);
}
