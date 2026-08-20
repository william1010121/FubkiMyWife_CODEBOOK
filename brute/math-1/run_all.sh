#!/bin/sh
set -eu
base=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
for name in ax_by_gcd floor_ceil floor_enumeration mod_min mod_inverse get_mu \
            gaussian_gcd gaussian_elimination floor_sum fraction simultaneous_equations
do
  c++ -std=c++17 -O2 -Wall -Wextra -pedantic "$base/$name/harness.cpp" -o "$base/$name/harness"
  "$base/$name/harness"
done
