#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/1_Basic/Pragma.cpp"

int main() {
  // The optimization directives are compile-time properties.  The helper is
  // the only runtime contract and must remain callable on every target.
#if defined(__x86_64__) || defined(__i386__)
  unsigned before = __builtin_ia32_stmxcsr();
#endif
  apply_pragmas();
#if defined(__x86_64__) || defined(__i386__)
  assert(__builtin_ia32_stmxcsr() == (before | 0x8040));
  apply_pragmas();
  assert(__builtin_ia32_stmxcsr() == (before | 0x8040));
  __builtin_ia32_ldmxcsr(before);
#endif

  volatile unsigned checksum = 0;
  for (unsigned i = 0; i < 257; ++i) checksum ^= i * 2654435761u;
  assert(checksum != 0);
  cout << "PASS Pragma helper callable; checksum=" << checksum << '\n';
}
