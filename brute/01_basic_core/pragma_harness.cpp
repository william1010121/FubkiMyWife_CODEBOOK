#include <bits/stdc++.h>
using namespace std;

#include "../../codebook/1_Basic/Pragma.cpp"

int main() {
  // Optimization directives are compile-time properties. The helper is
  // the only runtime contract and must remain callable on every target.
#if defined(__x86_64__) || defined(__i386__)
  unsigned before = __builtin_ia32_stmxcsr();
#endif
  apply_pragmas();
#if defined(__x86_64__) || defined(__i386__)
  assert(__builtin_ia32_stmxcsr() == (before | 0x8040));
  apply_pragmas();
  assert(__builtin_ia32_stmxcsr() == (before | 0x8040));
  // Every rounding mode and initial DAZ/FTZ combination; preserve all
  // other bits, including exception flags/masks. Restore the caller state.
  for (unsigned mode = 0; mode < 4; ++mode)
    for (unsigned flags : {0u, 0x40u, 0x8000u, 0x8040u})
      for (unsigned status : {0u, 1u, 0x15u, 0x3fu}) {
        unsigned state = (before & ~0xe07fu) | (mode << 13) |
                         flags | status;
        __builtin_ia32_ldmxcsr(state);
        apply_pragmas();
        assert(__builtin_ia32_stmxcsr() == (state | 0x8040u));
        apply_pragmas();
        assert(__builtin_ia32_stmxcsr() == (state | 0x8040u));
      }
  __builtin_ia32_ldmxcsr(before);
#else
  for (int i = 0; i < 16; ++i) apply_pragmas();
#endif

  volatile unsigned checksum = 0;
  for (unsigned i = 0; i < 257; ++i) checksum ^= i * 2654435761u;
  assert(checksum != 0);
  cout << "PASS Pragma helper callable; checksum=" << checksum << '\n';
}
