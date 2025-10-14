#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

if [ -f "${ROOT_DIR}/build/compile_commands.json" ]; then
  CPPCHECK_PROJECT="--project=${ROOT_DIR}/build/compile_commands.json"
else
  CPPCHECK_PROJECT=""
fi

EXCLUDES="--suppress=missingIncludeSystem --enable=all --inconclusive -I include -j$(nproc) --quiet"

# Run cppcheck
cppcheck ${EXCLUDES} ${CPPCHECK_PROJECT} "${ROOT_DIR}/src" "${ROOT_DIR}/include"