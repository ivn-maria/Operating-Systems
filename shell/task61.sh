#!/bin/bash

if [[ "$USER" != "oracle" ]] && [[ "$USER" != "grid" ]]; then
  echo "This script can only be executed by the oracle or grid users"
  exit 1
fi

ORACLE_HOME="$ENV{ORACLE_HOME}"

if [[ ! -x "${ORACLE_HOME/bin/adrci" ]]; then
  echo "adrci is not executable"
  exit 0
fi

DIAG_DEST="/u01/app/$USER/diag_dest"
output="$("${ORACLE_HOME}/bin/adrci" exec="show homes")"

if echo "${output}" | grep -q "ADR Homes:"; then
    diag_dirs=$(echo "${output}" | awk '/diag/ {print $1}')
    for dir in ${diag_dirs}; do
        size=$(find "${dir}" -type f -printf '%s\n' | awk '{s+=$1} END {print s/1024}')
        printf '%s\t%s\n' "${size}" "${DIAG_DEST}/${dir}"
    done
else
    echo "No ADR homes are set"
fi
