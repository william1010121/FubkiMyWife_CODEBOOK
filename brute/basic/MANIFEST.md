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
| Compile script | `codebook/1_Basic/Shell_script.cpp` | N/A (shell utility) | `../22_inactive_extra/language_tools_brute.py` compiles and runs a temporary C++17 program. |
| Java BigInteger | `codebook/10_JAVA/Big_number.cpp` | N/A (language fallback) | `../22_inactive_extra/language_tools_brute.py` checks the source contract and compiles/differential-tests it when `javac` is installed. |
| Python misc | `codebook/11_Python/misc.py` | N/A (language fallback) | `../21_language_misc/python_misc_brute.py` imports and exercises every helper. |

The inactive `Default_code.cpp` and `Texas_holdem.cpp` are covered by the
focused harnesses under `brute/01_basic_core/`, including exhaustive five-card
poker enumeration.
