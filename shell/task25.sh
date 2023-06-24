#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments are expected"
    exit 1
fi

extensions=$(find ${1} -type f -name "*.*" -exec basename {} \; | sed 's/.*\.//' | sort -u)

for ext in ${extensions}; do
    mkdir -p "${2}/${ext}"
done

find $1 -type f -name "*.*" | while read file; do
    ext=$(echo ${file} | sed 's/.*\.//')
    cp "${file}" "${2}/${ext}/"
done
