#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Two arguments needed"
    exit 1
fi  

file="${1}"
directory=“${2}”

checksum=$(md5sum "${file}" | awk '{print $1}')

for f in $(md5sum "${directory}" -type f); do
    if [[ ${checksum} = $(md5sum "${f}" | awk '{print $1}') ]]; then
        echo "${f}"
    fi
done
