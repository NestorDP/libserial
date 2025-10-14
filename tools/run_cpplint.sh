#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
# Files to check (exclude third_party)
FILES=$(git ls-files '*.cpp' '*.c' '*.h' '*.hpp' | grep -v -E '^third_party/')

for f in $FILES; do
  echo "cpplint: $f"
  cpplint --extensions=cpp,hpp,h,c --filter=-build/include_order "$f"
done