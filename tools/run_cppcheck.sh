#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

if [ -f "${ROOT_DIR}/build/compile_commands.json" ]; then
  # When compile_commands.json is present prefer project mode. Do not pass
  # source files together with --project (cppcheck rejects that).
  CPPCHECK_PROJECT="--project=${ROOT_DIR}/build/compile_commands.json"
  # In project mode we avoid passing -j so cppcheck can make correct unusedFunction analysis
  CPPCHECK_PARALLEL=""
  CPPCHECK_TARGETS=""
else
  CPPCHECK_PROJECT=""
  CPPCHECK_PARALLEL="-j$(nproc)"
  CPPCHECK_TARGETS="${ROOT_DIR}/src ${ROOT_DIR}/include ${ROOT_DIR}/test"
fi

# Common cppcheck options
CPPCHECK_SUPPRESS=""
# If running in parallel, suppress checks that are incompatible with -j
if [ -n "${CPPCHECK_PARALLEL}" ]; then
  CPPCHECK_SUPPRESS="--suppress=unusedFunction"
fi

CPPCHECK_COMMON=(--suppress=missingIncludeSystem --enable=all --inconclusive -I "${ROOT_DIR}/include" --quiet --suppress=unusedFunction)
if [ -n "${CPPCHECK_SUPPRESS}" ]; then
  CPPCHECK_COMMON+=("${CPPCHECK_SUPPRESS}")
fi

# Run cppcheck: either in project mode or on source paths
if [ -n "${CPPCHECK_PROJECT}" ]; then
  cppcheck "${CPPCHECK_PROJECT}" "${CPPCHECK_COMMON[@]}"
else
  cppcheck "${CPPCHECK_COMMON[@]}" ${CPPCHECK_PARALLEL} ${CPPCHECK_TARGETS}
fi