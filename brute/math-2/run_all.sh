#!/bin/sh
set -eu

# Deliberately sequential: this partition must keep total machine use <= 4 CPUs.
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
build="$root/.build"
mkdir -p "$build"

run_one() {
  name=$1
  dir="$root/$name"
  c++ -std=c++17 -O2 "$dir/harness.cpp" -o "$build/$name"
  "$build/$name" ${2-}
}

run_one miller_rabin
run_one pollard_rho
run_one primes "$root/../../codebook/6_Math/Primes.cpp"
run_one discrete_log
run_one chinese_remainder
run_one fac_no_p
run_one pi_count
run_one berlekamp_massey
echo "math-2: all 8 harnesses passed"
