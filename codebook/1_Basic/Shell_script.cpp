#!/usr/bin/env bash
set -euo pipefail
stem=${1:?usage: compile.sh source-without-.cpp}
g++ -O2 -std=c++17 -Dbbq -Wall -Wextra -Wshadow -o "$stem" "$stem.cpp"
