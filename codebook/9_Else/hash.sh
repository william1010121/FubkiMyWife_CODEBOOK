# Hashes a file, ignoring all whitespace and comments. Use for verifying that code was correctly typed.
set -euo pipefail
file=${1:?usage: hash.sh file}
normalized=$(cpp "$file" -dD -P -fpreprocessed | tr -d '[:space:]')
if command -v md5sum >/dev/null 2>&1; then
  printf '%s' "$normalized" | md5sum | cut -c-6
else
  printf '%s' "$normalized" | md5 -q | cut -c-6
fi
