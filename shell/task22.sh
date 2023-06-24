#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

dir="${1}"
size="${2}"

find "${dir}" -type f -size "+${size}c" -printf "%s\n" | awk '{total += $1} END {print total " bytes"}'
