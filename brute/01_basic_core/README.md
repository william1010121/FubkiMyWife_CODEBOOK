# Basic core boundary tests

This directory is the test-only extension for `Default_code.cpp`,
`Pragma.cpp`, `Shell_script.cpp`, and `Texas_holdem.cpp`.  Each C++ harness is
single-process, deterministic, and uses no threads or child workers.

`default_code_harness.cpp` checks the aliases, pair access macros, and the
vector/array behavior of `SZ`, `ALL`, and `pb`.  `pragma_harness.cpp` checks
that the cross-platform runtime helper remains callable; compiler pragmas
themselves are compile-time directives and have no portable runtime oracle.

`texas_holdem_harness.cpp` compares every `C(52,5) = 2,598,960` distinct hand
against an independent five-card poker evaluator.  It additionally checks the
string insertion overload, reset/reuse, shuffled insertion order, 20,000
seeded random hands, and 40,000 seeded comparator checks.

`Shell_script.cpp` is a shell utility rather than a C++ translation unit.
`brute/22_inactive_extra/language_tools_brute.py` executes it inside a temporary
directory, then runs the generated binary and checks its output.

Run the three C++ checks one at a time from the repository root, for example:

```sh
rbox g++ -std=c++17 -O2 -pipe -I. brute/01_basic_core/default_code_harness.cpp -o /tmp/default_code_harness
/tmp/default_code_harness
```
