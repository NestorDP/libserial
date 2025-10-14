#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

# Check-only mode: exit non-zero when formatting differs
find "${ROOT_DIR}/src" "${ROOT_DIR}/include" "${ROOT_DIR}/test" -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \
  | grep -v '^./third_party/' \
  | xargs -r -n1 uncrustify -c "${ROOT_DIR}/uncrustify.cfg" --no-backup