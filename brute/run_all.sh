#!/usr/bin/env bash
set -u

root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
test_dir="$root/brute"
bin_dir="$root/brute/bin"
log_dir="$root/brute/logs"
mkdir -p "$bin_dir" "$log_dir"

requested=${JOBS:-2}
case "$requested" in
  ''|*[!0-9]*) requested=2 ;;
esac
if [ "$requested" -lt 1 ]; then requested=1; fi
if [ "$requested" -gt 4 ]; then requested=4; fi
timeout_seconds=${TIMEOUT_SECONDS:-60}
case "$timeout_seconds" in
  ''|*[!0-9]*) timeout_seconds=60 ;;
esac
if [ "$timeout_seconds" -lt 1 ]; then timeout_seconds=1; fi

export OMP_NUM_THREADS=1
export OPENBLAS_NUM_THREADS=1
export MKL_NUM_THREADS=1
export VECLIB_MAXIMUM_THREADS=1
export PYTHONDONTWRITEBYTECODE=1

if ! python3 "$test_dir/audit_coverage.py"; then
  exit 1
fi

mapfile -d '' tests < <(
  find "$test_dir" -type f \( -name '*.cpp' -o -name '*_brute.py' \) -print0 | sort -z
)
if [ "${#tests[@]}" -eq 0 ]; then
  echo "no brute tests found" >&2
  exit 2
fi

run_one() {
  local src=$1
  local rel=${src#"$test_dir"/}
  local stem=${rel%.cpp}
  local exe="$bin_dir/$stem"
  local log="$log_dir/$stem.log"
  mkdir -p "$(dirname -- "$exe")" "$(dirname -- "$log")"
  case "$src" in
    *.cpp)
      if ! g++ -std=c++20 -O2 -pipe -I"$root" "$src" -o "$exe" >"$log" 2>&1; then
        echo "FAIL compile $rel"
        return 1
      fi
      if ! timeout "${timeout_seconds}s" "$exe" >>"$log" 2>&1; then
        echo "FAIL run     $rel"
        return 1
      fi
      ;;
    *_brute.py)
      if ! timeout "${timeout_seconds}s" python3 "$src" >"$log" 2>&1; then
        echo "FAIL run     $rel"
        return 1
      fi
      ;;
    *)
      echo "FAIL unknown $rel"
      return 1
      ;;
  esac
  echo "PASS        $rel"
  return 0
}
export -f run_one
export root test_dir bin_dir log_dir timeout_seconds

printf '%s\0' "${tests[@]}" | xargs -0 -r -n1 -P"$requested" bash -c 'run_one "$1"' _
status=$?
echo "runner: jobs=$requested timeout=${timeout_seconds}s tests=${#tests[@]} status=$status"
exit "$status"
