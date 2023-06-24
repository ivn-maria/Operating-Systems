#!/bin/bash

if [[ $# -ne 2 ]]; then
  echo "Two arguments are expected"
  exit 1
fi

sorted_lines=$(cat "${1}" | sort -n)

for line in ${sorted_lines}; do
    formatted_line=$(echo ${line} | sed ’s/^([0-9]*//‘)
    if grep -qF "${formatted_line}" "${2}"; then
        continue
    else
        echo "${line}" >> "${2}"
    fi
done
