Init(); // build omega table once
Transform(a, n); Transform(b, n); // n = 2^k
for (int i = 0; i < n; ++i)
  a[i] = 1LL * a[i] * b[i] % kMod;
InverseTransform(a, n);
