#pragma GCC optimize("Ofast,no-stack-protector,no-math-errno,unroll-loops")
#if defined(__x86_64__) || defined(__i386__)
#pragma GCC target("sse,sse2,sse3,ssse3,sse4")
#pragma GCC target("popcnt,abm,mmx,avx,arch=skylake")
inline void apply_pragmas() {
  __builtin_ia32_ldmxcsr(__builtin_ia32_stmxcsr() | 0x8040);
}
#else
inline void apply_pragmas() {}
#endif
