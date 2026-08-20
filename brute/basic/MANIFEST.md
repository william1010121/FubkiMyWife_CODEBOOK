# Basic partition manifest

All harnesses in this directory are independent C++17 programs. Run one at a
time with one compiler process; no harness starts threads or child workers.

| Template | Source | Online problem / reference | Local oracle test |
|---|---|---|---|
| vimrc | `codebook/1_Basic/vimrc.cpp` | N/A (editor configuration) | `vimrc/main.cpp` checks the shipped config contains the key mappings and editor options. |
| readchar | `codebook/1_Basic/readchar.cpp` | N/A (I/O primitive) | `readchar/main.cpp` reads a fixed nine-byte stream and checks every byte. |
| BigIntIO | `codebook/1_Basic/bigIntIO.cpp` | N/A (I/O primitive) | `bigint_io/main.cpp` parses and prints a signed `__int128`; shell check compares exact output. |
| Black Magic | `codebook/1_Basic/black_magic.cpp` | N/A (collection of GNU extension examples) | `black_magic/main.cpp` checks PBDS order statistics, meldable heap, and rope persistence operations. |
| Pragma Optimization | `codebook/1_Basic/Pragma.cpp` | N/A (compiler directives) | `pragma/main.cpp` compiles and calls the MXCSR helper. |
| Bitset | `codebook/1_Basic/bitset.cpp` | N/A (standard-library API reference) | `bitset/main.cpp` exhaustively checks set/reset/count/any/none/all on a small bitset. |

The inactive `Shell_script.cpp`, `Default_code.cpp`, and `Texas_holdem.cpp`
are not included by `content.tex`; they are outside the active Basic template
set. `Texas_holdem.cpp` still has no complete standalone problem harness.
