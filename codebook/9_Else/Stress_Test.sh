#!/usr/bin/env bash
set -euo pipefail
generator=${1:?usage: stress.sh generator brute solution [iterations]}
brute=${2:?usage: stress.sh generator brute solution [iterations]}
solution=${3:?usage: stress.sh generator brute solution [iterations]}
iterations=${4:-0} # 0 means run until the first mismatch.
work=$(mktemp -d)
trap 'rm -rf -- "$work"' EXIT

for ((case_id = 0; iterations == 0 || case_id < iterations; ++case_id)); do
  "$generator" >"$work/input.txt"
  "$brute" <"$work/input.txt" >"$work/brute.txt"
  "$solution" <"$work/input.txt" >"$work/output.txt"
  if ! diff -u "$work/brute.txt" "$work/output.txt"; then
    cp "$work/input.txt" stress-failure.txt
    echo "mismatch at case $case_id; input saved to stress-failure.txt" >&2
    exit 1
  fi
done
