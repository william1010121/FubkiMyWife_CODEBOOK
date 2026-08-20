int x = 0; char c = readchar();
while (c < '0' || c > '9') c = readchar();
while (c >= '0' && c <= '9')
  x = x * 10 + c - '0', c = readchar();
